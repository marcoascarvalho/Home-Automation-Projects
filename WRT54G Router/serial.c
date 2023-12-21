#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#define UART "/dev/ttyS1"

struct termios tio;

unsigned char c='D';

int tty_fd;

static void setup_tty(char *tty_file)
{

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
    tty_fd = open(tty_file, O_RDWR | O_NONBLOCK); // make the reads non-blocking
	if ( tty_fd == -1 )
	{
		printf("Open %s port failed", tty_file);
		//rc = tty_fd;
		exit(-1);
	}

	/* Setup baudrate for input and output */
    cfsetospeed(&tio, B115200);
    cfsetispeed(&tio, B115200);


    /* now clean the modem line and activate the settings for the port */
    tcflush(tty_fd, TCIFLUSH);
    tcsetattr(tty_fd, TCSANOW, &tio);

    printf("%s: open and configured: successful\n", tty_file);

}

int main(int argc, char **argv)
{

	/* Our file descriptor */
	//int fd;
	//int rc = 0;
	//char *rd_buf[16];
	//const char wr_buf[16] = "testando serial\n";
	//char *wr_buf;

	setup_tty(UART);

	while (1)
    {
            if (read(tty_fd,&c,1)>0)
	{
		printf("Recebido %s \n",&c);
		write(tty_fd,&c,1);
	}
    }

    close(tty_fd);

	/* Issue a read */
	//rc = read(fd, rd_buf, 0);
	
	//if ( rc == -1 )
	//{
	//	perror("read failed");
	//	close(fd);
	//	exit(-1);
	//}

	//printf("%s: read: returning %d bytes!\n", argv[0], rc);

	/* Print data */
	//wr_buf = "testando serial\n";
	
	//rc = write(fd, wr_buf, strlen(wr_buf));

	//if ( rc == -1 )
	//{
	//	perror("write failed");
	//	close(fd);
	//	exit(-1);
	//}

	//printf("%s: write: writing to serial port!\n", argv[0]);

	//close(fd);
	//return 0;

}
