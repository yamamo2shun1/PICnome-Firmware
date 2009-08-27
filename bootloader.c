/*
 * Copylight (C) 2009, Shunichi Yamamoto, tkrworks.net
 *
 * This file is part of PICnome.
 *
 * PICnome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option ) any later version.
 *
 * PICnome is distributed in the hope that it will be useful,
 * but WITHIOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PICnome. if not, see <http:/www.gnu.org/licenses/>.
 *
 * bootloader.c,v.1.0 2009/06/24
 */

#include <18F2550.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES WDT128                	//Watch Dog Timer uses 1:128 Postscale
#FUSES HSPLL                 	//High Speed Crystal/Resonator with PLL enabled
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES BORV20                	//Brownout reset at 2.0V
#FUSES NOPUT                 	//No Power Up Timer
#FUSES NOCPD                 	//No EE protection
#FUSES NOSTVREN              	//Stack full/underflow will not cause reset
#FUSES NODEBUG               	//No Debug mode for ICD
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                 	//Program memory not write protected
#FUSES NOWRTD                	//Data EEPROM not write protected
#FUSES NOIESO                	//Internal External Switch Over mode disabled
#FUSES NOFCMEN               	//Fail-safe clock monitor disabled
#FUSES NOPBADEN              	//PORTB pins are configured as digital I/O on RESET
#FUSES NOWRTC                	//configuration not registers write protected
#FUSES NOWRTB                	//Boot block not write protected
#FUSES NOEBTR                	//Memory not protected from table reads
#FUSES NOEBTRB               	//Boot block not protected from table reads
#FUSES NOCPB                 	//No Boot Block code protection
#FUSES LPT1OSC               	//Timer1 configured for low-power operation
#FUSES NOMCLR                	//Master Clear pin used for I/O
#FUSES NOXINST               	//Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PLL5                  	//Divide By 5(20MHz oscillator input)
#FUSES USBDIV                	//USB clock source comes from PLL divide by 2
#FUSES CPUDIV1               	//No System Clock Postscaler
#FUSES VREGEN                	//USB voltage regulator enabled

#USE DELAY(CLOCK=48MHZ, CRYSTAL=20MHZ)

/*
 Configure, then load the bootloader definitions
*/
#define _bootloader
#include <usb_bootloader.h>
//sy #include "usb_bootloader.h"

#define LOADER_ISR 0x28
#build(interrupt=LOADER_ISR)

#include "usb_cdc.h"

//MAX7219CNG Setting
#define LDD_LOAD PIN_B5

void initLedDriver(void);
void sendSpiLED(int msb, int lsb);

/*
 Goto the interrupt vector of the application.
*/
#org 0x08,0x17
void high_isr(void) 
{
   if (bit_test(g_InBootloader,0))
   {
    #ASM
     goto LOADER_ISR
    #ENDASM
   }
   else
   {
    #ASM
     goto APPLICATION_ISR
    #ENDASM
   }
}

#org 0x18,0x27
void low_isr(void) 
{
   if (bit_test(g_InBootloader,0))
   {
    #ASM
     goto LOADER_ISR+0x10
    #ENDASM
   }
   else
   {
    #ASM
     goto APPLICATION_ISR+0x10
    #ENDASM
   }
}

//Write to Flash ROM.
//
// location - flash program memory address
// src - pointer to data to write
// size - number of bytes to write to flash
//
// Here is the sequence of events:
//   1.) Goes to the beginning of the first erase block for this address
//   2.) Reads n records to ram, where n is the PIC's flash erase size
//   3.) Erases block in flash
//   4.) Modifies block in RAM
//   5.) Writes changed block back to FLASH.  Writes in chunks defined by PIC's flash write size
//   6.) Goes back to step1 if there is still more data to be written
void rom_w(int32 location, char *src, int16 size)
{
   #define EEPROM_ERASE_SIZE  getenv("FLASH_ERASE_SIZE")
   #define EEPROM_WRITE_SIZE  getenv("FLASH_WRITE_SIZE")
   int8 block[EEPROM_ERASE_SIZE];
   int32 block_start;
   int8 i;
   int8 num;

   block_start = location & (~((int32)EEPROM_ERASE_SIZE-1));
   i=location-block_start;

   while (size) 
   {
      read_program_memory(block_start, block, sizeof(block));  //read entire block to ram buffer

      if (size>(EEPROM_ERASE_SIZE-i)) {num=EEPROM_ERASE_SIZE-i;} else {num=size;}

      memcpy(&block[i],src,num);    //modify ram buffer

      erase_program_eeprom(block_start);     //erase entire block

      write_program_memory(block_start, block, sizeof(block));    //write modified block

      src+=num;
      block_start+=EEPROM_ERASE_SIZE;
      i=0;
      size-=num;
   }
}

#define BUFFER_LEN_LOD 64

#define ACKLOD 0x06
#define XON    0x11
#define XOFF   0x13

// Convert two hex characters to a int8
unsigned int8 atoi_b16(char *s) 
{  
   char c;
   unsigned int8 result = 0;
   int i;

   for (i=0; i<2; i++,s++)  
   {
      c = *s;
      if (c >= 'A')
         result = 16*result + c - 'A' + 10;
      else
         result = 16*result + c - '0';         
   }

   return(result);
}

