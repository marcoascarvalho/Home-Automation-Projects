#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* serial */
#define UART "/dev/ttyS1"
struct termios tio;
unsigned char c;
//int tty_fd;

/*socket */
#define MAXPENDING 5    /* Max connection requests */
#define BUFFSIZE 32
void Die(char *mess)
{
	perror(mess);
	exit(1);
}

#define DEBUG_MESSAGES 0

/* HAProt */

#define HA_ASCII_SOH         (0x55)

/*Message field position */
#define HA_POS_SOH              (0)  //Header position
#define HA_POS_COMMAND          (1)  //Command position
#define HA_POS_SIZE             (2)  //Size position
#define HA_POS_DATA             (3)  //Data position

/*Socket message bitfield position */
#define SOCKET_POS_COMMAND          (0)  //Command position
#define SOCKET_POS_DATA             (1)  //Data position

/*Message byte quantity */
#define HA_HEADER_BYTES      (3)
#define HA_TRAILER_BYTES     (1)
#define HA_CKS_BYTES         (1)
#define HA_EXTRA_BYTES       (HA_HEADER_BYTES + HA_TRAILER_BYTES)
#define HA_MSG_SZ_MIN        (HA_EXTRA_BYTES)
#define MSG_MIN_SZ           (HA_HEADER_BYTES + HA_TRAILER_BYTES + HA_CKS_BYTES)

typedef enum
{
    HA_SET_UNIT                 = 0x30,  //ASCII 0
    HA_GET_UNIT                 = 0x31,  //ASCII 1
    HA_LIGHT_DIM                = 0x32,  //ASCII 2
    HA_PROG_UNIT                = 0x33,  //ASCII 3
    HA_SET_DATETIME             = 0x34,  //ASCII 4
    HA_GET_DATETIME             = 0x35,  //ASCII 5
    HA_SET_HOUSE_ADDR           = 0x36,  //ASCII 6
    HA_GET_HOUSE_ADDR           = 0x37,  //ASCII 7
    HA_SET_UNIT_ADDR            = 0x38,  //ASCII 8
    HA_GET_UNIT_ADDR            = 0x39,  //ASCII 9
    HA_SET_LIGHT_SENSOR         = 0x3A,  //ASCII :
    HA_GET_LIGHT_SENSOR         = 0x3B   //ASCII ;
} CmdHAProt_t;


static int setup_tty(char *tty_file)
{

	int fd;
	printf("Configuring %s port\n", tty_file);

	memset(&tio,0,sizeof(tio));

	/* c_iflag field describes the basic terminal input control */
	tio.c_iflag = 0;

	/* c_oflag field specifies the system treatment of output */
    tio.c_oflag = 0;
	tio.c_cflag = 0;

	/*
    CS8     : 8n1 (8bit,no parity,1 stopbit)
    CLOCAL  : local connection, no modem contol
    CREAD   : enable receiving characters
	*/
	tio.c_cflag = CS8 | CREAD | CLOCAL;

	/*c_lflag field of the argument structure is used to control various terminal functions */
	tio.c_lflag = 0;

	/* blocking read until 1 character arrives */
    tio.c_cc[VMIN] = 1;

    /* inter-character timer unused */
    tio.c_cc[VTIME] = 0;

    /* Open the device */
    fd = open(tty_file, O_RDWR | O_NONBLOCK); // make the reads non-blocking
	if ( fd == -1 )
	{
		printf("Open %s port failed", tty_file);
		//rc = tty_fd;
		exit(-1);
	}

	/* Setup baudrate for input and output */
    cfsetospeed(&tio, B115200);
    cfsetispeed(&tio, B115200);


    /* now clean the modem line and activate the settings for the port */
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);

    printf("%s: open and configured: successful\n", tty_file);

    return(fd);

}

unsigned char ComputeChkSum(const unsigned char *aBuffer, const unsigned short aSize)
{
    unsigned char chk = 0;
    unsigned short i;

    for (i = 0; i < aSize; i++)
    {
        chk ^= aBuffer[i];
    }

    return chk;
}

void AppendChkSum(unsigned char *aBuffer, const unsigned short aSize)
{
    aBuffer[aSize - 1] = ComputeChkSum(aBuffer, (unsigned short) (aSize - 1));
}

