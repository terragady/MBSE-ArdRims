#ifndef	TIMERS_H
#define	TIMERS_H


unsigned long TimeLeft;
unsigned long TimeSpent;
unsigned long _TimeStart;


/*
 * Should be called as much as possible
 */
void TimerRun() {

  gCurrentTimeInMS = millis();
  if ((gCurrentTimeInMS - _TimeStart) > 977) {  // 1 millis() is 1,024 mSec.
    _TimeStart += (gCurrentTimeInMS - _TimeStart);
    TimeSpent++;

    if (TimeLeft) {
      TimeLeft--;
      if (TimeLeft == 0)
        Buzzer(1, 500);
      else if (TimeLeft <= 10)
        Buzzer(1, 35);
      if ((TimeLeft % 60) == 0)
        pumpTime++;
    }    
  }
}


void TimerSet(unsigned long seconds) {
  TimeSpent = 0;
  TimeLeft = seconds;
  _TimeStart = millis();
}


void TimerShow(unsigned long Time, byte X, byte Y) {

  byte Hours   = (byte)(Time / 3600);
  byte Minutes = (byte)((Time % 3600) / 60);
  byte Seconds = (byte)(Time % 60);

  lcd.setCursor(X, Y);
  if (Hours < 10)
    lcd.print("0");
  lcd.print(Hours);

  lcd.print(F(":"));
  if (Minutes < 10)
    lcd.print("0");
  lcd.print(Minutes);

  lcd.print(F(":"));
  if (Seconds < 10)
    lcd.print("0");
  lcd.print(Seconds);
}


#endif