void load_program(void)
{
  int1  do_ACKLOD, done=FALSE;
  int8  checksum, line_type;
  int16 l_addr,h_addr=0;
  int8 to;
  int32 addr;
  int8  dataidx, i, count;
  int8  data[32];
  int  buffidx;
  char buffer[BUFFER_LEN_LOD];
   
  while (!done)  // Loop until the entire program is downloaded
  {
    usb_task();

    if(!usb_cdc_kbhit())
      continue;
         
    buffidx = 0;  // Read into the buffer until 0x0D ('\r') is received or the buffer is full
    to = 250;   //250 milliseconds
    do 
    {
      if(!usb_cdc_kbhit())
      {
	delay_ms(1);
	to--;
	if(!to)
	  break;
      }
      else
	to = 250;
      i = usb_cdc_getc();
      buffer[buffidx++] = i;
    }while((i != 0x0D) && (i != 0x0A) && (buffidx <= BUFFER_LEN_LOD));
            
    if(!to)
      continue;

    usb_cdc_putc(XOFF);  // Suspend sender

    do_ACKLOD = TRUE;

    // Only process data blocks that start with ':'
    if(buffer[0] == ':')
    {
      count = atoi_b16 (&buffer[1]);  // Get the number of bytes from the buffer

      // Get the lower 16 bits of address
      l_addr = make16(atoi_b16(&buffer[3]),atoi_b16(&buffer[5]));

      line_type = atoi_b16 (&buffer[7]);

      addr = make32(h_addr,l_addr);

      // If the line type is 1, then data is done being sent
      if(line_type == 1) 
      {
	done = TRUE;
      }
      else if((addr >= (int32)APPLICATION_START) && (addr < ((int32)0x300000)))
      {
	checksum = 0;  // Sum the bytes to find the check sum value
	for(i=1; i<(buffidx-3); i+=2)
	  checksum += atoi_b16 (&buffer[i]);
	checksum = 0xFF - checksum + 1;

	if(checksum != atoi_b16 (&buffer[buffidx-3]))
	  do_ACKLOD = FALSE;
	else   
	{
	  if(line_type == 0) {
	    // Loops through all of the data and stores it in data
	    // The last 2 bytes are the check sum, hence buffidx-3
	    for(i = 9,dataidx=0; i < buffidx-3; i += 2)
	      data[dataidx++]=atoi_b16(&buffer[i]);
                    
	    rom_w(addr, data, count);
	  }
	  else if(line_type == 4)
	    h_addr = make16(atoi_b16(&buffer[9]), atoi_b16(&buffer[11]));
	}
      }
    }

    if(do_ACKLOD)
      usb_cdc_putc (ACKLOD);

    usb_cdc_putc(XON);
  }

  usb_cdc_putc(ACKLOD);
  usb_cdc_putc(XON);
  delay_ms(2000);   //give time for packet to flush
  reset_cpu();
}

void main(void) 
{
  //we use PIN_C2 as an event to determine if we should start the USB CDC
  //bootloader.  if it is not low (button is not pressed) then goto the 
  //application, else if is low (button is pressed) then do the bootloader.
  output_bit(PIN_C2, 1);

  if(!input(PIN_C2))
  {
    setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_16);

    //Max7219 Initialized
    initLedDriver();

    g_InBootloader = TRUE;
    usb_cdc_init();
    usb_init();
    while(!usb_enumerated());
    load_program();
  }

  g_InBootloader = FALSE;
#ASM
  goto APPLICATION_START
#ENDASM
}

/***********************************/
/*                                 */
/*    Functions for Max7219CNG     */
/*                                 */
/***********************************/
void initLedDriver(void)
{
  int i;

  sendSpiLED(0x0B, 0x07);         // Scan Limit full range
  //sy sendSpiLED(0x0A, 0x04);         // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(0x0A, 0x0F);         // Max Intensity 0x00[min] - 0x0F[max]

  // print startup pattern
  sendSpiLED(1, 0x00);
  sendSpiLED(2, 0x3F);
  sendSpiLED(3, 0x46);
  sendSpiLED(4, 0x46);
  sendSpiLED(5, 0x3E);
  sendSpiLED(6, 0x46);
  sendSpiLED(7, 0x46);
  sendSpiLED(8, 0x3F);

  sendSpiLED(0x0C, 0x01);         // Shutdown Normal Operation
  sendSpiLED(0x0F, 0x00);         // Display Test Off
  for(i = 0; i < 64; i++)
  {
    sendSpiLED(10, (64 - i) / 4); // set to max intensity
    delay_ms(24);
  }
  //sy sendSpiLED(0x0A, 0x04);         // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(0x0A, 0x0F);         // Max Intensity 0x00[min] - 0x0F[max]

  for(i = 1; i < 9; i++)
    sendSpiLED(i, 0x00);

}

void sendSpiLED(int msb, int lsb)
{
  output_bit(LDD_LOAD, 0);
  spi_write(msb);
  spi_write(lsb);
  output_bit(LDD_LOAD, 1);
}
