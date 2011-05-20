/*
 * Copyright (C) 2006, Brian Crabtree and Joe Lake, monome.org
 *
 * This file is used the copied code from monome/40h/40h.c.
 *
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
 * picnome.h,v.1.2.02 2011/05/20
 */

//sy #define ONE_TWENTY_EIGHT
//dora #define MULTI_PLEXER

#include "usb.h"
#include "usb_function_cdc.h"
#include "HardwareProfile.h"

_CONFIG1(WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx3 & GWRP_OFF & GCP_OFF & JTAGEN_OFF)
_CONFIG2(POSCMOD_HS & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_PRIPLL & PLL96MHZ_ON & PLLDIV_DIV5 & IESO_OFF)
_CONFIG3(WPFP_WPFP0 & SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM)
_CONFIG4(DSWDTPS_DSWDTPS0 & DSWDTOSC_LPRC & RTCOSC_LPRC & DSBOREN_OFF & DSWDTEN_OFF)

#define	CLOCK 32 // 32MHz

#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "stdio.h"

// Global Variables
BYTE i, j, k, p, q;
char *ch;
BOOL flag = FALSE;
BYTE msg_index = 0, msg_index2 = 0;
#ifndef ONE_TWENTY_EIGHT//for sixty four
BYTE sendmsg[64];
#else
BYTE sendmsg[128];
#endif
BYTE sendmsg2[18];
BYTE x, y, state;
WORD spibits = 0x0000;
#ifdef ONE_TWENTY_EIGHT
BYTE lsb0, lsb1;
#endif

char string[15];
char space[] = " ";

//Button Settings
BYTE start_row = 0;
WORD btnCurrent[8], btnLast[8], btnState[8];
#ifndef ONE_TWENTY_EIGHT//for sixty four
WORD btnDebounceCount[8][8];
#else
WORD btnDebounceCount[8][16];
#endif

void buttonInit(void);
BOOL buttonCheck(int row, int index);
void sendOscMsgPress(void);

//A/D Conversion Settings
#ifdef MULTI_PLEXER
  #define NUM_ADC_PINS 11//max = 11
  #define NUM_MP_PINS 8//max = 8
  #define ADC_CHK_NUM 8//sy 4
#else
  #define NUM_ADC_PINS 6//max = 6
  #define ADC_CHK_NUM 8//sy 4
#endif


#ifdef MULTI_PLEXER
  WORD gAdcEnableState = 0;
#else
  BYTE gAdcEnableState = 0;
#endif
BOOL enableAdcFlag = FALSE;
BYTE countChk = 0, enableAdcNum = 0;
BOOL adcSendFlag[NUM_ADC_PINS];
WORD anlg1[NUM_ADC_PINS];
WORD anlg[ADC_CHK_NUM][NUM_ADC_PINS];
WORD anlg0[NUM_ADC_PINS];

//MULTI_PLEXER
#ifdef MULTI_PLEXER
  BYTE scanID;
  BYTE scanCount;
#endif

void enableAdc(int port);
void disableAdc(int port);
void sendOscMsgAdc(void);

//OSC Messages Receive Setting
#ifndef ONE_TWENTY_EIGHT//for sixty four
BYTE led_data[8];
#else//for one twenty eight
WORD led_data[8];
#endif//sy
BOOL firstRun = TRUE;

//EEPROM
struct{
  int intensity;
}para;

void initLedDriver(void);
#ifndef ONE_TWENTY_EIGHT//for sixty four
void sendSpiLED(BYTE msb, BYTE lsb);
#else//for one twenty eight
void sendSpiLED2(BYTE msb0, BYTE lsb0, BYTE msb1, BYTE lsb1);
#endif//sy

void receiveOscMsgs(void);

//sub function
void delayUs(WORD usec);
void delayMs(WORD msec);
int my_atoi(char s);

