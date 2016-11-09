#ifndef	TIMERS_H
#define	TIMERS_H


unsigned long TimeLeft;
unsigned long TimeSpent;
unsigned long Steady;


/*
 * Should be called as much as possible
 */
void TimerRun() {

  gCurrentTimeInMS = millis();
  
  if (_seconds != myTimer.readTimer()) {
    _seconds = myTimer.readTimer();

    TimeSpent++;
    Steady++;

    if (TimeLeft) {
      TimeLeft--;
      if (TimeLeft == 5)
        BuzzerPlay(BUZZ_TimeOut);
      if ((TimeLeft % 60) == 0)
        pumpTime++;
    }    
  }

}


void TimerSet(unsigned long seconds) {
  Steady = TimeSpent = 0;
  TimeLeft = seconds;
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
