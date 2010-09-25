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
 * picnome.c,v.1.1.03 2010/09/08
 */

#include "picnome.h"

#pragma code

int main(void)
{
  CLKDIV = 0;
  AD1PCFGL = 0xFFC0;//AN0,AN1,AN2,AN3,AN4,AN5
  TRISA = 0x0003;
  TRISB = 0x00AF;
  ODCBbits.ODB9 = 1;
  ODCBbits.ODB4 = 1;
  ODCAbits.ODA4 = 1;
  ODCBbits.ODB13 = 1;
  ODCBbits.ODB14 = 1;

  WORD pll_startup_counter = 600;
  CLKDIVbits.PLLEN = 1;
  while(pll_startup_counter--);

  RPOR3bits.RP7R = 7;// SDO PIN_B7
  RPOR4bits.RP8R = 8;// SCK PIN_B8

  //sy SPI1CON1 = 0x053f;//sy 16mhz   /* Initalizes the spi module */
  SPI1CON1 = 0x053b;//sy 8mhz   /* Initalizes the spi module */
  //sy SPI1CON1 = 0x053e;//sy 4mhz   /* Initalizes the spi module */
  //sy SPI1CON1 = 0x053a;//sy 2mhz   /* Initalizes the spi module */
  //sy SPI1CON1 = 0x0536;//sy 1mhz   /* Initalizes the spi module */
  SPI1CON2 = 0x0000;
  SPI1STATbits.SPIEN = 1;

  IFS0bits.SPI1IF = 0; /* Clear IF bit */
  IPC2bits.SPI1IP = 0; /* Assign interrupt priority */
  IEC0bits.SPI1IE = 0; /* Interrupt Enable/Disable bit */

  // A/D Conversion Interrupt Intialize
  PR3 = 16;//sy 311; // 5msec
  T3CON = 0b1000000000110000;
  AD1CON1 = 0x8044;
  AD1CON2 = 0x0414;
  AD1CON3 = 0x1F02;//sy 0x1F05;
  AD1CHS =  0x0000;
  AD1PCFG = 0xFFC0;
  AD1CSSL = 0x003F;
  IEC0bits.AD1IE = 1;
  countAdc = 0;
  for(i = 0; i < 6; i++)
  {
    adcSendFlag[i] = FALSE;
    anlg_avg[i] = 0;
  }

  delayMs(100);

  SR_A = 1;
  for(i = 0; i < 8; i++)
  {
    led_data[i] = 0;
    SR_CLK = 1;
    SR_CLK = 0;
  }
  buttonInit();
  initLedDriver();

  USBDeviceInit();

#if defined(USB_INTERRUPT)
  USBDeviceAttach();
#endif

  i = 1, j = 1;
  while(1)
  {
    if((USBDeviceState >= CONFIGURED_STATE) && (USBSuspendControl == 0))
    {
      sendOscMsgPress();
      sendOscMsgAdc();
      receiveOscMsgs();
      CDCTxService();
    }
  }
}

