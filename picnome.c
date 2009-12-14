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
 * picnome.c,v.1.13 2009/12/14
 */

#include "picnome.h"

void main()
{
  setup_adc_ports(AN0_TO_AN9 || VSS_VDD);
  setup_adc(ADC_CLOCK_DIV_64);
  setup_spi(SPI_MASTER | SPI_L_TO_H | SPI_XMIT_L_TO_H | SPI_CLK_DIV_16);
  setup_wdt(WDT_OFF);
  setup_timer_0(RTCC_INTERNAL);
  setup_timer_1(T1_DISABLED);
  setup_timer_2(T2_DIV_BY_1, 255, 1);// for PWM
  setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
  setup_comparator(NC_NC_NC_NC);
  setup_vref(FALSE);
  setup_ccp1(CCP_PWM);
  set_pwm1_duty(0);
  //Setup_Oscillator parameter not selected from Intr Oscillator Config tab

  //Parameters Initialized
  getParaFromEeprom(&para, sizeof(para), 0);
  if(!(para.intensity > 0))
    initAndSaveParaToEeprom();

  //Button Initialized
  buttonInit();

  output_bit(SR_A, 1);
  for(i = 0; i < 8; i++)
  {
    led_data[i] = 0;
    output_bit(SR_CLK, 1);
    output_bit(SR_CLK, 0);
  }

  //Input Initialized
  inputInit();

  //Max7219 Initialized
  initLedDriver();

  //USB CDC Class Initialized
  usb_cdc_init();
  usb_init();
  while(!usb_cdc_connected()){}

  while(TRUE)
  {
    usb_task();
    //test if(usb_enumerated())
    //test {
      receiveOscMsgs();

      //Button Handling
      sendOscMsgPress();

      //Digital Input Handling
      sendOscMsgInput();

      //Adc Handling
      sendOscMsgAdc();
    //test }
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
  sendSpiLED(0x0B, 0x07);           // Scan Limit full range

  sendSpiLED(0x0A, para.intensity); // Max Intensity 0x00[min] - 0x0F[max]

  for(i = 1; i < 9; i++)
    sendSpiLED(i, i);               // print startup pattern 

  sendSpiLED(0x0C, 0x01);           // Shutdown Normal Operation

  sendSpiLED(0x0F, 0x00);           // Display Test Off

  for(i = 0; i < 64; i++)
  {
    sendSpiLED(0x0A, (64 - i) / 4);   // set to max intensity
    delay_ms(8);
  }

  sendSpiLED(0x0A, para.intensity); // Max Intensity 0x00[min] - 0x0F[max]

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
#else//for one twenty eight
void initLedDriver(void)
{
  sendSpiLED(0, 0x0B, 0x07);           // Scan Limit full range
  sendSpiLED(1, 0x0B, 0x07);           // Scan Limit full range

  sendSpiLED(0, 0x0A, para.intensity); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(1, 0x0A, para.intensity); // Max Intensity 0x00[min] - 0x0F[max]

  for(i = 1; i < 9; i++)
  {
    sendSpiLED(0, i, i);               // print startup pattern 
    sendSpiLED(1, i, i);               // print startup pattern 
  }

  sendSpiLED(0, 0x0C, 0x01);           // Shutdown Normal Operation
  sendSpiLED(1, 0x0C, 0x01);           // Shutdown Normal Operation

  sendSpiLED(0, 0x0F, 0x00);           // Display Test Off
  sendSpiLED(1, 0x0F, 0x00);           // Display Test Off

  for(i = 0; i < 64; i++)
  {
    sendSpiLED(0, 0x0A, (64 - i) / 4);   // set to max intensity
    sendSpiLED(1, 0x0A, (64 - i) / 4);   // set to max intensity
    delay_ms(8);
  }

  sendSpiLED(0, 0x0A, para.intensity); // Max Intensity 0x00[min] - 0x0F[max]
  sendSpiLED(1, 0x0A, para.intensity); // Max Intensity 0x00[min] - 0x0F[max]

  for(i = 1; i < 9; i++)
  {
    sendSpiLED(0, i, 0x00);
    sendSpiLED(1, i, 0x00);
  }
}

void sendSpiLED(int id, int msb, int lsb)
{
  if(id == 0)
  {
    output_bit(LDD_LOAD, 0);
    spi_write(0x00);
    spi_write(0x00);
    spi_write(msb);
    spi_write(lsb);
    output_bit(LDD_LOAD, 1);
    delay_us(5);
  }
  else if(id == 1)
  {
    output_bit(LDD_LOAD, 0);
    spi_write(msb);
    spi_write(lsb);
    spi_write(0x00);
    spi_write(0x00);
    output_bit(LDD_LOAD, 1);
    delay_us(5);
  }
}
#endif//sy

/***********************************/
/*                                 */
/*    Function for OSC Msgs Rcv    */
/*                                 */
/***********************************/
void receiveOscMsgs(void)
{
  if(usb_cdc_kbhit())
  {
    get_string_usb(string, 20);

    ch = strtok(string, space);

    if(!strcmp(ch, l)) // led
    {
      //test int x, y, state;
      ch = strtok(0, space);
      x = atoi(ch);
      ch = strtok(0, space);
      y = atoi(ch);
      ch = strtok(0, space);
      state = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      if(state)
        led_data[y] |= (1 << x);
      else
        led_data[y] &= ~(1 << x);

      sendSpiLED(y + 1, led_data[y]);
#else//for one twenty eight
      if(x < 8)
      {
        int x1, y1;
        x1 = y;
        y1 = 7 - x;
        x = x1;
        y = y1;
      }
      if(state == 0)
        led_data[y] &= ~((long)1 << x);
      else
        led_data[y] |= ((long)1 << x);

      {
        int lsb0, lsb1;
        lsb0 = (int)(led_data[y] & 0x00FF);
        sendSpiLED(0, y + 1, lsb0);
        lsb1 = (int)((led_data[y] & 0xFF00) >> 8);
        sendSpiLED(1, y + 1, lsb1);
      }
#endif
    }
    else if(!strcmp(ch, lc)) // led_col
    {
#ifndef ONE_TWENTY_EIGHT//for sixty four      
      int column, data;
      ch = strtok(0, space);
      column = atoi(ch);
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
      for(i = 0; i < 8; i++)
      {
        if(data & (1 << i))
          led_data[i] |= 1 << column;
        else
          led_data[i] &= ~(1 << column);
        
        sendSpiLED(i + 1, led_data[i]);
      }
#else//for one twenty eight
      int column, data;
      int column1, i1;
      int lsb0, lsb1;
      ch = strtok(0, space);
      column = atoi(ch);
      ch = strtok(0, space);
      data = atoi(ch);

      if(firstRun == TRUE)
      {
        for(i = 0; i < 8; i++)
        {
          led_data[i] = 0;
          lsb0 = (int)(led_data[i] & 0x00FF);
          sendSpiLED(0, i + 1, lsb0);
          lsb1 = (int)((led_data[i] & 0xFF00) >> 8);
          sendSpiLED(1, i + 1, lsb1);
        }
        firstRun = FALSE;
      }
      for(i = 0; i < 8; i++)
      {
        if(column < 8)
        {
          column1 = i;
          i1 = 7 - column;
        }
        else
        {
          column1 = column;
          i1 = i;
        }
        if(data & (1 << i))
          led_data[i1] |= ((long)1 << column1);
        else
          led_data[i1] &= ~((long)1 << column1);
        
        lsb0 = (int)(led_data[i1] & 0x00FF);
        sendSpiLED(0, i1 + 1, lsb0);
        lsb1 = (int)((led_data[i1] & 0xFF00) >> 8);
        sendSpiLED(1, i1 + 1, lsb1);
      }
#endif//sy
    }
    else if(!strcmp(ch, lr)) // led_row
    {
#ifndef ONE_TWENTY_EIGHT//for sixty four
      int row, data;
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
      int row; 
      long data;
      int lsb0, lsb1;
      int row1, i1;
      ch = strtok(0, space);
      row = atoi(ch);
      ch = strtok(0, space);
      data = atol(ch);
      
      if(firstRun == TRUE)
      {
        for(i = 0; i < 8; i++)
        {
          int lsb0, lsb1;
          led_data[i] = 0;
          lsb0 = (int)(led_data[i] & 0x00FF);
          sendSpiLED(0, i + 1, lsb0);
          lsb1 = (int)((led_data[i] & 0xFF00) >> 8);
          sendSpiLED(1, i + 1, lsb1);
        }
        firstRun = FALSE;
      }

      lsb0 = (int)(data & 0x00FF);
      for(i = 0; i < 8; i++)
      {
        i1 = row;
        row1 = 7 - i;
        
        if(lsb0 & (1 << i))
          led_data[row1] |= ((long)1 << i1);
        else
          led_data[row1] &= ~((long)1 << i1);
        
        sendSpiLED(0, row1 + 1, led_data[row1]);
      }
      lsb1 = (int)((data & 0xFF00) >> 8);
      sendSpiLED(1, row + 1, lsb1);
#endif//sy
    }
    else if(!strcmp(ch, ae)) // adc_enable
    {
      int pin, state;
      ch = strtok(0, space);
      pin = atoi(ch);
      ch = strtok(0, space);
      state = atoi(ch);
      if(state)
        enableAdc(pin);
      else
        disableAdc(pin);
    }
    else if(!strcmp(ch, p)) // pwm
    {
      int prescale, period;
      float duty;
      ch = strtok(0, space);
      prescale = atoi(ch);
      ch = strtok(0, space);
      period = atoi(ch);
      ch = strtok(0, space);
      duty = atof(ch);
      if(prescale == 1)
        setup_timer_2(T2_DIV_BY_1, period, 1);
      else if(prescale == 4)
        setup_timer_2(T2_DIV_BY_4, period, 1);
      else if(prescale == 16)
        setup_timer_2(T2_DIV_BY_16, period, 1);
      set_pwm1_duty((long)(1024.0 * duty));
    }
/*sy
    else if(!strcmp(ch, o)) // output
    {
      int pin, state;
      ch = strtok(0, space);
      pin = atoi(ch);
      ch = strtok(0, space);
      state = atoi(ch);
      if(pin == 0)
        output_bit(PIN_B4, state);
      else if(pin == 1)
        output_bit(PIN_B0, state);
    }
*/
    else if(!strcmp(ch, it)) // intensity
    {
      ch = strtok(0, space);
      para.intensity = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(0x0A, para.intensity);
#else//for one twenty eight
      sendSpiLED(0, 0x0A, para.intensity);
      sendSpiLED(1, 0x0A, para.intensity);
#endif
      putParaToEeprom(&para, sizeof(para), 0);
    }
    else if(!strcmp(ch, t)) // test
    {
      int state;
      ch = strtok(0, space);
      state = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(15, state);
#else//for one twenty eight
      sendSpiLED(0, 15, state);
      sendSpiLED(1, 15, state);
#endif//sy
    }
    else if(!strcmp(ch, s)) // shutdown
    {
      int state;
      ch = strtok(0, space);
      state = atoi(ch);
#ifndef ONE_TWENTY_EIGHT//for sixty four
      sendSpiLED(12, state);
#else//for one twenty eight
      sendSpiLED(0, 12, state);
      sendSpiLED(1, 12, state);
#endif
    }
    else if(!strcmp(ch, r)) // report
    {
      int state;
      ch = strtok(0, space);
      state = atoi(ch);
      printf(usb_cdc_putc, "report %d %d", state, 1);
      usb_cdc_putc(0x0D);
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

short buttonCheck(int row, int index)
{
  flag = FALSE;

#ifndef ONE_TWENTY_EIGHT//for sixty four
  if(((btnCurrent[row] ^ btnLast[row]) & (1 << index)) && ((btnCurrent[row] ^ btnState[row]) & (1 << index)))
    btnDebounceCount[row][index] = 0;
  else if (((btnCurrent[row] ^ btnLast[row]) & (1 << index)) == 0 && ((btnCurrent[row] ^ btnState[row]) & (1 << index)))
  {
    if(btnDebounceCount[row][index] < 6 && ++btnDebounceCount[row][index] == 6)
    {
      if(btnCurrent[row] & (1 << index))
        btnState[row] |= (1 << index);
      else
        btnState[row] &= ~(1 << index);
      flag = TRUE;
    }
  }
#else//for one twenty eight
  if(((btnCurrent[row] ^ btnLast[row]) & ((long)1 << index)) && ((btnCurrent[row] ^ btnState[row]) & ((long)1 << index)))
    btnDebounceCount[row][index] = 0;
  else if (((btnCurrent[row] ^ btnLast[row]) & ((long)1 << index)) == 0 && ((btnCurrent[row] ^ btnState[row]) & ((long)1 << index)))
  {
    if(btnDebounceCount[row][index] < 8 && ++btnDebounceCount[row][index] == 8)
    //sy if(btnDebounceCount[row][index] < 24 && ++btnDebounceCount[row][index] == 24)
    {
      if(btnCurrent[row] & ((long)1 << index))
        btnState[row] |= ((long)1 << index);
      else
        btnState[row] &= ~((long)1 << index);
      flag = TRUE;
    }
  }
#endif
  return flag;
}

void sendOscMsgPress(void)
{
  if(start_row == 0)
    output_bit(SR_A, 0);

  for(i = start_row; i < start_row + 4; i++)
  {
    output_bit(SR_CLK, 1);
    output_bit(SR_CLK, 0);
    
    output_bit(SR_A, 1);

    btnLast[i] = btnCurrent[i];

    output_bit(SR_SL, 0);
    delay_us(5);
    output_bit(SR_SL, 1);

#ifndef ONE_TWENTY_EIGHT//for sixty four
    for(j = 0; j < 8; j++)
#else//for one twenty eight
    for(j = 0; j < 16; j++)
#endif
    {
      if(input(SR_QH))
        btnCurrent[i] &= ~((long)1 << j);
      else
        btnCurrent[i] |= ((long)1 << j);
      
      if(buttonCheck(i, j))
      {
#ifdef ONE_TWENTY_EIGHT//for sixty four
        if(j < 8)
          printf(usb_cdc_putc, "press %d %d %d\r", 7 - i, j, ((btnState[i] & ((long)1 << j)) ? 1 : 0));
        else
#endif
          printf(usb_cdc_putc, "press %d %d %d\r", j, i, ((btnState[i] & ((long)1 << j)) ? 1 : 0));
      }
      output_bit(SR_CLK2, 1);
      output_bit(SR_CLK2, 0);
    }
  }
  if(start_row == 0)
    start_row = 4;
  else if(start_row == 4)
    start_row = 0;
}

/***********************************/
/*                                 */
/*  Functions for A/D Conversion   */
/*                                 */
/***********************************/
void enableAdc(int port)
{
  if(port >= kAdcFilterNumAdcs)
    return;

  if((gAdcEnableState & 0x7F) == 0)
    enableAdcFlag = TRUE;

  gAdcEnableState |= (1 << port);
}

void disableAdc(int port)
{
  if(port >= kAdcFilterNumAdcs)
    return;

  gAdcEnableState &= ~(1 << port);

  if((gAdcEnableState & 0x7F) == 0)
    enableAdcFlag = FALSE;
}

void sendOscMsgAdc(void)
{
  if(enableAdcFlag)
  {
    if(countAdc >= 25)
    {
      if((gAdcEnableState & (1 << loopAdc)) == (1 << loopAdc))
      {
        //test float fvalue = 0.0;
        fvalue = 0.0;
        for(i = 0; i < 8; i++)
        {
          set_adc_channel(adc_id[loopAdc]);
          fvalue += read_adc(ADC_START_AND_READ);
        }
        fvalue = (fvalue / 8.0) / 1024.0;

        printf(usb_cdc_putc, "adc %d %f", loopAdc, fvalue);
        usb_cdc_putc(0x0D);
      }
      loopAdc++;
      if(loopAdc >= kAdcFilterNumAdcs)
        loopAdc = 0;

      countAdc = 0;
    }
    countAdc++;
  }
}

/**********************************/
/*                                */
/*  Functions for Input Handling  */
/*                                */
/**********************************/
void inputInit(void)
{
  inCurrent = 0x00;
  inLast    = 0x00;
  inState   = 0x00;

  for(i = 0; i < 2; i++)
    inDebounceCount[i] = 0;
}

short inputCheck(int index)
{
  flag = FALSE;

  if(((inCurrent ^ inLast) & (1 << index)) && ((inCurrent ^ inState) & (1 << index)))
    inDebounceCount[index] = 0;
  else if (((inCurrent ^ inLast) & (1 << index)) == 0 && ((inCurrent ^ inState) & (1 << index)))
  {
    if(inDebounceCount[index] < 32 && ++inDebounceCount[index] == 32)
    {
      if(inCurrent & (1 << index))
        inState |= (1 << index);
      else
        inState &= ~(1 << index);
      flag = TRUE;
    }
  }
  return flag;
}

void sendOscMsgInput(void)
{
  inLast = inCurrent;

  for(j = 0; j < 2; j++)
  {
    if(j == 0)
      k = input(PIN_B4);
    else if(j == 1)
      k = input(PIN_B0);
    //test k = (k == 0);
    if(k == 0)
      inCurrent |= (1 << j);
    else
      inCurrent &= ~(1 << j);
      
    if(inputCheck(j))
    {
      printf(usb_cdc_putc, "input %d %d", j, ((inState & (1 << j)) ? 1 : 0));
      usb_cdc_putc(0x0D);
    }
  }
}

/***********************************/
/*                                 */
/*      Functions for EEPROM       */
/*                                 */
/***********************************/
void initAndSaveParaToEeprom(void)
{
  para.intensity = 0x0F;
  putParaToEeprom(&para, sizeof(para), 0);
}

void getParaFromEeprom(int *ptr, int num, int addr)
{
  int count;
  for(count = 0; count < num; count++)
    ptr[count] = read_eeprom(addr + count);
}

void putParaToEeprom(int *ptr,int num,int addr)
{
  int count;
  for(count = 0; count < num; count++)
    write_eeprom(addr + count, ptr[count]);
}
