/*************************************************************************
 *
 *    Home Automation Protocol - main function
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : main.c
 *    Description : Define a main function to test HA Protocol 
 *                  Serial Port (UART)
 *    History :
 *    1. Data        : Feb 8, 2011
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *
 *    $Revision: 1.0 $
**************************************************************************/

#include "16F877.h"

#device ICD=TRUE

#use delay(clock=20000000)
#fuses NOWDT,HS, PUT, NOPROTECT, BROWNOUT, NOLVP, NOCPD, NOWRT, NODEBUG
#use rs232(baud=115200, xmit=TX, rcv=RX)

#define BOOL short int
#define UCHAR unsigned char
#define USHORT unsigned char

#include "haprot.c"

/*;----------------------------------------------------------------------
; Main Routine
;----------------------------------------------------------------------*/
void main()
{
  
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(FALSE);
   setup_counters(RTCC_INTERNAL,RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);

   /* enable TX IRQ */
   //enable_interrupts(INT_TBE);
   
   /* enable RX IRQ */
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   
   printf("Iniciar Teste\n");
   
   while (1)
   {

      DecodeHACommand();
      
   }

}
