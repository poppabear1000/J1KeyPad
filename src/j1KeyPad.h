//---------------------------------------------------------------------
//  j1KeyPad.h - j1KeyPad "library"
//  ***NOTE*** there is no ".cpp" file in this library, it is all in the .h
//  Developed by Ed Bryson of Joshua 1 Systems, and Scott Shafer
//	Ed is my programing Yoda, I needed a keypad library, tried several, 
//	and then asked my Yoda to help me make one as a learning experience. 
//	I liked what he/we came up with and wanted to release it to the world 
//  for their use or amusement. The usual, it works for me but YMMV.
//  see Ed's Ebay store: https://www.ebay.com/str/joshua1systems
//---------------------------------------------------------------------

#ifndef __j1KEYPAD_H__
#define __j1KEYPAD_H__

#include <Arduino.h>
#define j1KEYPAD_MAXHIST    8		//max number of remembered previous key states from last 8 scan changes
#define FALSE               false
#define TRUE                true

//Change below boot to true for debugging to the serial monitor, false for not to serial debugging. 
bool DEBUG = false; //true = debug, false = no-debug

//---------------------------------------------------------------------
//  j1KeyPad typedefs
//---------------------------------------------------------------------

typedef struct _j1KEYPAD_Params
{
	uint8_t				rowCnt;			// row count
	uint8_t				colCnt;			// col count
	uint8_t				*rowPins;		// row pin #'s
	uint8_t				*colPins;		// col pin #'s
	char      			*keyTbl;    	// key char table
	uint8_t   			scanCnt;    	// scan count
	uint32_t  			scanMS;     	// scan interval
	uint32_t  			kaMS;       	// keepAlive interval;

}	j1KEYPAD_Params_t;

typedef union
{
	uint8_t				u8[4];
	uint16_t			u16[2];
	uint32_t			u32;
}	u32t_union_t;


typedef	struct	_j1KEYPAD_Ctrl
{
	j1KEYPAD_Params_t	params;
	uint8_t             keyCnt;
	u32t_union_t 		keyStatus;		
	uint32_t			history[j1KEYPAD_MAXHIST];
	uint8_t				historyIdx;
	uint32_t  			lastScanMS;
	uint32_t  			lastKeepAliveMS;
	u32t_union_t  		currentKeys;
}	j1KEYPAD_Ctrl_t;

//---------------------------------------------------------------------
//  j1KeyPad functions
//---------------------------------------------------------------------
bool		j1KeyPad_Start(j1KEYPAD_Ctrl_t *ctrl, j1KEYPAD_Params_t *params);
bool      	j1KeyPad_CheckKeys(j1KEYPAD_Ctrl_t *ctrl);
uint32_t	j1KeyPad_ScanKeys(j1KEYPAD_Ctrl_t *ctrl);

//---------------------------------------------------------------------
//  j1KeyPad_Start()
//---------------------------------------------------------------------
bool j1KeyPad_Start(j1KEYPAD_Ctrl_t *ctrl, j1KEYPAD_Params_t *params)
{
	uint8_t   row, col;
	memset(ctrl, 0, sizeof(j1KEYPAD_Ctrl_t));
	if (params->rowCnt == 0) return FALSE;
	if (params->colCnt == 0) return FALSE;
	if (params->rowCnt * params->colCnt > 32) return FALSE;
	if (params->scanCnt > j1KEYPAD_MAXHIST) return FALSE;
	ctrl->params  = *params;
	ctrl->keyCnt  = params->rowCnt * params->colCnt; 
	//  ctrl->lastScanMS      -= ctrl->params.scanMS;
	//  ctrl->lastKeepAliveMS -= ctrl->params.kaMS;
	for (row = 0; row < ctrl->params.rowCnt; row++)
	{
		pinMode(ctrl->params.rowPins[row], INPUT);
	}

	for (col = 0; col < ctrl->params.colCnt; col++)
	{
		pinMode(ctrl->params.colPins[col], INPUT_PULLUP);
	}

	return TRUE;
}

//---------------------------------------------------------------------
//  j1KeyPad_CheckKeys()
//---------------------------------------------------------------------

bool j1KeyPad_CheckKeys(j1KEYPAD_Ctrl_t *ctrl)
{
	uint32_t  keysOn;
	uint32_t  keysOff;
	uint32_t  newKeys;
	bool      updated = FALSE;
	uint8_t   i;
	uint32_t  *scanPtr;
	uint32_t  keyBit;

	if ((millis() - ctrl->lastKeepAliveMS) > ctrl->params.kaMS)
	{
		ctrl->lastKeepAliveMS = millis();
		updated = TRUE;
	}

	if ((millis() - ctrl->lastScanMS) > ctrl->params.scanMS)
	{
		ctrl->lastScanMS = millis();

		ctrl->history[ctrl->historyIdx++] = j1KeyPad_ScanKeys(ctrl);
		if (ctrl->historyIdx >= ctrl->params.scanCnt) ctrl->historyIdx = 0;
    
		keysOn  = 0xffffffff;
		keysOff = 0xffffffff;

		for (i = 0, scanPtr = ctrl->history; i < ctrl->params.scanCnt; i++, scanPtr++)
		{
			keysOn    &= *scanPtr;
			keysOff   &= ~(*scanPtr);
		}

		for (i = 0, keyBit = 1; i < ctrl->keyCnt; i++, keyBit <<= 1)
		{
			if (keysOn & keyBit)
			{
				if ((ctrl->currentKeys.u32 & keyBit) == 0)
				{
					ctrl->currentKeys.u32 |= keyBit;
					updated = TRUE;
				}
			}

			if (keysOff & keyBit)
			{
				if ((ctrl->currentKeys.u32 & keyBit))
				{
					ctrl->currentKeys.u32 &= ~keyBit;
					updated = TRUE;
				}
			}
		}
	}

  return updated;
}

//---------------------------------------------------------------------
//  j1KeyPad_Scankeys()
//---------------------------------------------------------------------
uint32_t j1KeyPad_ScanKeys(j1KEYPAD_Ctrl_t *ctrl)
{
	uint32_t  keysStatus  = 0;
	uint32_t  keyBit      = 1;
	uint8_t   row, col, rPin, cPin;

	for (row = 0; row < ctrl->params.rowCnt; row++)
	{
		rPin = ctrl->params.rowPins[row];

		#if (DEBUG)
		Serial.print(row);
		Serial.print("=");
		Serial.print(rPin);
		Serial.print(" { ");
		#endif

		pinMode(rPin, OUTPUT);
		digitalWrite(rPin, LOW);

		for (col = 0; col < ctrl->params.colCnt; col++)
		{
		  cPin = ctrl->params.colPins[col];
		
		  #if (DEBUG)
		  if (col) Serial.print(",");
		  Serial.print(col);
		  Serial.print("=");
		  Serial.print(cPin);
		  Serial.print(" [");
		  Serial.print(keyBit, HEX);
		  Serial.print("] ");
		  #endif

		  if (!digitalRead(cPin)) keysStatus |= keyBit;
		  
		  keyBit <<= 1;
		}
		#if (DEBUG)
		Serial.print(" }  ");
		#endif

		pinMode(rPin, INPUT);
	  }
	  #if (DEBUG)
	  Serial.print("\r\n");
	  #endif

	  ctrl->keyStatus.u32 = keysStatus;

	  return keysStatus;
}
#endif
