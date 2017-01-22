#ifndef BUTTONS_H
#define BUTTONS_H


byte  kp_key = 0;
byte  kp_duration = 0;
int   kp_repeat_count = 0;
unsigned long kp_repeat = 0;


#define KP_NORMAL    100
#define KP_LONG      1000
#define KP_REPEAT    75

#if DebugButton == true
const char *keynames[16] = { "None", "Enter", "Start", "3", "Down", "5", "6", "7", "Up", "9", "10", "11", "Escape", "13", "14", "All" };
#endif

const byte buttonNone    = 0;
const byte buttonEnter   = 1;
const byte buttonStart   = 2;
const byte buttonDown    = 4;
const byte buttonUp      = 8;
const byte buttonEscape  = 12;



/*
   Initialize this thread.
*/
void ButtonsInit() {
  pinMode (ButtonUpPin,    INPUT_PULLUP);
  pinMode (ButtonDownPin,  INPUT_PULLUP);
  pinMode (ButtonStartPin, INPUT_PULLUP);
  pinMode (ButtonEnterPin, INPUT_PULLUP);
}



/*
   Buttons thread, register the current button state.
*/
void ButtonsThread() {
  static byte _kp_key_current = 0;
  static byte _kp_key_last = 0;
  static unsigned long _kp_key_start = 0;
  static unsigned long lastcall = 0;

  if (gCurrentTimeInMS < (lastcall + 20)) {
    return;
  }

  lastcall = gCurrentTimeInMS;

  _kp_key_current = 0;
  if (digitalRead (ButtonEnterPin) == 0)
    _kp_key_current += 1;
  if (digitalRead (ButtonStartPin) == 0)
    _kp_key_current += 2;
  if (digitalRead (ButtonDownPin) == 0)
    _kp_key_current += 4;
  if (digitalRead (ButtonUpPin) == 0)
    _kp_key_current += 8;

  if (_kp_key_current) {
    // Some key is pressed.
    if (_kp_key_current != _kp_key_last) {
      // Steady key.
      _kp_key_last = _kp_key_current;
      _kp_key_start = gCurrentTimeInMS;
    } else {
      if ((gCurrentTimeInMS > (_kp_key_start + KP_NORMAL)) && (! kp_duration)) {
        // At least 100 mSec pressed.
        kp_key = _kp_key_current;
        kp_duration = gCurrentTimeInMS - _kp_key_start;
#if DebugButton == true
        Serial.print(F("ButtonsThread: key: "));
        Serial.print(kp_key);
        Serial.print(F(" "));
        Serial.print(keynames[kp_key]);
        Serial.print(F(" duration: "));
        Serial.print(kp_duration);
        Serial.println(F(" single"));
#endif
      }
      if ((gCurrentTimeInMS > (_kp_key_start + KP_LONG)) && (! kp_repeat)) {
        kp_repeat = gCurrentTimeInMS;
      }
      if ((gCurrentTimeInMS > (_kp_key_start + KP_LONG)) && (gCurrentTimeInMS > (kp_repeat + KP_REPEAT))) {
        kp_key = _kp_key_current;
        kp_duration = gCurrentTimeInMS - _kp_key_start;
        kp_repeat_count++;
#if DebugButton == true
        Serial.print(F("ButtonsThread: key: "));
        Serial.print(kp_key);
        Serial.print(F(" "));
        Serial.print(keynames[kp_key]);
        Serial.print(F(" duration: "));
        Serial.print(kp_duration);
        Serial.print(F(" repeated: "));
        Serial.println(kp_repeat_count);
#endif
        kp_repeat = 0;
      }
    }
  } else {
    // No key pressed, clear variables.
    kp_key = 0;
    kp_duration = 0;
    kp_repeat = 0;
    kp_repeat_count = 0;
    _kp_key_current = _kp_key_last = 0;
  }
}



