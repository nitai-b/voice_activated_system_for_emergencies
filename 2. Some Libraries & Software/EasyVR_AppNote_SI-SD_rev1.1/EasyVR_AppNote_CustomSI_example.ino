#include "Arduino.h"
#if !defined(SERIAL_PORT_MONITOR)
  #error "Arduino version not supported. Please update your IDE to the latest version."
#endif

#if defined(SERIAL_PORT_USBVIRTUAL)
  // Shield Jumper on HW (for Leonardo and Due)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_USBVIRTUAL
#else
  // Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
  #include "SoftwareSerial.h"
  SoftwareSerial port(12, 13);
  #define pcSerial SERIAL_PORT_MONITOR
#endif

#include "EasyVR.h"

EasyVR easyvr(port);


int8_t group, idx;

void setup()
{
  // setup PC serial port
  pcSerial.begin(9600);

  // bridge mode?
  int mode = easyvr.bridgeRequested(pcSerial);
  switch (mode)
  {
  case EasyVR::BRIDGE_NONE:
    // setup EasyVR serial port
    port.begin(9600);
    // run normally
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge not started!"));
    break;
    
  case EasyVR::BRIDGE_NORMAL:
    // setup EasyVR serial port (low speed)
    port.begin(9600);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge connection aborted!"));
    break;
    
  case EasyVR::BRIDGE_BOOT:
    // setup EasyVR serial port (high speed)
    port.begin(115200);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("---"));
    pcSerial.println(F("Bridge connection aborted!"));
    break;
  }

  while (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    delay(1000);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(0); // US English
  
  group = 4; //<-- start group (considering to start from custom grammar 4)
}

void action();

void loop()
{
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.print("Say a command in WS ");
  Serial.println(group);
  easyvr.recognizeWord(group);
  //easyvr.recognizeCommand(group); // used for SD commands/trigger or for built-in trigger word

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  /* This part of the code is not needed if using SI commands only (excluding the built-in trigger ROBOT)
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    return;
  }
  idx = easyvr.getCommand();
  */
  
  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // print debug message for SI commands
    uint8_t flags, num;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpGrammar(group, flags, num))
    for (int8_t i = 0; i <= idx; ++i) easyvr.getNextWordLabel(name);
    pcSerial.print(F(" = "));
    pcSerial.println(name);
      
	// beep
    easyvr.playSound(0, EasyVR::VOL_FULL);
    // perform some action
    action();
  }
  else // errors or timeout
  { 
    if (easyvr.isTimeout())
      Serial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      Serial.print("Error ");
      Serial.println(err, HEX);
    }    
  }
}

void action()
{
    switch (group)
    {
    case 4:
      switch (idx)
      {
      case 0:
        // write your action code here
        group = 5; //<-- jump to WS 5 after trigger recognition in WS 4
        break;
      }
      break;
    case 5:
      switch (idx)
      {
      case 0:
        // write your action code here
        //group = X; // <-- jump to another WS X for composite commands
        break;
      
      case 1:
        // write your action code here
        //group = X; // <-- jump to another WS X for composite commands
        break;
      
      case 2:
        // write your action code here
        //group = X; // <-- jump to another WS X for composite commands
        break;
       //...add other case depending on how many custom commands you have in WS5!
      }
      break;
       }
}
