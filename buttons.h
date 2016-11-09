#ifndef BUTTONS_H
#define BUTTONS_H

#define buttonUp       0
#define buttonDown     1
#define buttonStart    2
#define buttonEnter    3


unsigned long _LastButtonTime = 0;
int  _buttons[4] = { 0, 0, 0, 0};
byte _pressed[4] = { 0, 0, 0, 0};


/*
   Initialize this thread.
*/
void ButtonsInit() {
  _LastButtonTime = millis();
  pinMode (ButtonUpPin,    INPUT_PULLUP);
  pinMode (ButtonDownPin,  INPUT_PULLUP);
  pinMode (ButtonStartPin, INPUT_PULLUP);
  pinMode (ButtonEnterPin, INPUT_PULLUP);
}



/*
   Buttons thread, register the current button state.
*/
void ButtonsThread() {
  if (gCurrentTimeInMS != _LastButtonTime) {
    int bElapsed = gCurrentTimeInMS - _LastButtonTime;
    _LastButtonTime = gCurrentTimeInMS;

    /*
       Register how long a button is continuous pressed.
    */
    (digitalRead(ButtonUpPin) == 0)    ? _buttons[buttonUp]    += bElapsed : _buttons[buttonUp]    = 0;
    (digitalRead(ButtonDownPin) == 0)  ? _buttons[buttonDown]  += bElapsed : _buttons[buttonDown]  = 0;
    (digitalRead(ButtonStartPin) == 0) ? _buttons[buttonStart] += bElapsed : _buttons[buttonStart] = 0;
    (digitalRead(ButtonEnterPin) == 0) ? _buttons[buttonEnter] += bElapsed : _buttons[buttonEnter] = 0;
  }
}



/*
   Check if a button is pressed for at least a given time.
*/
byte button_Press(byte Button_no, int msTime) {
#if DebugButton == true
  if (_buttons[Button_no]) {
    Serial.print(F("button_Press("));
    Serial.print(Button_no);
    Serial.print(F(","));
    Serial.print(msTime);
    Serial.print(F(") = "));
    Serial.println(_buttons[Button_no]);
  }
#endif
  if (_buttons[Button_no] >= msTime) {
    _buttons[Button_no] = 0;
    return 1;
  } else {
    return 0;
  }
}



/*
   Check if a button has been pressed for at least a given time,
   and that it is released afterwards.
*/
byte button_Used(byte Button_no, int msTime) {
  if (_pressed[Button_no]) {
    if (_buttons[Button_no] == 0) {
#if DebugButton == true
      Serial.print(F("button_Used("));
      Serial.print(Button_no);
      Serial.print(F(","));
      Serial.print(msTime);
      Serial.println(F(")"));
#endif
      _pressed[Button_no] = 0;
      return 1;
    }
  } else {
    if (button_Press(Button_no, msTime)) {
      _pressed[Button_no] = 1;
    }
  }
  return 0;
}



/*
   Test Up/Down buttons and register the time if pressed.
*/
byte ReadButton(byte& Direction, unsigned long& Timer ) {
  boolean f_btnUp, f_btnDn;

  if (digitalRead (ButtonUpPin) == 0) {   // Up button is pressed
    if (Direction != DirectionUp)
      Timer = millis();         // Register time of new direction
    f_btnUp = true;
    Direction = DirectionUp;    // Register new direction
    delay(35);
  } else
    f_btnUp = false;

  if (digitalRead (ButtonDownPin) == 0) {   // Down button is pressed
    if (Direction != DirectionDown)
      Timer = millis();         // Register time of new direction
    f_btnDn = true;
    Direction = DirectionDown;  // Register new direction
    delay(35);
  } else
    f_btnDn = false;

  if (f_btnUp == false && f_btnDn == false)
    Direction = DirectionNone;  // No Up or Down direction.
}


int Set(int& Set, int Up, int Low, int Step, long Timer, byte Direction) {
  int step_size;
  int ButtonPressed;

  if (Set > Up)
    Set = Up;
  if (Set < Low)
    Set = Low;

  delay(50);
  if (Direction == DirectionUp) {
    ButtonPressed = digitalRead(ButtonUpPin);
  }
  if (Direction == DirectionDown) {
    ButtonPressed = digitalRead(ButtonDownPin);
  }

  if (ButtonPressed == 0 && Direction != DirectionNone) {
    if (((millis() - Timer) / 1000) >= 4)
      step_size = (Step * 20);
    else {
      if (((millis() - Timer) / 1000) >= 2)
        step_size = (Step * 5);
      else
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

float Set(float& Set, float Up, float Low, float Step, long Timer, byte Direction) {
  float step_size;
  int ButtonPressed;

  if (Set > Up)
    Set = Up;
  if (Set < Low)
    Set = Low;

  delay(50);
  if (Direction == DirectionUp) {
    ButtonPressed = digitalRead(ButtonUpPin);
  }
  if (Direction == DirectionDown) {
    ButtonPressed = digitalRead(ButtonDownPin);
  }

  if (ButtonPressed == 0 && Direction != DirectionNone) {
    if (((millis() - Timer) / 1000) >= 4)
      step_size = (Step * 20.0);
    else {
      if (((millis() - Timer) / 1000) >= 2)
        step_size = (Step * 5.0);
      else
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

byte Set(byte& Set, byte Up, byte Low, byte Step, long Timer, byte Direction) {
  int step_size;
  int ButtonPressed;

  if (Set > Up)
    Set = Up;
  if (Set < Low)
    Set = Low;

  delay(50);
  if (Direction == DirectionUp) {
    ButtonPressed = digitalRead(ButtonUpPin);
  }
  if (Direction == DirectionDown) {
    ButtonPressed = digitalRead(ButtonDownPin);
  }

  if (ButtonPressed == 0 && Direction != DirectionNone) {
    if (((millis() - Timer) / 1000) >= 4)
      step_size = (Step * 20);
    else {
      if (((millis() - Timer) / 1000) >= 2)
        step_size = (Step * 5);
      else
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
