#ifndef _BUZZER_H
#define _BUZZER_H

#define BUZZER_TIME_TOLERANCE   5

#define BUZZ_Prompt             0
#define BUZZ_TempReached        1
#define BUZZ_TimeOut            2
#define BUZZ_AddHop             3
#define BUZZ_End                4
#define BUZZ_Warn               5

const byte _sound_Prompt[] PROGMEM      = { 2, 2, 5 };
const byte _sound_TempReached[] PROGMEM = { 5,10, 3,10, 3,10};
const byte _sound_TimeOut[] PROGMEM     = {10, 1,39, 1,39, 1,39, 1,39,20,20 }; // 5 seconds
const byte _sound_AddHop[] PROGMEM      = {19,30,10,30,10,30,10,30,10,30,10,30,10,30,10,30,10,30,10,40}; // 10 seconds
const byte _sound_End[] PROGMEM         = { 2,40, 5 };
const byte _sound_Warn[] PROGMEM        = { 5, 4, 3, 4, 3, 4 };
const byte * const _sounds [] PROGMEM = {
  _sound_Prompt,
  _sound_TempReached,
  _sound_TimeOut,
  _sound_AddHop,
  _sound_End,
  _sound_Warn
};


byte _numberofNtesToPlay;
boolean _playing = false;
boolean _buzzing;
unsigned long _buzzingTime;
word _currentPeriod;
byte* _ptrCurrentNote;
byte* _currentSound;

void BuzzerMute(void);

void BuzzerInit() {
  pinMode (BuzzControlPin, OUTPUT);
  digitalWrite (BuzzControlPin, LOW);
}


void BuzzerThread() {
  if (! _playing)
    return;

  if ((gCurrentTimeInMS - _buzzingTime) >= (_currentPeriod + BUZZER_TIME_TOLERANCE)) {
    _numberofNtesToPlay --;
    if (_numberofNtesToPlay == 0) {
      // finished, stop
      digitalWrite (BuzzControlPin, LOW);
      _playing=false;
      return;
    }
    
    _buzzingTime = millis();
    _ptrCurrentNote ++; 
    _currentPeriod = (word)pgm_read_byte(_ptrCurrentNote) *25;

#if Silent != true
    if (_buzzing) {
      digitalWrite(BuzzControlPin, LOW);
    } else {
      digitalWrite(BuzzControlPin, HIGH);
    }
#endif
    _buzzing = ! _buzzing;
  }   
}


void BuzzerPlay(byte id) {
  
  _currentSound=(byte *)pgm_read_word(&(_sounds[id]));  
  _numberofNtesToPlay = pgm_read_byte(_currentSound);  
  _ptrCurrentNote =_currentSound;

  _ptrCurrentNote ++; 
  _currentPeriod = (word)pgm_read_byte(_ptrCurrentNote) *25;
  _playing=true;
  
  _buzzingTime = millis();
#if Silent != true
  digitalWrite (BuzzControlPin, HIGH);
#endif
  _buzzing=true;

#if DebugBuzzer == true
  Serial.print(F("BufferPlay("));
  Serial.print(id);
  Serial.print(F(") notes: "));
  Serial.println(_numberofNtesToPlay);
#endif
}



#endif
