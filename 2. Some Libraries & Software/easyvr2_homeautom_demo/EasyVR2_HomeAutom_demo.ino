/**
  EasyVR 2.0
  Custom Speaker Independent and Speaker Dependent/Verification demo.

  Simple home automation demo showing how to use the EasyVR Shield with Arduino
  to implement a very simple building automation system with
  integrated access control based on voice passwords.

  User names and related passwords can be trained directly within the
  Arduino application without using the EasyVR Commander.

  EasyVR Commander must be used to import speech output recordings (sound table)
  and Speaker Independent (SI) commands created with the QuickT2SI software.

  SI commands to be placed in grammar number 5:

  PASSWORD_SETUP
  PASSWORD_ACTIVATED
  CURTAIN_UP
  CURTAIN_DOWN
  LIGHTS_ON
  LIGHTS_OFF

  SI trigger is in grammar 4.

  The system waits for the trigger word and then, if the system has been
  previously "locked" with the command "Password activated", it asks for a user
  name and password to "unlock" the system, otherwise it waits for a command.

  Use long names and passwords to increase recognition performance.
  Moreover, for better security set the recognition level to HARDER using the
  setLevel function.

  Servo for simulating a "curtain" is connected to pin 9.
  LED for simulating "lights" is connected to pin 8.

  With the EasyVR Shield, the green LED is ON while the module
  is listening (using pin IO1 of EasyVR).
  Details are displayed on the serial monitor window.

**
  Example code for the EasyVR library v1.3
  Written in 2013 by RoboTech srl for VeeaR <http://www.veear.eu>

  To the extent possible under law, the author(s) have dedicated all
  copyright and related and neighboring rights to this software to the
  public domain worldwide. This software is distributed without any warranty.

  You should have received a copy of the CC0 Public Domain Dedication
  along with this software.
  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#include "SoftwareSerial.h"
SoftwareSerial port(12,13);
#else // Arduino 0022 - use modified NewSoftSerial
#include "WProgram.h"
#include "NewSoftSerial.h"
NewSoftSerial port(12,13);
#endif

#include "EasyVR.h"
#include <Servo.h>

// Exported by EasyVR Commander
#include "EasyVR2_custom_data.h"

Servo myservo;
EasyVR easyvr(port);
EasyVRBridge bridge;

uint32_t mask = 0;
int8_t group = 0;
uint8_t train = 0;
int8_t trained_pw = 0;  //number of existing passwords
int8_t trained_un = 0;  //number of existing user names
bool lock = 0;          // locked/unlocked flag
bool error = 0;         // error flag
char name[32];


void setup()
{
  // bridge mode?
  if (bridge.check())
  {
    cli();
    bridge.loop(0, 1, 12, 13);
  }

  Serial.begin(9600);
  port.begin(9600);

  myservo.attach(9);
  myservo.write(90);
  pinMode(8, OUTPUT); //set pin 8 as output for LED
  digitalWrite(8, LOW); //switch off LED

  if (!easyvr.detect())
  {
    Serial.println("EasyVR not detected!");
    for (;;);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  Serial.println("EasyVR detected!");
  easyvr.setTimeout(5);
  easyvr.setLanguage(EasyVR::ITALIAN);

  int16_t count = 0;

  if (easyvr.getGroupMask(mask)) // get trained user names and passwords
  {
    uint32_t msk = mask;
    for (group = 0; group <= EasyVR::PASSWORD; ++group, msk >>= 1)
    {
      if (!(msk & 1)) continue;
      count = easyvr.getCommandCount(group);
      if (group == EasyVR::TRIGGER)
        Serial.print("Trigger: ");
      else if (group == EasyVR::PASSWORD)
      {
        Serial.print("Password: ");
        trained_pw = count;
      }
      else
      {
        if (group == 1)
          trained_un = count;
        Serial.print("Group ");
        Serial.print(group);
        Serial.print(": ");
      }

      Serial.println(count);
      for (int8_t idx = 0; idx < count; ++idx)
      {
        if (easyvr.dumpCommand(group, idx, name, train))
        {
          Serial.print(idx);
          Serial.print(" = ");
          Serial.print(name);
          Serial.print(", Trained ");
          Serial.print(train, DEC);
          if (!easyvr.isConflict())
            Serial.println(" times, OK");
          else
          {
            int8_t confl = easyvr.getWord();
            if (confl >= 0)
              Serial.print(" times, Similar to Word ");
            else
            {
              confl = easyvr.getCommand();
              Serial.print(" times, Similar to Command ");
            }
            Serial.println(confl);
          }
        }
      }
    }
  }
  easyvr.setLevel(EasyVR::HARDER);
  easyvr.setKnob(EasyVR::STRICTER);
  easyvr.playSound(SND_Hello, EasyVR::VOL_FULL);
}

void loop()
{
  int idx_cmd;
  int idx_pwd;
  int idx;

  easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  Serial.println("Wait for trigger");
  easyvr.recognizeWord(4); // recognise grammar number 4 (custom SI trigger word must be here!)
  while (!easyvr.hasFinished()); // wait for trigger

  idx_cmd = easyvr.getWord(); // get recognised command
  if ((idx_cmd == WS4_HELLO_DEVICE) && (lock == 0))
  {
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

    easyvr.playSound(SND_Hello_give_command , EasyVR::VOL_FULL);  // ask for a command
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

    Serial.println("Wait for commmand");
    easyvr.recognizeWord(5); // recognise grammar number 5 (custom SI commands must be here!)
    while (!easyvr.hasFinished()); // wait for command

    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

    idx_cmd = easyvr.getWord(); // get recognised command
  }

  if (trained_un != trained_pw)
  {
    Serial.println("unexpected error: different number of users and passwords!");
    easyvr.playSound(EasyVR::BEEP, EasyVR::VOL_FULL);
    // security issue: a real device should stay locked and fall back to manual unlock
    // here we just reset trained users/passwords and unlock
    Serial.print("resetting memory, please wait... ");
    easyvr.resetAll();
    Serial.println("done!");
    easyvr.playSound(EasyVR::BEEP, EasyVR::VOL_FULL);
    trained_un = trained_pw = 0;
    lock = 0;
    idx_cmd = -1; // set to invalid command to skip the following code
  }

  if ((lock == 1) && (idx_cmd == WS4_HELLO_DEVICE)) // system locked, ask for password!
  {
    Serial.println("system locked, ask user name");
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off
    easyvr.playSound(SND_Please_say_name , EasyVR::VOL_FULL);  // asking for user name

    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

    Serial.println("Say a name in Group 1");
    easyvr.recognizeCommand(1); // recognise command in group 1
    while (!easyvr.hasFinished()); // wait for user name

    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

    idx_cmd = easyvr.getCommand(); // get recognised user name

    if (idx_cmd >= 0)
    {
      Serial.print("Name: ");
      if (easyvr.dumpCommand(1, idx_cmd, name, train))
        Serial.println(name);
      else
        Serial.println();

      easyvr.playSound(SND_Please_say_your_password , EasyVR::VOL_FULL);  // ask for password

      easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

      Serial.println("Say the password");
      easyvr.recognizeCommand(EasyVR::PASSWORD); // set group 16
      while (!easyvr.hasFinished()); // wait for password

      easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

      idx_pwd = easyvr.getCommand(); // get recognised password

      if (idx_pwd >= 0)
      {
        Serial.print("Password: ");

        if (easyvr.dumpCommand(EasyVR::PASSWORD, idx_pwd, name, train))
        {
          Serial.print(" = ");
          Serial.println(name);
        }
        else
          Serial.println();

        if ( idx_pwd == idx_cmd) // index of username and password are the same, access granted
        {

          Serial.println("Access granted");
          easyvr.playSound(SND_Access_granted , EasyVR::VOL_FULL);

          lock = 0; //system unlocked!
        }
        else // index of username and password differ, access is denied
        {
          Serial.println("Access denied");
          easyvr.playSound(SND_Access_denied , EasyVR::VOL_FULL);
        }
      }

      int16_t err = easyvr.getError();

      if (easyvr.isTimeout() || (err >= 0)) // password timeout, access is denied
      {
        Serial.println("Error, try again...");
        easyvr.playSound(SND_Access_denied , EasyVR::VOL_FULL);
      }
    }

    else
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
  else
  {
    switch (idx_cmd)
    {
    case WS5_PASSWORD_SETUP:
      Serial.println("user name setup");

      for (idx = 1; idx < 3; ++idx)
      {
        easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

        if (idx == 1)
        {
          Serial.println("add command");
          Serial.println(easyvr.addCommand (1, trained_un));
          strcpy (name, "NAME");
          name[4]= '0'+trained_un;
          easyvr.setCommandLabel (1, trained_un, name);
          easyvr.playSound(SND_Please_say_name , EasyVR::VOL_FULL);  // asking for user name
        }

        else easyvr.playSound(SND_Please_repeat , EasyVR::VOL_FULL);  // repeat!

        easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
        easyvr.trainCommand (1, trained_un);

        while (!easyvr.hasFinished()); // waits for user name to be trained
        int16_t err = easyvr.getError();
        if (err >= 0)
        {
          error = 1;
          Serial.print("Error ");
          Serial.println(err, HEX);
        }
      }

      Serial.println("password setup");
      for (idx = 1; idx < 3; ++idx)
      {
        easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

        if (idx ==1)
        {
          easyvr.addCommand (16, trained_pw);
          strcpy (name, "PWD_");
          name[4]= '0'+trained_pw;
          easyvr.setCommandLabel (16, trained_pw, name);
          easyvr.playSound(SND_Please_say_your_password , EasyVR::VOL_FULL);  // asking for password
        }

        else easyvr.playSound(SND_Please_repeat , EasyVR::VOL_FULL);  // repeat!

        easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
        easyvr.trainCommand (16, trained_pw);

        while (!easyvr.hasFinished()); // waits for password to be trained

        int16_t err = easyvr.getError();
        if (err >= 0)
        {
          error = 1;
          Serial.print("Error ");
          Serial.println(err, HEX);
        }
      }

      if (!error)
      {
        trained_un++;
        trained_pw++;
      }

      break;

    case WS5_PASSWORD_ACTIVATED:
      Serial.println("password locked!");
      easyvr.playSound(SND_Pwd_activated , EasyVR::VOL_FULL); // Password activated and system locked!
      lock = 1; //locked!
      break;

    case WS5_CURTAIN_UP:
      Serial.println("Curtain up");
      myservo.write(120);
      break;

    case WS5_CURTAIN_DOWN:
      Serial.println("Curtain down");
      myservo.write(10);
      break;

    case WS5_LIGHTS_ON:
      Serial.println("Lights ON");
      digitalWrite(8, HIGH); //switch on LED;
      break;

    case WS5_LIGHTS_OFF:
      Serial.println("Lights OFF");
      digitalWrite(8, LOW); //switch off LED;
      break;
    }
  }
}


