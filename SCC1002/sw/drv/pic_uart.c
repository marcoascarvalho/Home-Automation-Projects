/*************************************************************************
 *
 *    Home Automation Project
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : pic_uart.c
 *    Description : Define API for UART
 *
 *    History :
 *    1. Data: Feb 8, 2011
 *       Author: Marco Carvalho
 *       Description: Create the basic functions
 *
 *
 *    $Revision: 1.0 $
 **************************************************************************/
#include "pic_uart.h"

#define RX_BUFFER_SIZE 32

//volatile BYTE rx_buffer[RX_BUFFER_SIZE];
BYTE rx_buffer[RX_BUFFER_SIZE];
BYTE rx_next_in = 0;
BYTE rx_next_out = 0;

BOOL char_received = FALSE;

//#define bkbhit (rx_next_in!=rx_next_out)

/* Interrupcao de recepção */
#int_RDA
void UART_RX_isr()
{
   int t;

   rx_buffer[rx_next_in]=getc();
   t=rx_next_in;
   rx_next_in=(rx_next_in+1) % RX_BUFFER_SIZE;
   if(rx_next_in==rx_next_out)
   {
      rx_next_in=t;           // Buffer full !!
   }
   
   /* Flag to identify a char received by UART */
   char_received = TRUE;
}



/*************************************************************************
 * Function Name: UART_GetChar
 * Parameters: None
 *
 * Return: char
 *
 * Description: Receive a character from Uart.
 *
 *************************************************************************/
BYTE UART_GetChar()
{

   BYTE c;

   //while(!bkbhit) ;
   c=rx_buffer[rx_next_out];
   rx_next_out=(rx_next_out+1) % RX_BUFFER_SIZE;
   
   /* Clear char_received Flag */
   char_received = FALSE;
   
   return(c);

}

/*************************************************************************
 * Function Name: UART_GetCharAtIndex
 * Parameters:  USHORT Index
 * assumes there are at least index + 1 chars standing in queue
 * Return:  the char
 * Description: Tells the application the n-th char received, but does not consume it
 *
 *************************************************************************/
UCHAR UART_GetCharAtIndex (USHORT index)
{
   
    USHORT BuffPos;
    
    BuffPos = index + rx_next_out;
    
    //if (index >= rx_next_in)
    //{
        //ASSERT(index > rx_next_in);
    //    return 0;
    //}
    
    if (BuffPos > (RX_BUFFER_SIZE-1))
    {
        return rx_buffer[BuffPos - RX_BUFFER_SIZE];
    }
    else
    {
        return rx_buffer[BuffPos];
        //return rx_buffer[index];
    }
}

/*************************************************************************
 * Function Name: UART_GetCharsInBuffer
 * Parameters:  char *ch
 * Return:  0 -  Rx FIFO is empty
 *          n -  number of valid chars received
 * Description: How many chars are there waiting for us ?
 *
 *************************************************************************/
USHORT UART_GetCharsInBuffer()
{
     //return (USHORT) rx_buffer[rx_next_in];
     if(rx_next_in >= rx_next_out)
     {     
         return (rx_next_in-rx_next_out);
     }
     else
     {
         return (RX_BUFFER_SIZE - rx_next_out + rx_next_in);
     }
}
