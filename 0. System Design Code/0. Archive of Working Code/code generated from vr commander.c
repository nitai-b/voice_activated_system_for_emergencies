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

//Groups and Commands
enum Groups
{
  GROUP_1  = 1,
  GROUP_2  = 2,
  GROUP_3  = 3,
};

enum Group1 
{
  G1_CALL = 0,
  G1_ANSWER = 1,
  G1_LIGHTS = 2,
  G1_LED = 3,
  G1_OTHER = 4,
};

enum Group2 
{
  G2_DOCTOR = 0,
  G2_PARENT = 1,
  G2_MOM = 2,
  G2_DAD = 3,
  G2_SON = 4,
  G2_DAUGHTER = 5,
  G2_ON = 6,
  G2_OFF = 7,
  G2_OTHER = 8,
};

enum Group3 
{
  G3_ZERO = 0,
  G3_ONE = 1,
  G3_TWO = 2,
  G3_THREE = 3,
  G3_FOUR = 4,
  G3_FIVE = 5,
  G3_SIX = 6,
  G3_SEVEN = 7,
  G3_EIGHT = 8,
  G3_NINE = 9,
};


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
  easyvr.setLanguage(0);

  group = EasyVR::TRIGGER; //<-- start group (customize)
}

void action();

void loop()
{
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.print("Say a command in Group ");
  Serial.println(group);
  easyvr.recognizeCommand(group);

  do
  {
    // can do some processing while waiting for a spoken command
  }
  while (!easyvr.hasFinished());
  
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx >= 0)
  {
    // built-in trigger (ROBOT)
    // group = GROUP_X; <-- jump to another group X
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // print debug message
    uint8_t train = 0;
    char name[32];
    Serial.print("Command: ");
    Serial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      Serial.print(" = ");
      Serial.println(name);
    }
    else
      Serial.println();
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
    case GROUP_1:
      switch (idx)
      {
      case G1_CALL:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_ANSWER:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_LIGHTS:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_LED:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G1_OTHER:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    case GROUP_2:
      switch (idx)
      {
      case G2_DOCTOR:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_PARENT:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_MOM:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_DAD:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_SON:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_DAUGHTER:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_ON:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_OFF:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G2_OTHER:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    case GROUP_3:
      switch (idx)
      {
      case G3_ZERO:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_ONE:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_TWO:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_THREE:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_FOUR:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_FIVE:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_SIX:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_SEVEN:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_EIGHT:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      case G3_NINE:
        // write your action code here
        // group = GROUP_X; <-- or jump to another group X for composite commands
        break;
      }
      break;
    }
}