/***********************************/
/*                                 */
/*    Functions for Max7219CNG     */
/*                                 */
/***********************************/
#ifndef ONE_TWENTY_EIGHT//for sixty four
void initLedDriver(void)
{
  sendSpiLED(0x0B, 0x07);         // Scan Limit full range
  sendSpiLED(0x0C, 0x01);         // Shutdown Normal Operation
  sendSpiLED(0x0F, 0x00);         // Display Test Off

 // print startup pattern 
  sendSpiLED(0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(1, 0);
  sendSpiLED(2, 0);
  sendSpiLED(3, 0);
  sendSpiLED(4, 24);
  sendSpiLED(5, 24);
  sendSpiLED(6, 0);
  sendSpiLED(7, 0);
  sendSpiLED(8, 0);
  for(i = 0; i < 64; i++)
  {
    sendSpiLED(0x0A, (64 - i) / 4);   // set to max intensity
    delayMs(4);
  }
  sendSpiLED(0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(1, 0);
  sendSpiLED(2, 0);
  sendSpiLED(3, 24);
  sendSpiLED(4, 36);
  sendSpiLED(5, 36);
  sendSpiLED(6, 24);
  sendSpiLED(7, 0);
  sendSpiLED(8, 0);
  for(i = 0; i < 64; i++)
  {
    sendSpiLED(0x0A, (64 - i) / 4);   // set to max intensity
    delayMs(4);
  }
  sendSpiLED(0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(1, 0);
  sendSpiLED(2, 24);
  sendSpiLED(3, 36);
  sendSpiLED(4, 66);
  sendSpiLED(5, 66);
  sendSpiLED(6, 36);
  sendSpiLED(7, 24);
  sendSpiLED(8, 0);
  for(i = 0; i < 64; i++)
  {
    sendSpiLED(0x0A, (64 - i) / 4);   // set to max intensity
    delayMs(4);
  }
  sendSpiLED(0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  for(i = 1; i < 9; i++)
    sendSpiLED(i, 0x00);
}

void sendSpiLED(BYTE msb, BYTE lsb)
{
  spibits = (0xFF00 & ((0x00FF & (WORD)msb) << 8)) + (0x00FF & (WORD)lsb);
  LDD_LOAD = 0;
  while(SPI1STATbits.SPITBF);
  SPI1BUF = spibits;
  delayUs(1);
  LDD_LOAD = 1;
}
#else//for one twenty eight
void initLedDriver(void)
{
  sendSpiLED2(0x0B, 0x07, 0x0B, 0x07); // Scan Limit full range
  sendSpiLED2(0x0C, 0x01, 0x0C, 0x01); // Shutdown Normal Operation
  sendSpiLED2(0x0F, 0x00, 0x0F, 0x00); // Display Test Off

 // print startup pattern 
  sendSpiLED2(0x0A, 0x0F, 0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED2(1, 0, 1, 0);
  sendSpiLED2(2, 0, 2, 0);
  sendSpiLED2(3, 0, 3, 0);
  sendSpiLED2(4, 24, 4, 0);
  sendSpiLED2(5, 24, 5, 0);
  sendSpiLED2(6, 0, 6, 0);
  sendSpiLED2(7, 0, 7, 0);
  sendSpiLED2(8, 0, 8, 0);
  for(i = 0; i < 64; i++)
  {
    sendSpiLED2(0x0A, (64 - i) / 4, 0x0A, (64 - i) / 4);   // set to max intensity
    delayMs(4);
  }
  sendSpiLED2(0x0A, 0x0F, 0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED2(1, 0, 1, 0);
  sendSpiLED2(2, 0, 2, 0);
  sendSpiLED2(3, 24, 3, 0);
  sendSpiLED2(4, 36, 4, 0);
  sendSpiLED2(5, 36, 5, 0);
  sendSpiLED2(6, 24, 6, 0);
  sendSpiLED2(7, 0, 7, 0);
  sendSpiLED2(8, 0, 8, 0);
  for(i = 0; i < 64; i++)
  {
    sendSpiLED2(0x0A, (64 - i) / 4, 0x0A, (64 - i) / 4);   // set to max intensity
    delayMs(4);
  }
  sendSpiLED2(0x0A, 0x0F, 0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED2(1, 0, 1, 0);
  sendSpiLED2(2, 24, 2, 0);
  sendSpiLED2(3, 36, 3, 0);
  sendSpiLED2(4, 66, 4, 0);
  sendSpiLED2(5, 66, 5, 0);
  sendSpiLED2(6, 36, 6, 0);
  sendSpiLED2(7, 24, 7, 0);
  sendSpiLED2(8, 0, 8, 0);
  for(i = 0; i < 64; i++)
  {
    sendSpiLED2(0x0A, (64 - i) / 4, 0x0A, (64 - i) / 4);   // set to max intensity
    delayMs(4);
  }

  sendSpiLED2(0x0A, 0x0F, 0x0A, 0x0F); // Max Intensity 0x00[min] - 0x0F[max]

  for(i = 1; i < 9; i++)
  {
    sendSpiLED2(i, 0x00, i, 0x00);
  }
}

void sendSpiLED2(BYTE msb0, BYTE lsb0, BYTE msb1, BYTE lsb1)
{
  WORD test;
  LDD_LOAD = 0;
  test = (msb1 << 8) + lsb1;
  SPI1BUF = test;
  while(SPI1STATbits.SPITBF);
  test = (msb0 << 8) + lsb0;
  SPI1BUF = test;
  while(SPI1STATbits.SPITBF);
  delayUs(1);
  LDD_LOAD = 1;
}
#endif//sy

/***********************************/
/*                                 */
/*    Function for OSC Msgs Rcv    */
/*                                 */
/***********************************/
void receiveOscMsgs(void)
{
  if(getsUSBUSART(string, 15))
  {
    if(string[0] == 'l' && (string[1] == '0' || string[1] == '1')) // led
    {
      y = my_atoi(string[3]);
      if(y >= 8)
        return;
      x = my_atoi(string[2]);
      state = my_atoi(string[1]);

      if(state)
        led_data[y] |= (1 << x);
      else
        led_data[y] &= ~(1 << x);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(y + 1, led_data[y]);
#else//for one twenty eight
      lsb0 = led_data[y] & 0x00FF;
      lsb1 = (led_data[y] & 0xFF00) >> 8;
      sendSpiLED2(y + 1, lsb0, y + 1, lsb1);
#endif
    }
    else if(string[0] == 'l' && string[1] == 'c') // led_col
    {
      BYTE column, data;
      ch = strtok(string, space);
      ch = strtok(0, space);
      column = atoi(ch);
      ch = strtok(0, space);
      data = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      if(firstRun == TRUE)
      {
        for(i = 0; i < 8; i++)
        {
          led_data[i] = 0;
          sendSpiLED(i + 1, led_data[i]);
        }
        firstRun = FALSE;
      }
      for(i = 0; i < 8; i++)
      {
        if(data & (1 << i))
          led_data[i] |= 1 << column;
        else
          led_data[i] &= ~(1 << column);
        
        sendSpiLED(i + 1, led_data[i]);
      }
#else//for one twenty eight
      if(firstRun == TRUE)
      {
        for(i = 0; i < 8; i++)
        {
          led_data[i] = 0;
          lsb0 = (BYTE)(led_data[i] & 0x00FF);
          lsb1 = (BYTE)((led_data[i] & 0xFF00) >> 8);
          sendSpiLED2(i + 1, lsb0, i + 1, lsb1);
        }
        firstRun = FALSE;
      }
      for(i = 0; i < 8; i++)
      {
        if(data & (1 << i))
          led_data[i] |= (0x0001 << column);
        else
          led_data[i] &= ~(0x0001 << column);
        
        lsb0 = (BYTE)(led_data[i] & 0x00FF);
        lsb1 = (BYTE)((led_data[i] >> 8) & 0x00FF);
        sendSpiLED2(i + 1, lsb0, i + 1, lsb1);
      }
#endif//sy
    }
    else if(string[0] == 'l' && string[1] == 'r') // led_row
    {
#ifndef ONE_TWENTY_EIGHT//for sixty four
      BYTE row, data;
      ch = strtok(string, space);
      ch = strtok(0, space);
      row = atoi(ch);
      ch = strtok(0, space);
      data = atoi(ch);
      
      if(firstRun == TRUE)
      {
        for(i = 0; i < 8; i++)
        {
          led_data[i] = 0;
          sendSpiLED(i + 1, led_data[i]);
        }
        firstRun = FALSE;
      }
      led_data[row] = data;
      sendSpiLED(row + 1, led_data[row]);
#else//for one twenty eight
      BYTE row; 
      WORD data;
      ch = strtok(string, space);
      ch = strtok(0, space);
      row = atoi(ch);
      ch = strtok(0, space);
      data = atoi(ch);
      
      if(firstRun == TRUE)
      {
        for(i = 0; i < 8; i++)
        {
          led_data[i] = 0;
          lsb0 = (BYTE)(led_data[i] & 0x00FF);
          lsb1 = (BYTE)((led_data[i] & 0xFF00) >> 8);
          sendSpiLED2(i + 1, lsb0, i + 1, lsb1);
        }
        firstRun = FALSE;
      }
      led_data[row] = data;
      lsb0 = (BYTE)(led_data[row] & 0x00FF);
      lsb1 = (BYTE)(led_data[row] >> 8);
      sendSpiLED2(row + 1, lsb0, row + 1, lsb1);
#endif//sy
    }
    else if(string[0] == 'a' && string[1] == 'e') // adc_enable
    {
      BYTE pin, state;
      ch = strtok(string, space);
      ch = strtok(0, space);
      pin = atoi(ch);
      ch = strtok(0, space);
      state = atoi(ch);
      if(state)
        enableAdc(pin);
      else
        disableAdc(pin);
    }
    else if(string[0] == 'i') // intensity
    {
      ch = strtok(string, space);
      ch = strtok(0, space);
      para.intensity = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(0x0A, para.intensity);
#else//for one twenty eight
      sendSpiLED2(0x0A, para.intensity, 0x0A, para.intensity);
#endif
      //sy putParaToEeprom(&para, sizeof(para), 0);
    }
    else if(string[0] == 't') // test
    {
      BYTE state;
      ch = strtok(string, space);
      ch = strtok(0, space);
      state = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(15, state);
#else//for one twenty eight
      sendSpiLED2(15, state, 15, state);
#endif//sy
    }
    else if(string[0] == 's') // shutdown
    {
      BYTE state;
      ch = strtok(string, space);
      ch = strtok(0, space);
      state = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(12, state);
#else//for one twenty eight
      sendSpiLED2(12, state, 12, state);
#endif
    }
    else if(string[0] == 'f') // firmware
    {
      sendmsg[0] = 'f';
      sendmsg[1] = 11;
      sendmsg[2] = 3;
      if(mUSBUSARTIsTxTrfReady())
        mUSBUSARTTxRam(sendmsg, 3);
      CDCTxService();
      delayUs(2);
    }
  }
}

/***********************************/
/*                                 */
/*  Functions for Button Handling  */
/*                                 */
/***********************************/
void buttonInit(void)
{
  for(i = 0; i < 8; i++)
  {
    btnCurrent[i] = 0x00;
    btnLast[i] = 0x00;
    btnState[i] = 0x00;

#ifndef ONE_TWENTY_EIGHT//for sixty four
    for(j = 0; j < 8; j++)
#else//for one twenty eight
    for(j = 0; j < 16; j++)
#endif
      btnDebounceCount[i][j] = 0;
  }
}

BOOL buttonCheck(int row, int index)
{
  flag = FALSE;

  if(((btnCurrent[row] ^ btnLast[row]) & (1 << index)) && ((btnCurrent[row] ^ btnState[row]) & (1 << index)))
    btnDebounceCount[row][index] = 0;
  else if (((btnCurrent[row] ^ btnLast[row]) & (1 << index)) == 0 && ((btnCurrent[row] ^ btnState[row]) & (1 << index)))
  {
    if(btnDebounceCount[row][index] < 4 && ++btnDebounceCount[row][index] == 4)
    {
      //debug sendSpiLED2(row + 1, (BYTE)(btnCurrent[row] & 0x00FF), row + 1, (BYTE)((btnCurrent[row] >> 8) & 0x00FF));
      if(btnCurrent[row] & (1 << index))
        btnState[row] |= (1 << index);
      else
        btnState[row] &= ~(1 << index);
      flag = TRUE;
    }
  }
  return flag;
}

void sendOscMsgPress(void)
{
  if(start_row == 0)
  {
    SR_A = 0;
    SR_CLK = 1;
    SR_A = 1;
  }

  for(i = start_row; i < start_row + 4; i++)
  {
    SR_CLK = 1;
    for(k = 0; k < 5; k++)
      asm("NOP");
    SR_CLK = 0;

    btnLast[i] = btnCurrent[i];

    SR_SL = 0;
    delayUs(1);
    SR_SL = 1;

#ifndef ONE_TWENTY_EIGHT//for sixty four
    for(j = 0; j < 8; j++)
#else//for one twenty eight
    for(j = 0; j < 16; j++)
#endif
    {
      if(SR_QH)
        btnCurrent[i] &= ~(1 << j);
      else
        btnCurrent[i] |= (1 << j);

      SR_CLK2 = 1;
      for(k = 0; k < 2; k++)
        asm("NOP");
      SR_CLK2 = 0;

      if(buttonCheck(i, j))
      {
        //sy if(btnState[i] & (1 << j))
        if(btnCurrent[i] & (1 << j))
          sendmsg[msg_index] = 'p';
        else
          sendmsg[msg_index] = 'r';
        sendmsg[msg_index + 1] = (i << 4) + j;
        msg_index += 2;
      }
    }
  }

  if(msg_index > 0 && mUSBUSARTIsTxTrfReady())
  {
    mUSBUSARTTxRam(sendmsg, msg_index);
    msg_index = 0;
  }
  CDCTxService();
  delayUs(2);

  start_row += 4;
  if(start_row >= 8)
  {
    start_row = 0;
  }
}

/***********************************/
/*                                 */
/*  Functions for A/D Conversion   */
/*                                 */
/***********************************/
void enableAdc(int port)
{
  if(port >= NUM_ADC_PINS)
    return;

  if((gAdcEnableState & 0x3F) == 0)
    enableAdcFlag = TRUE;

  gAdcEnableState |= (1 << port);
  enableAdcNum++;
}

void disableAdc(int port)
{
  if(port >= NUM_ADC_PINS)
    return;

  gAdcEnableState &= ~(1 << port);

  if((gAdcEnableState & 0x3F) == 0)
    enableAdcFlag = FALSE;
  enableAdcNum--;
}

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void)
{
  IFS0bits.AD1IF = 0;
  anlg_avg[0] = ADC1BUF0;
  anlg_avg[1] = ADC1BUF1;
  anlg_avg[2] = ADC1BUF2;
  anlg_avg[3] = ADC1BUF3;
  anlg_avg[4] = ADC1BUF4;
  anlg_avg[5] = ADC1BUF5;

  for(p = 0; p < NUM_ADC_PINS; p++)
  {
    if((gAdcEnableState & (1 << p)) == (1 << p))
      adcSendFlag[p] = TRUE;
  }
}

void sendOscMsgAdc(void)
{
  for(i = 0; i < 6; i++)
  {
    if((gAdcEnableState & (1 << i)) == (1 << i) && adcSendFlag[i] == TRUE)
    {
      sendmsg2[msg_index2] = 'a';
      sendmsg2[msg_index2 + 1] = (i << 4) + ((anlg_avg[i] & 0x0300) >> 8);
      sendmsg2[msg_index2 + 2] = (anlg_avg[i] & 0x00FF);
      msg_index2 += 3;
      adcSendFlag[i] = FALSE;
    }
  }
  if(msg_index2 > 0 && mUSBUSARTIsTxTrfReady())
  {
    mUSBUSARTTxRam(sendmsg2, msg_index2);
    msg_index2 = 0;
  }
  CDCTxService();
  delayUs(1);
}


void delayUs(WORD usec)
{
  usec = (WORD)(CLOCK * usec) / 10;
  while(usec)
  {
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");					
    usec--;
  }
}

void delayMs(WORD msec)
{
  for(k = 0; k < msec; k++)
    delayUs(1000);
}

int my_atoi(char s)
{
  int result = 0;

  if (s >= '0' && s <= '9')
    result = (result << 4) + (s - '0');
  else
    result = (result << 4) + (s - 'A' + 10);
  return result;
}

// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:
	
	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
	//things to not work as intended.	
	

    #if defined(__C30__)
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        Sleep();
    #endif
    #endif
}

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
#if 0//sy
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.

    //This is reverse logic since the pushbutton is active low
    if(buttonPressed == sw2)
    {
        if(buttonCount != 0)
        {
            buttonCount--;
        }
        else
        {
            //This is reverse logic since the pushbutton is active low
            buttonPressed = !sw2;

            //Wait 100ms before the next press can be generated
            buttonCount = 100;
        }
    }
    else
    {
        if(buttonCount != 0)
        {
            buttonCount--;
        }
    }
#endif//sy
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}

/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckCDCRequest();
}//end

/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end

/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    CDCInitEP();
}

/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }      
    return TRUE; 
}
