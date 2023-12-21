/*************************************************************************
 *
 *    Home Automation Protocol
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : haprot.h
 *    Description : Header file for haprot.c
 *
 *    History :
 *    1. Data        : Feb 8, 2011
 *       Author      : Marco Carvalho
 *       Description : The HA Protocol Structure is based on the following
 *                     command format:
 *                     SOH + CMD + SIZE + CONTENT + CHKSUM
 *       
 *                     If SIZE=0 --> CONTENT = NULL (ie. Do not exist)
 *
 *                     CHK = xor (SOH + CMD + SIZE + CONTENT) 
 *
 *    $Revision: 1.0 $
**************************************************************************/

#define HA_ASCII_SOH         (0x55)

/*Message field position */
#define HA_POS_SOH              (0)  //Header position
#define HA_POS_COMMAND          (1)  //Command position 
#define HA_POS_SIZE             (2)  //Size position

/*Message byte quantity */ 
#define HA_HEADER_BYTES      (3)
#define HA_TRAILER_BYTES     (1)
#define HA_CKS_BYTES         (1)
#define HA_EXTRA_BYTES       (HA_HEADER_BYTES + HA_TRAILER_BYTES)
#define HA_MSG_SZ_MIN        (HA_EXTRA_BYTES)
#define MSG_MIN_SZ           (HA_HEADER_BYTES + HA_TRAILER_BYTES + HA_CKS_BYTES)


typedef enum
{
    HA_SET_UNIT                 = 0x30, //ASCII 0
    HA_GET_UNIT                 = 0x31, //ASCII 1
    HA_LIGHT_DIM                = 0x32, //ASCII 2
    HA_PROG_UNIT                = 0x33, //ASCII 3
    HA_SET_DATETIME             = 0x34, //ASCII 4
    HA_GET_DATETIME             = 0x35, //ASCII 5
    HA_SET_HOUSE_ADDR           = 0x36, //ASCII 6
    HA_GET_HOUSE_ADDR           = 0x37, //ASCII 7
    HA_SET_UNIT_ADDR            = 0x38, //ASCII 8
    HA_GET_UNIT_ADDR            = 0x39, //ASCII 9
    HA_SET_LIGHT_SENSOR         = 0x3A, //ASCII :
    HA_GET_LIGHT_SENSOR         = 0x3B, //ASCII ;
	HA_GET_TEMPERATURE_VALUE    = 0x3C  //ASCII <
} CmdHAProt_t;


/* Function Prototypes */
//void DecodeHACommand();

/*
USHORT GetMessageLength();
UCHAR GetElement(USHORT aIdx);
UCHAR IncrCommandStart();
BOOL HACommandIsNotEmpty();
BOOL HACommandIsNotValid();
USHORT GetCommandSize();
BOOL MessageIsComplete();
BOOL CheckSum();
VOID DecodeHACommand();
VOID ProcessHACommand(CmdHAProt_t aCommand, USHORT aSizeMessage);
*/

