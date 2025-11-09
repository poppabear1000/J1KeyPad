//---------------------------------------------------------------------
//
//  j1KeyPadTest.ino - test the keypad library
//  Using the typical 4x4 bubble/button type matrix keypad
//
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//  common defines
//---------------------------------------------------------------------

#define FALSE   false
#define TRUE    true

#define j1KEYPAD_MAXHIST    8
#include <j1KeyPad.h>

//---------------------------------------------------------------------
//  global data
//---------------------------------------------------------------------
bool serialMonitorDebugging = false; //true = 1, use serial monitor, false = 0 use of serial monitor

j1KEYPAD_Ctrl_t   keyPad;

//---------------------------------------------------------------------
//  setup() - arduino sketch
//---------------------------------------------------------------------

void setup() 
{
  j1KEYPAD_Params_t   keyPad_Params;

  // setup our serial console over usb
  Serial.begin(115200);
  while(!Serial) delay(10); 
  delay(1000);
  Serial.print("\r\n\n\n\nOnce more unto the breach, dear friends, once more ...\r\n\n");

  // start our keyPad, modify pin GPIO for the board you are using, standard I/O pins
  static  uint8_t rowPins[] = { 0, 1, 2, 3 }; //change these pin numbers to gpio that your using for your matrix key rows
  static  uint8_t colPins[] = { 4, 5, 6, 7 }; //change these pin numbers to gpio that your using for your matrix key columns
  static  char    keyTbl[]  = "123A456B789C*0#D"; //this is from standard, 4x4 membrane matrix keys, NO SPACES between identifiers
                                                  //also, only a SINGLE character identifier per key.

  memset(&keyPad_Params, 0, sizeof(keyPad_Params)); //ensure memory space is clean

  keyPad_Params.rowCnt  = 4; //number of keys in each row
  keyPad_Params.colCnt  = 4; //number of keys in each column
  keyPad_Params.rowPins = rowPins;
  keyPad_Params.colPins = colPins;
  keyPad_Params.keyTbl  = keyTbl;
  keyPad_Params.scanCnt = 5; //how many scans in milliseconds of a pressed key or keys to be true before that is considered a valid push 
  keyPad_Params.scanMS  = 2; //how often to scan the keys for changes in milliseconds
  keyPad_Params.kaMS    = 1000; //value in milliseconds, 5000 ms is good for debugging, the Keep-Alive if no key activity, does a scan at this time value 
                                //in case you need to send something down a communication line: like Can Bus, 
                                //or other to some receiver to varify commincation line is intact

  if (!j1KeyPad_Start(&keyPad, &keyPad_Params)) { Serial.print("keyPad failed to Start ...\r\n"); }
  else { Serial.print("keyPad Started ...\r\n"); }
}

//---------------------------------------------------------------------
//  loop() - arduino sketch
//---------------------------------------------------------------------

void loop()
{
  uint16_t  keysBits, keyBit;
  uint8_t   keyNum;

  if (j1KeyPad_CheckKeys(&keyPad))
  {
    if (serialMonitorDebugging) //for debugging on serial monitor
    { 
      keysBits  = keyPad.currentKeys.u16[0];
      Serial.printf("keyPad = %04x =", keysBits);
      for (keyNum = 0, keyBit = 1; keyNum < 16; keyNum++, keyBit <<= 1)
      {
        if (keysBits & keyBit)
        {
          Serial.print(' ');
          Serial.print(keyPad.params.keyTbl[keyNum]);
        }
      }
      Serial.print("\r\n");
    }  
  }
}