void HAProcessCommand(unsigned char *DataToSendBuffer, int nBytesReceivedFromSocket, int HAfd)
{

	unsigned char SerialTXBuffer[BUFFSIZE];
	int i;
	//unsigned char cks = 0;
	unsigned short nBytesToTransmit;
	unsigned char CmdValid = 0;

	printf("Buffer_len received = %d\n", nBytesReceivedFromSocket);

	/* Setup header */
	SerialTXBuffer[HA_POS_SOH] = HA_ASCII_SOH;

	/* get the command */
	SerialTXBuffer[HA_POS_COMMAND] = DataToSendBuffer[SOCKET_POS_COMMAND];

	printf("Command received %x\n", SerialTXBuffer[HA_POS_COMMAND]);

    /* Set the command size */
	switch (SerialTXBuffer[HA_POS_COMMAND])
    {

        case HA_SET_UNIT:

        	SerialTXBuffer[HA_POS_SIZE] = 3; //size
        	CmdValid = 1;

        break;

        case HA_GET_UNIT:


        break;

        case HA_LIGHT_DIM:

        	SerialTXBuffer[HA_POS_SIZE] = 3; //size
        	CmdValid = 1;

        break;

        case HA_PROG_UNIT:

        	SerialTXBuffer[HA_POS_SIZE] = 10; //size
        	CmdValid = 1;


        break;

        case HA_SET_DATETIME:

        	SerialTXBuffer[HA_POS_SIZE] = 6; //size
        	CmdValid = 1;

        break;

        case HA_GET_DATETIME:

        	SerialTXBuffer[HA_POS_SIZE] = 2; //size
        	CmdValid = 1;

        break;

        case HA_SET_HOUSE_ADDR:

        	SerialTXBuffer[HA_POS_SIZE] = 1; //size
        	CmdValid = 1;

        break;

        case HA_GET_HOUSE_ADDR:


        break;

        case HA_SET_UNIT_ADDR:

        	SerialTXBuffer[HA_POS_SIZE] = 1; //size
        	CmdValid = 1;

        break;

        case HA_GET_UNIT_ADDR:


        break;

        case HA_SET_LIGHT_SENSOR:

        	SerialTXBuffer[HA_POS_SIZE] = 4; //size
        	CmdValid = 1;

        break;

        case HA_GET_LIGHT_SENSOR:

        	SerialTXBuffer[HA_POS_SIZE] = 2; //size
        	CmdValid = 1;

        break;

        default:
          break;

    }

	printf ("CmdValid = %d\n", CmdValid);

	/* If command is valid and the data received from socket */
	/*   match the number of the command data                */
	if( (CmdValid == 1) && (SerialTXBuffer[HA_POS_SIZE] == (nBytesReceivedFromSocket-1)))
	{
		/* Fill buffer to send over the serial port */
		for(i=0; i < SerialTXBuffer[HA_POS_SIZE]; i++)
		{
			SerialTXBuffer[HA_POS_DATA+i] = DataToSendBuffer[SOCKET_POS_DATA+i];
		}

		nBytesToTransmit = HA_EXTRA_BYTES + SerialTXBuffer[HA_POS_SIZE];

		printf("nBytesToTransmit = %d\n", nBytesToTransmit);

		/* Compute Checksum */
		AppendChkSum(SerialTXBuffer, nBytesToTransmit);

		printf("Data Send to %s: [", UART);
		for ( i = 0; i < nBytesToTransmit; i++ )
		{
			printf(" %2x ", SerialTXBuffer[i]);
		}
		printf("]\n");

		write(HAfd, SerialTXBuffer, nBytesToTransmit);

	}

    return;

}

void HandleClient(int sock, int fd)
{
	unsigned char SocketBuffer[BUFFSIZE];
	int nBytesReceived = -1;

	/* Init socket Buffer */
	SocketBuffer[0] = '\0';

	/* Receive message */
	if ((nBytesReceived = recv(sock, SocketBuffer, BUFFSIZE, 0)) < 0)
	{
		Die("Failed to receive initial bytes from client");
	}

	/* Send bytes and check for more incoming data in loop */
	while (nBytesReceived > 0)
	{
		/* Send back received data */
		//if (send(sock, buffer, received, 0) != received)
		//{
		//	Die("Failed to send bytes to client");
		//}

		HAProcessCommand(SocketBuffer, nBytesReceived, fd);

		/* reinicializa buffer de recepcao de dados do socket */
		SocketBuffer[0] = '\0';

		/* Check for more data */
		if ((nBytesReceived = recv(sock, SocketBuffer, BUFFSIZE, 0)) < 0)
		{
			Die("Failed to receive additional bytes from client");
		}

	}

	//if (read(tty_fd,&c,1)>0)
	//{
	//	printf("Recebido %s \n",&c);
	//	write(tty_fd,&c,1);
	//}

	close(sock);

}

int main(int argc, char **argv)
{

    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;
    int tty_fd;
	unsigned int NumBytesRxPort = 0;
	int i;
	unsigned char SerialRXBuffer[BUFFSIZE];


    if (argc != 2)
    {
    	fprintf(stderr, "USAGE: HA_server <port>\n");
    	exit(1);
    }

    /* Create the TCP socket */
    if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
    	Die("Failed to create socket");
    }

    /* Construct the server sockaddr_in structure */
    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = AF_INET;                  /* Internet/IP */
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
    echoserver.sin_port = htons(atoi(argv[1]));       /* server port */

    /* Bind the server socket */
    if (bind(serversock, (struct sockaddr *) &echoserver,
                               sizeof(echoserver)) < 0)
    {
    	Die("Failed to bind the server socket");
    }

    /* Listen on the server socket */
    if (listen(serversock, MAXPENDING) < 0)
    {
    	Die("Failed to listen on server socket");
    }

	/* serial */
	tty_fd = setup_tty(UART);


	while (1)
    {

        unsigned int clientlen = sizeof(echoclient);

    	/* verifica se existem bytes a serem lidos da serial */
    	ioctl(tty_fd, FIONREAD, &NumBytesRxPort);

    	if(NumBytesRxPort > 0)
    	{
    		/*Faz a Leitura de dados da serial*/
    		read(tty_fd, SerialRXBuffer, NumBytesRxPort);

    		printf("Received from Serial Port = [ ");
    		for (i = 0; i < NumBytesRxPort; i++)
    		{
    			//printf(" %2x ",SerialRXBuffer[i]);
    			printf("%c",SerialRXBuffer[i]);
    		}
    		printf("]\n");

    	}


        /* Wait for client connection */
        if ((clientsock =
             accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0)
        {
        	Die("Failed to accept client connection");
        }

        fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));

        HandleClient(clientsock, tty_fd);

    }

    close(tty_fd);

	return 0;

}
