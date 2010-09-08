///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     USB_BOOTLOADER.H                              ////
////                                                                   ////
////  This include file must be included by any application loaded     ////
////  by the example USB bootloader (ex_usb_bootloader                 ////
////                                                                   ////
////  The directives in this file relocate the reset and interrupt     ////
////  vectors as well as reserving space for the bootloader.           ////
////                                                                   ////
////  For more documentation see ex_usb_bootloader.c                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// VERSION HISTORY                                                   ////
////                                                                   ////
//// March 5th, 2009:                                                  ////
////   Cleanup for Wizard.                                             ////
////   PIC24 Initial release.                                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2009 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

//how big is the bootloader?
//the bootloader will reside from address 0x0000 to this location.  the
//application will then sit at this location+1 to the end of program memory.
//#if defined(__DEBUG)
#define LOADER_SIZE        (0x1FFF)
//#else
//#define LOADER_SIZE        (0x17FF)
//#endif

//the loader and application need a common flag that determines if we are in
//the bootloader or application, that way the ISR knows where to go.  this
//is the location in ram that is reserved for this flag.
#define LOC_IN_LOADER_FLAG  0x25

//// --- end configuration --- ////////////////////////////////////////////

#reserve LOC_IN_LOADER_FLAG

int8 g_InBootloader;
#locate g_InBootloader=LOC_IN_LOADER_FLAG

#define LOADER_START       (0)
#define LOADER_END         (LOADER_SIZE)
#define APPLICATION_START  (LOADER_SIZE+1)
#if defined(__USB_87J50__)
 #define APPLICATION_END    (getenv("PROGRAM_MEMORY")-9) //configuration bits
#else
 #define APPLICATION_END    (getenv("PROGRAM_MEMORY")-1)
#endif 
#define APPLICATION_ISR    (APPLICATION_START+8)

#ifdef _bootloader
 /*
  Provide an empty application, so if you load this .HEX file into the pic
  without an application this will prevent the pic from executing unknown code.
 */
 #org APPLICATION_START,APPLICATION_START+0xF
 void BlankApplication(void) 
 {
   while(TRUE);
 }

 //we need to prevent the loader from using application space
 #if (APPLICATION_END > 0x10000)
   #org APPLICATION_START+0x10, 0xFFFF {}
   #if (APPLICATION_END > 0x20000)
      #org 0x10000, 0x1FFFF {}
      #org 0x20000, APPLICATION_END {}
   #else
      #org 0x10000, APPLICATION_END {}
   #endif
 #else
   #org APPLICATION_START+0x10, APPLICATION_END {}
 #endif

   #define  USB_CONFIG_PID       0x0034

   #define USB_STRINGS_OVERWRITTEN
#if 0//sy
   char USB_STRING_DESC_OFFSET[]={0,4,12};

   // Here is where the "CCS" Manufacturer string and "SERIAL DEMO" are stored.
   // Strings are saved as unicode.
   // These strings are mostly only displayed during the add hardware wizard.
   // Once the operating system drivers have been installed it will usually display
   // the name from the drivers .INF.
   char const USB_STRING_DESC[]={
      //string 0
            4, //length of string index
            0x03, //USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
            0x09,0x04,   //Microsoft Defined for US-English
      //string 1  - manufacturer
            8, //length of string index
            0x03, //USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
            'C',0,
            'C',0,
            'S',0,
      //string 2 - product
            30, //length of string index
            0x03, //USB_DESC_STRING_TYPE, //descriptor type 0x03 (STRING)
            'C',0,
            'D',0,
            'C',0,
            ' ',0,
            'B',0,
            'o',0,
            'o',0,
            't',0,
            'l',0,
            'o',0,
            'a',0,
            'd',0,
            'e',0,
            'r',0
   };
#endif//sy
#endif   //_bootloader

#ifndef _bootloader
 //in the application, this moves the reset and isr vector out of the bootload
 //space.  it then reserves the loader space from being used by the application.
 #build(reset=APPLICATION_START, interrupt=APPLICATION_ISR)
 #org 0, LOADER_END {}
#endif