/*
   Non-blocking read the pressed key and remove it from the buffer.
*/
byte ReadKey() {
  byte rc = 0;

  if (kp_key) {
    rc = kp_key;
    kp_key = 0;
  }

  return rc;
}



/*
   Test Up/Down buttons and register the time if pressed.
*/
byte ReadButton(byte& Direction, unsigned long& Timer, byte button) {
  boolean f_btnUp, f_btnDn;

  if (button == buttonUp) {     // Up button is pressed
    if (Direction != DirectionUp)
      Timer = millis();         // Register time of new direction
    f_btnUp = true;
    Direction = DirectionUp;    // Register new direction
  } else
    f_btnUp = false;

  if (button == buttonDown) {   // Down button is pressed
    if (Direction != DirectionDown)
      Timer = millis();         // Register time of new direction
    f_btnDn = true;
    Direction = DirectionDown;  // Register new direction
  } else
    f_btnDn = false;

  if (f_btnUp == false && f_btnDn == false)
    Direction = DirectionNone;  // No Up or Down direction.
}



int Set(int& Set, int Up, int Low, int Step, long Timer, byte Direction, byte button) {
  int step_size;
  int ButtonPressed;

  if (Set > Up)
    Set = Up;
  if (Set < Low)
    Set = Low;

  if (Direction == DirectionUp) {
    ButtonPressed = (button == buttonUp);
  }
  if (Direction == DirectionDown) {
    ButtonPressed = (button == buttonDown);
  }

  if (ButtonPressed && Direction != DirectionNone) {
    if (kp_repeat_count > 20) {
      step_size = (Step * 20);
    } else if (kp_repeat_count > 5) {
      step_size = (Step * 5);
    } else {
      step_size = Step;
    }
    if (Direction == DirectionUp) {
      if (Set + step_size > Up)
        Set = Up;
      else
        Set += step_size;
    } else if (Direction == DirectionDown) {
      if (Set - step_size < Low)
        Set = Low;
      else
        Set -= step_size;
    }
  }
}



float Set(float& Set, float Up, float Low, float Step, long Timer, byte Direction, byte button) {
  float step_size;
  int ButtonPressed;

  if (Set > Up)
    Set = Up;
  if (Set < Low)
    Set = Low;

  if (Direction == DirectionUp) {
    ButtonPressed = (button == buttonUp);
  }
  if (Direction == DirectionDown) {
    ButtonPressed = (button == buttonDown);
  }

  if (ButtonPressed && Direction != DirectionNone) {
    if (kp_repeat_count > 20) {
      step_size = (Step * 20.0);
    } else if (kp_repeat_count > 5) {
      step_size = (Step * 5.0);
    } else {
      step_size = Step;
    }
    if (Direction == DirectionUp) {
      if (Set + step_size > Up)
        Set = Up;
      else
        Set += step_size;
    } else if (Direction == DirectionDown) {
      if (Set - step_size < Low)
        Set = Low;
      else
        Set -= step_size;
    }
  }
}



byte Set(byte& Set, byte Up, byte Low, byte Step, long Timer, byte Direction, byte button) {
  byte step_size;
  int ButtonPressed;

  if (Set > Up)
    Set = Up;
  if (Set < Low)
    Set = Low;

  if (Direction == DirectionUp) {
    ButtonPressed = (button == buttonUp);
  }
  if (Direction == DirectionDown) {
    ButtonPressed = (button == buttonDown);
  }

  if (ButtonPressed && Direction != DirectionNone) {
    if (kp_repeat_count > 20) {
      step_size = (Step * 20);
    } else if (kp_repeat_count > 5) {
      step_size = (Step * 5);
    } else {
      step_size = Step;
    }
    if (Direction == DirectionUp) {
      if (Set + step_size > Up)
        Set = Up;
      else
        Set += step_size;
    } else if (Direction == DirectionDown) {
      if (Set - step_size < Low)
        Set = Low;
      else
        Set -= step_size;
    }
  }
}


#endif
