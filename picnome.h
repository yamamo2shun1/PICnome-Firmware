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
 * picnome.h,v.0.92 2009/06/26
 */

#include <18F2550.h>
#device adc=10

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

#include <stdlib.h>
#include <string.h>
#include "usb_cdc.h"

#include <usb_bootloader.h>

//TC74HC164 Setting
#define SR_CLK PIN_B6
#define SR_A   PIN_B7

//TC74HC165 Setting
#define SR_SL   PIN_C1
#define SR_CLK2 PIN_C0
#define SR_QH   PIN_C6

//MAX7219CNG Setting
#define LDD_LOAD PIN_B5

void initLedDriver(void);
void sendSpiLED(int msb, int lsb);

//OSC Messages Receive Setting
int led_data[8];
int firstRun = TRUE;

char space[] = " ";
char l[]   = "led";
char lc[]  = "led_col";
char lr[]  = "led_row";
char ae[]  = "adc_enable";
char p[]   = "pwm";
char out[] = "output";
char it[]  = "intensity";
char t[]   = "test";
char s[]   = "shutdown";
char r[]   = "report";

void receiveOscMsgs(void);

//Button Settings
int btnCurrent[8], btnLast[8], btnState[8], btnDebounceCount[8][8];

void buttonInit(void);
short buttonCheck(int row, int index);
void sendOscMsgPress(void);


//A/D Conversion Settings
#define kAdcFilterNumAdcs 7

const int adc_id[] = {0, 1, 2, 3, 4, 8, 9};

int gAdcEnableState = 0;
short enableAdcFlag = FALSE;
int countAdc = 0, loopAdc = 0;

void enableAdc(int port);
void disableAdc(int port);
void sendOscMsgAdc(void);

//EEPROM
struct{
  int intensity;
}para;

void initAndSaveParaToEeprom(void);
void getParaFromEeprom(int *ptr, int num, int addr);
void putParaToEeprom(int *ptr, int num, int addr);
