///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        ex_ezapp_style.c                           ////
////                                                                   ////
//// An EZ App Lynx example that shows how styles can be applied to    ////
//// the App.  Styles can be used to change colors, spacings, text     ////
//// sizes, alignment and more.                                        ////
////                                                                   ////
//// For full EZ App Lynx documentation, EZApp.PDF contains an         ////
//// overview of the entire system while EZApp.h documents the full    ////
//// API that can be used with this library.                           ////
////                                                                   ////
//// For full EZ App Lynx documentation, EZApp.PDF contains an         ////
//// overview of the entire system while EZApp.h documents the full    ////
//// API that can be used with this library.                           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

// Common definitions for all the examples.
// Hardware is configured in this .h file and then populates several
// macros/definitions used by all the EZ App Lynx examples.
#include "ex_ezapp_common.h"

#include <EZApp.h>

// all previously styles removed.
//
// text style:
//    left column: red color, bold, one point bigger
//    right column: green color, not bold, one point smaller.
rom char style1[] =
{
   EZAPP_STYLE_DEFAULT_ALL(),

   EZAPP_STYLE_TEXT_CREATE(
         true,
         false,
         1,
         EZAppCreateRGB(0xFF, 0, 0),
         -1,
         EZAppCreateRGB(0, 0xFF, 0),
         EZAppCreateRGB(0xFF, 0, 0)
      )
};

// previous styles kept (the text style applied in style1 will still be in
// effect).
//
// row colors style:
//    black border, 1dp with 5dp radius/rounding, background gradient is 
//    dark gray (top) to light gray (bottom)
rom char style2[] =
{
   EZAPP_STYLE_ROW_COLORS_CREATE(
         1,
         5,
         EZAppCreateRGB(0, 0, 0),
         EZAppCreateRGB(0xCC, 0xCC, 0xCC),
         EZAppCreateRGB(0xEE, 0xEE, 0xEE)
      )
};

// previous stlyes kept (the row color style applied in style2[] 
// will still be in effect).
//
// put text back to default colors
rom char style3[] =
{
   EZAPP_STYLE_TEXT_DEFAULT()   
};

// reset all styles back to default
//
// text style:
//    left column: red color, bold, one point bigger
//    right column: green color, not bold, one point smaller.
//
// row colors style:
//    black border, 1dp with 5dp radius/rounding, background gradient is 
//    dark gray (top) to light gray (bottom)
rom char style4[] =
{
   EZAPP_STYLE_DEFAULT_ALL(),

   EZAPP_STYLE_TEXT_CREATE(
         true,
         false,
         1,
         EZAppCreateRGB(0xFF, 0, 0),
         -1,
         EZAppCreateRGB(0, 0xFF, 0),
         EZAppCreateRGB(0xFF, 0, 0)
      ),
      
   EZAPP_STYLE_ROW_COLORS_CREATE(
         1,
         5,
         EZAppCreateRGB(0, 0, 0),
         EZAppCreateRGB(0xCC, 0xCC, 0xCC),
         EZAppCreateRGB(0xEE, 0xEE, 0xEE)
      )
};

// previous stlyes kept (text colors and row colors from style4 will still
// be in effect)
//
// row spacing style:
//    left column 50% width, left column contents center horizontal and
//    center vertical.  right column center horizontal and center vertical,
//    8dp padding below row (half of the default), 4dp padding inner (half of 
//    the default), 8dp padding between columns (space between buttons on the row)
//
// button style:
//    5dp border, 5dp border radius, bordor color red, text color green,
//    top background color dark gray, bottom background color black,
//    text size + 2, innerPadding 8dp
// EZAPP_STYLE_ROW_SPACINGS_CREATE(leftWidth, leftAlignment, rightAlignment, paddingInner, paddingPerColumn, paddingBelow)
// EZAPP_STYLE_BUTTON_CREATE(borderWidth, borderRadius, borderColor, textSizeDelta, textColor, topColor, bottomColor, innerPadding)
rom char style5[] =
{
   EZAPP_STYLE_ROW_SPACINGS_CREATE(
         50, 
         EZAPP_ALIGN_HOR_CENTER | EZAPP_ALIGN_VER_CENTER, 
         EZAPP_ALIGN_HOR_CENTER | EZAPP_ALIGN_VER_CENTER, 
         4, 
         8, 
         8
      ),
   
   EZAPP_STYLE_BUTTON_CREATE(
         5, 
         5, 
         EZAppCreateRGB(0xFF, 0, 0), 
         2, 
         EZAppCreateRGB(0, 0xFF, 0), 
         EZAppCreateRGB(0x33, 0x33, 0x33), 
         EZAppCreateRGB(0, 0, 0),
         8
      )
};

// all previously styles removed.
rom char style6[] =
{
   EZAPP_STYLE_DEFAULT_ALL()
};

void main(void)
{   
   char g_Style1Str[] = "Style 1";
   ezapp_field_index_t button0Index, button1Index;
   ezapp_buttons_t buttonConfig;
   
   HW_INIT();

   EZAppInit();
           
   EZAppSetTitleROM((rom char *)"ex_ezapp_style.c");  //0

   EZAppAddStylesROM(style1, sizeof(style1));   //1

   EZAppSetValueString(                         //2
         EZAppAddFieldString(), 
         g_Style1Str
      );
  
   EZAppAddStylesROM(style2, sizeof(style2));   //3

   EZAppSetValueStringROM(                      //4
         EZAppAddFieldString(), 
         (rom char*)"Style 2"
      );
  
  #if 0
   #warning quirk in the compiler
   EZAppAddStylesROM(&style3[0], sizeof(style3));   //5
  #else
   EZAppAddStylesROM(style3, sizeof(style3));   //5
  #endif

   EZAppSetValueStringROM(                      //6
         EZAppAddFieldString(), 
         (rom char*)"Style 3"
      );
  
   EZAppAddStylesROM(style4, sizeof(style4));   //7

   EZAppSetValueStringROM(                      //8
         EZAppAddFieldString(), 
         (rom char*)"Style 4"
      );
  
   EZAppAddStylesROM(style5, sizeof(style5));   //9

   EZAppSetValueStringROM(                      //10
         EZAppAddFieldString(), 
         (rom char*)"Style 5"
      );


   buttonConfig.numButtons = 4;  
   buttonConfig.fillRow = true;
   buttonConfig.oneState = true;
   buttonConfig.ledColor0 = EZAPP_LED_GRAY;
   buttonConfig.ledColor1 = EZAPP_LED_RED;
   
   //11
   button0Index = EZAppAddFieldButtonsROM(buttonConfig, NULL, (rom char*)"Button 1\tButton 2\tButton 3\tButton 4");
   
   //12
  #if 0
   #warning quirk in the compiler
   EZAppAddStylesROM(&style6[0], sizeof(style6));
  #else
   EZAppAddStylesROM(style6, sizeof(style6));
  #endif

   EZAppSetValueStringROM(                      //13
         EZAppAddFieldString(), 
         (rom char*)"Style 6"
      );
  
   //14
   button1Index = EZAppAddFieldButtonsROM(buttonConfig, NULL, (rom char*)"Button 5\tButton 6\tButton 7\tButton 8");

   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();
   }
}
