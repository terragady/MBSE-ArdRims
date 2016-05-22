#ifndef SETUP_H
#define	SETUP_H


float _previousMashtemp;

void (*_displayFunc)(int);
void displayTime(int);
void displayYesNo(int);



void editByte(const char *label, int address, int max, int min, void (*displayFunc)(int)) {
  byte _editingValue = er_byte(address);
  _displayFunc = displayFunc;
  boolean editLoop = true;
  int labelLength = strlen(label);
  int offset = 0;

  if (labelLength < 16) {
    offset = 1;
  }

  lcd.setCursor(0, 2);
  Prompt(P2_clear);
  Prompt(P3_QQxO);
  lcd.setCursor(offset, 2);
  lcd.print(label);

  while (editLoop) {

    ReadButton(Direction, Timer);
    lcd.setCursor((labelLength+1+offset), 2);
    (*_displayFunc)(_editingValue);
    Set(_editingValue, max, min, 1, Timer, Direction);

    #if USE_HLT == true
      if (address == EM_TempHLT) {
        if ((Direction == DirectionDown) && (_editingValue < 75))
          _editingValue = 0;
        if ((Direction == DirectionUp) && (_editingValue == 1))
          _editingValue = 75;
      }
    #endif

    if (btn_Press(ButtonEnterPin, 50)) {
      editLoop = false;
    }
  }
  ew_byte(address, _editingValue);
  Prompt(P2_clear);
}



float editTemp(int address, float max, float min) {
  float   temperature = word(er_byte(address), er_byte(address + 1)) / 16.0;
  boolean editLoop = true;

  Prompt(P3_QQxO);

  while (editLoop) {

    ReadButton(Direction, Timer);
    lcd.setCursor(13, 2);
    lcd.print(temperature, 2);
    lcd.write((byte)0);
    Set(temperature, max, min, 0.25, Timer, Direction);

    if (btn_Press(ButtonEnterPin, 50)) {
      editLoop = false;
    }
  }

  int w_stagetempSet = word(temperature * 16);
  ew_byte(address, highByte(w_stagetempSet));
  ew_byte(address + 1, lowByte(w_stagetempSet));
  return temperature;
}



void editMash(byte index, float max, float min, boolean allowskip) {
  boolean editLoop = true;
  byte    tsave    = er_byte(EM_StageTime(index));
  int     time     = tsave;

  lcd.setCursor(0, 2);
  Prompt(P2_clear);
  lcd.setCursor(1, 2);
  lcd.print(stageName[index]);

  if (allowskip) {
    /*
     * First enable or disable this step.
     */
    Prompt(P3_xxSO);
    byte tsave = time;
    while (editLoop) {
      lcd.setCursor(13, 2);
#if langNL == true
      (tsave) ? lcd.print(F("    Ja")) : lcd.print(F("   Nee"));
#else
      (tsave) ? lcd.print(F("   Run")) : lcd.print(F("  Skip"));
#endif
      if (btn_Press(ButtonStartPin, 50)) {
        if (tsave)
          tsave = 0;
        else
          tsave = 1;
      }
      if (btn_Press(ButtonEnterPin, 50)) {
        editLoop = false;
      }
    }
    if (tsave == 0)
      time = 0;
    else if (time == 0)
      time = 1;
  } else {
    if (time == 0)
      time = 5;           // force 5 minutes
  }

  if (time) {             // enabled step
    _previousMashtemp = editTemp(EM_StageTemp(index), max, min);

    if (index == 0) {
      editLoop = false;   // Fixed Mash-in step time
      time = 5;           // 5 minutes
    } else if (time == 0) {
      editLoop = false;   // Skipped step
    } else {
      editLoop = true;    // Enables step, edit steptime
    }

    while (editLoop) {

      ReadButton(Direction, Timer);
      lcd.setCursor(13, 2);
      lcd.print("      ");
      lcd.setCursor(13, 2);
      displayTime(time);
      Serial.println(time);
      Set(time, 140, 1, 1, Timer, Direction);

      if (btn_Press(ButtonEnterPin, 50)) {
        editLoop = false;
      }
    }
  }

  ew_byte(EM_StageTime(index), time);
  Prompt(P2_clear);
}


void displayOffset100(int data) {
  float fvalue = (float)data - 100.0;

  FormatNumber(fvalue, 1);
  lcd.print(fvalue, 0);
}


void displayMultiply250(int data) {
  float fvalue = (float)data * 250.0;

  FormatNumber(fvalue, 1);
  lcd.print(fvalue, 0);
}



void displayKi(int data) {
  float fvalue = (float)data / 1000.0;

  FormatNumber(fvalue, -3);
  lcd.print(fvalue, 3);
}



void displayPercentage(int data) {
  float fvalue = (float)data;

  //FormatNumber(fvalue, 0);
  lcd.print(" ");
  lcd.print(fvalue, 0);
  lcd.print(F("%"));
}


void displayTempShift50Divide10(int data) {
  float fvalue = ((float)data - 50.0) / 10.0;

  FormatNumber(fvalue, -2);
  lcd.print(fvalue, 1);
  lcd.write((byte)0);
}


void displaySimpleTemperature(int data) {
  float fvalue = (float)data;

  //FormatNumber(fvalue, 0);
  //lcd.print(" ");
  lcd.print(fvalue, 0);
  lcd.write((byte)0);
}


void displayTime(int data) {
  float fvalue = (float)data;

  //FormatNumber(fvalue, 1);
  //lcd.print(" ");
  lcd.print(fvalue, 0);
  lcd.print(F("m"));
}


void displayTimeOff(int data) {
  if (data)
    displayTime(data);
  else
#if langNL == true
    lcd.print(F("UIT"));
#else
    lcd.print(F("OFF"));
#endif
}


void displayNumber(int data) {
  float fvalue = (float)data;

  //FormatNumber(fvalue, 1);
  lcd.print(fvalue, 0);
}


void displayOnOff(int value) {
#if langNL == true
  value ? lcd.print(F("AAN")) : lcd.print(F("UIT"));
#else
  value ? lcd.print(F("ON")) : lcd.print(F("OFF"));
#endif
}


void displayYesNo(int value) {
#if langNL == true
  value ? lcd.print(F("JA ")) : lcd.print(F("NEE"));
#else
  value ? lcd.print(F("YES")) : lcd.print(F("NO "));
#endif
}


void set_PID(void) {
#if langNL == true
    editByte("Constant kP ", EM_PID_kP, 200, 100, & displayOffset100);
    editByte("Constant kI ", EM_PID_kI, 250,   0, & displayKi);
    editByte("Constant kD ", EM_PID_kD, 200, 100, & displayOffset100);
    editByte("SampleTime  ", EM_SampleTime, 3500 / 250, 1000 / 250, & displayMultiply250);
    editByte("WindowSet ms", EM_WindowSize, 7500 / 250, 3000 / 250, & displayMultiply250);
    editByte("Log Factor  ", EM_LogFactor, 20, 0, & displayNumber);
    editByte("Temp Offset ", EM_TempOffset, 100, 0, & displayTempShift50Divide10);
    editByte("Heat in Boil", EM_BoilHeat, 100, 20, & displayPercentage);
  #if USE_HLT == true
    editByte("HLT temp.   ", EM_TempHLT, 95, 0, & displaySimpleTemperature);
  #endif
#else
    editByte("Constant kP ", EM_PID_kP, 200, 100, & displayOffset100);
    editByte("Constant kI ", EM_PID_kI, 250,   0, & displayKi);
    editByte("Constant kD ", EM_PID_kD, 200, 100, & displayOffset100);
    editByte("SampleTime  ", EM_SampleTime, 3500 / 250, 1000 / 250, & displayMultiply250);
    editByte("WindowSet ms", EM_WindowSize, 7500 / 250, 3000 / 250, & displayMultiply250);
    editByte("Log Factor  ", EM_LogFactor, 20, 0, & displayNumber);
    editByte("Temp Offset ", EM_TempOffset, 100, 0, & displayTempShift50Divide10);
    editByte("Heat in Boil", EM_BoilHeat, 100, 20, & displayPercentage);
  #if USE_HLT == true
    editByte("HLT temp.   ", EM_TempHLT, 95, 0, & displaySimpleTemperature);
  #endif
#endif
}


void set_Unit(void) {
  //        23456789012
#if langNL == true
  editByte("Kook Temperatuur" , EM_BoilTemperature, 105, 60, & displaySimpleTemperature);
  editByte("Pomp Cyclus"      , EM_PumpCycle, 15, 5, & displayTime);
  editByte("Pomp Rust"        , EM_PumpRest, 5, 0, & displayTime);
  editByte("Pomp Inmaischen"  , EM_PumpPreMash, 1, 0, & displayOnOff);
  editByte("Pomp Maischen"    , EM_PumpOnMash, 1, 0, & displayOnOff);
  editByte("Pomp Uitmaischen" , EM_PumpMashout, 1, 0, & displayOnOff);
  editByte("Pomp Koken"       , EM_PumpOnBoil, 1, 0, & displayOnOff);
  editByte("Pomp Stop"        , EM_PumpMaxTemp, 105, 80, & displaySimpleTemperature);
  editByte("PID Pijp"         , EM_PIDPipe, 1, 0, & displayYesNo);
  editByte("Skip Toevoegen"   , EM_SkipAdd, 1, 0, & displayYesNo);
  editByte("Skip Verwijderen" , EM_SkipRemove, 1, 0, & displayYesNo);
  editByte("Skip Jodiumtest"  , EM_SkipIodine, 1, 0, & displayYesNo);
  editByte("Jodium Tijd"      , EM_IodoneTime, 90, 0, & displayTime);
#else
  editByte("Boil Temperature" , EM_BoilTemperature, 105, 90, & displaySimpleTemperature);
  editByte("Pump Cycle"       , EM_PumpCycle, 15, 5, & displayTime);
  editByte("Pump Rest"        , EM_PumpRest, 5, 0, & displayTime);
  editByte("Pump Pre Mash"    , EM_PumpPreMash, 1, 0, & displayOnOff);
  editByte("Pump On Mash"     , EM_PumpOnMash, 1, 0, & displayOnOff);
  editByte("Pump Mash Out"    , EM_PumpMashout, 1, 0, & displayOnOff);
  editByte("Pump on Boil"     , EM_PumpOnBoil, 1, 0, & displayOnOff);
  editByte("Pump Stop"        , EM_PumpMaxTemp, 105, 80, & displaySimpleTemperature);
  editByte("PID Pipe"         , EM_PIDPipe, 1, 0, & displayYesNo);
  editByte("Skip Add"         , EM_SkipAdd, 1, 0, & displayYesNo);
  editByte("Skip Remove"      , EM_SkipRemove, 1, 0, & displayYesNo);
  editByte("Skip Iodine"      , EM_SkipIodine, 1, 0, & displayYesNo);
  editByte("Iodine Time"      , EM_IodoneTime, 90, 0, & displayTime);
#endif
}


void set_Auto_Mash(void) {
  /*
   * Edit mash steps, make sure the temperature goes up.
   */
  editMash(0, 75.0, 20.0, false);                     // Mash-in
  editMash(1, 75.0, _previousMashtemp - 5.0, false);  // Step 1 (Mandatory)
  editMash(2, 75.0, _previousMashtemp + 1.0, true);   // Step 2
  editMash(3, 75.0, _previousMashtemp + 1.0, true);   // Step 3
  editMash(4, 75.0, _previousMashtemp + 1.0, true);   // Step 4
  editMash(5, 75.0, _previousMashtemp + 1.0, true);   // Step 5
  editMash(6, 75.0, _previousMashtemp + 1.0, true);   // Step 6
  editMash(7, 85.0, _previousMashtemp      , false);  // Mash-out
}


void set_Auto_Boil(void) {
  int   i;
#if langNL == true
  char  *Hopstr = (char *)"Hopgift .. ";
#else
  char  *Hopstr = (char *)"Hop Add .. ";
#endif

  /*
     * Set Boiltime and hop additions.
   */
  editByte(stageName[8], EM_BoilTime, 240, 10, & displayTime);
#if langNL == true
  editByte(" Aantal Hopgiften", EM_NumberOfHops, 10, 0, & displayNumber);
#else
  editByte(" Nr. Of Hops adds", EM_NumberOfHops, 10, 0, & displayNumber);
#endif

  if (er_byte(EM_NumberOfHops)) {
    for (i = 0; i < er_byte(EM_NumberOfHops); i++) {
      Hopstr[8] = ((i + 1) / 10) + '0';
      Hopstr[9] = ((i + 1) % 10) + '0';
      editByte(Hopstr, EM_TimeOfHop(i), er_byte(EM_TimeOfHop(i) - 1), 0, & displayTime);
    }
  }

  // Cooling Temperature
  Prompt(P2_clear);
  lcd.setCursor(1, 2);
#if langNL == true
  lcd.print(F("Koel Temp."));
#else
  lcd.print(F("Cool Temp."));
#endif
  editTemp(EM_CoolingTemp, 30.0, 10.0);

  // 4 Whirlpool settings
#if langNL == true
  editByte("Whirlpool 93+", EM_Whirlpool_9 , WhirlpoolMaxtime, 0, & displayTimeOff);
  editByte("Whirlpool 71-77", EM_Whirlpool_7 , WhirlpoolMaxtime, 0, & displayTimeOff);
  editByte("Whirlpool 60-66", EM_Whirlpool_6 , WhirlpoolMaxtime, 0, & displayTimeOff);
  editByte("Whirlpool Koud", EM_Whirlpool_2 , WhirlpoolMaxtime, 0, & displayTimeOff);
#else
  editByte("Whirlpool 93+", EM_Whirlpool_9 , WhirlpoolMaxtime, 0, & displayTimeOff);
  editByte("Whirlpool 71-77", EM_Whirlpool_7 , WhirlpoolMaxtime, 0, & displayTimeOff);
  editByte("Whirlpool 60-66", EM_Whirlpool_6 , WhirlpoolMaxtime, 0, & displayTimeOff);
  editByte("Whirlpool Cold", EM_Whirlpool_2 , WhirlpoolMaxtime, 0, & displayTimeOff);
#endif
}



byte SelectRecipe(byte & numRecipe, byte Direction) {
  if (er_byte(EM_RecipeIndex(numRecipe - 1)) == 0) {
    boolean Control = true;

    while (Control) {
      if (Direction == DirectionUp) {
        if (numRecipe < 10)
          numRecipe++;
        else
          Control = false;
      }

      if (Direction == DirectionDown) {
        if (numRecipe > 1)
          numRecipe--;
        else
          Control = false;
      }

      if (er_byte(EM_RecipeIndex(numRecipe - 1)) == 1)
        Control = false;
    }
  }
}



void NoRecipe() {
  Prompt(P2_clear);
  Prompt(P2_norecipe);
  Buzzer(3, 50);
  delay(2500);
}



/*
 * Find lowest and highest valid recipe numbers.
 * Returns true  - no recipes
 *         false - recipes found.
 */
boolean RecipeStart(byte & RecipeUp, byte & RecipeDown, byte & numRecipe) {
  byte i;

  for (i = EM_RecipeIndexBase; i < (EM_RecipeIndexBase + 10); i++) {
    if (er_byte(i) == 1) {
      RecipeUp = (i - (EM_RecipeIndexBase - 1));
      if (RecipeDown == 0)
        RecipeDown = RecipeUp;
    }
  }
  if (RecipeUp == 0) {
    NoRecipe();
    return true;
  }
  numRecipe = RecipeDown;
  return false;
}



/*
 * Type 0 = Load, Save, Delete
 *      1 = SaveAs
 */
void Recipe(byte numRecipe, byte Type) {
  (Type == 0) ? Prompt(P3_SGQO) : Prompt(P3_QQBO);

  lcd.setCursor(2, 2);
  if (numRecipe < 10)
    lcd.print(F("0"));
  lcd.print(numRecipe);
  lcd.print(F(" > "));

  if (Type == 0) {
    for (byte pos = 0; pos < 10; pos++) {
      lcd.print((char)EEPROM.read(EM_RecipeName(numRecipe - 1) + pos));
    }
  }
}



void RecipeSelect(byte & numRecipe, byte RecipeUp, byte RecipeDown) {
  Recipe(numRecipe, 0);
  ReadButton(Direction, Timer);
  Set(numRecipe, RecipeUp, RecipeDown, 1, Timer, Direction);
}



/*
 * Recipe confirm prompt
 */
boolean RecipePrompt(const char *label, byte numRecipe) {
  lcd.setCursor(1, 2);
  lcd.print(label);
  lcd.print(F(" "));

  if (numRecipe < 10)
    lcd.print(F("0"));
  lcd.print(numRecipe);
  lcd.print(F("?"));
  return (! WaitForConfirm(2, false, 0, 0, 0, P3_proceed));
}


/*
 * Display final recipe action
 */
void RecipeDoing(const char *label) {
  Prompt(P2_clear);
  Prompt(P3_clear);
  lcd.setCursor(0, 3);
  lcd.print(label);
  delay(2500);
  Buzzer(1, 20);
}



void RecipeLoad(void) {
  byte    numRecipe = 0;
  byte    RecipeUp = 0;
  byte    RecipeDown = 0;
  byte    i;

  if (RecipeStart(RecipeUp, RecipeDown, numRecipe))
    return;
  // Prefer last used recipe
  i = er_byte(EM_ActiveRecipe);
  if (i >= 1 && i <= 10)
    numRecipe = i;

  while (true) {
    RecipeSelect(numRecipe, RecipeUp, RecipeDown);

    if (btn_Press(ButtonStartPin, 50)) { // Quit
      return;
    }
    if (btn_Press(ButtonEnterPin, 50)) { // Ok
      Prompt(P1_clear);

      int Da = EM_RecipeData(numRecipe - 1);
      for (i = EM_RunBase; i < (EM_RunBase + 52); i++) {
        ew_byte(i, EEPROM.read(Da));
        Da++;
      }
      ew_byte(EM_ActiveRecipe, numRecipe);

#if langNL == true
      RecipeDoing("  Recept Laden...");
#else
      RecipeDoing("   Recipe Load...");
#endif
      return;
    }
  }
}



void RecipeSave(void) {
  byte    numRecipe = 0;
  byte    RecipeUp = 0;
  byte    RecipeDown = 0;
  byte    i;

  if (RecipeStart(RecipeUp, RecipeDown, numRecipe))
    return;

  // Prefer the recipe number previous loaded.
  i = er_byte(EM_ActiveRecipe);
  if (i >= 1 && i <= 10)
    numRecipe = i;

  while (true) {
    RecipeSelect(numRecipe, RecipeUp, RecipeDown);

    if (btn_Press(ButtonStartPin, 50)) { // Quit
      return;
    }
    if (btn_Press(ButtonEnterPin, 50)) { // Ok
#if langNL == true
      if (RecipePrompt("Opslaan Recept", numRecipe)) {
#else
      if (RecipePrompt("  Save Recipe", numRecipe)) {
#endif
        return;
      }

      // Save data
      int Da = EM_RecipeData(numRecipe - 1);
      for (byte j = EM_RunBase; j < (EM_RunBase + 52); j++) {
        ew_byte(Da, EEPROM.read(j));
        Da++;
      }
      ew_byte(EM_ActiveRecipe, numRecipe);

#if langNL == true
      RecipeDoing("     Opslaan...");
#else
      RecipeDoing("      Saving...");
#endif
      return;
    }
  }
}



void RecipeSaveAs(void) {
  byte numRecipe = 0;

  /*
   * Search empty slot
   */
  for (byte i = EM_RecipeIndexBase; i < (EM_RecipeIndexBase + 10); i++) {
    if (EEPROM.read(i) == 0) {
      numRecipe = (i - (EM_RecipeIndexBase - 1));
      i = 99;
    }
  }

  if (numRecipe == 0) {
    Prompt(P3_clear);
    lcd.setCursor(1, 2);
#if langNL == true
    lcd.print(F("     ATTENTIE     "));
#else
    lcd.print(F("     ATTENTION    "));
#endif
    Buzzer(3, 125);
    lcd.setCursor(1, 3);
#if langNL == true
    lcd.print(F("  GEHEUGEN IS VOL "));
#else
    lcd.print(F("  MEMORY IS FULL  "));
#endif
    delay(2500);
    return;
  }

  byte NameRecipe[10];
  byte pos = 0;
  NameRecipe[pos] = 97;

  Prompt(P2_clear);
  Recipe(numRecipe, 1);

  // Spc  32
  // 0-9  48 to  57
  // A-Z  65 to  90
  // a-z  97 to 122
  while (pos < 10) {
    lcd.setCursor(pos + 7, 2);
    lcd.print((char)NameRecipe[pos]);
    lcd.blink();

    ReadButton(Direction, Timer);
    Set(NameRecipe[pos], 122, 32, 1, Timer, Direction);

    if ((NameRecipe[pos] > 32 && NameRecipe[pos] < 48) && Direction == DirectionUp)
      NameRecipe[pos] = 48;
    if ((NameRecipe[pos] > 57 && NameRecipe[pos] < 97) && Direction == DirectionUp)
      NameRecipe[pos] = 97;

    if ((NameRecipe[pos] < 97 && NameRecipe[pos] > 57) && Direction == DirectionDown)
      NameRecipe[pos] = 57;
    if ( NameRecipe[pos] < 48 && Direction == DirectionDown)
      NameRecipe[pos] = 32;

    if (btn_Press(ButtonEnterPin, 50)) {
      pos++;
      NameRecipe[pos] = 97;
    }

    if ((digitalRead(ButtonDownPin) == 0) && (digitalRead(ButtonUpPin) == 0)) {
      delay(350);
      if ((digitalRead(ButtonDownPin) == 0) && (digitalRead(ButtonUpPin) == 0)) {
        for (byte j = pos; j < 10; j++) {
          NameRecipe[pos] = 32;
          pos++;
        } pos = 9;
      }
    }

    // Backspace (sort of).
    if (btn_Press(ButtonStartPin, 50)) {
      if (pos > 0)
        pos--;
    }
  }

  lcd.noBlink();
#if langNL == true
      if (RecipePrompt("Opslaan recept", numRecipe)) {
#else
      if (RecipePrompt("  Save recipe", numRecipe)) {
#endif
    return;
  }

  // Save data
  int Da = EM_RecipeData(numRecipe - 1);
  for (byte j = EM_RunBase; j < (EM_RunBase + 52); j++) {
    ew_byte(Da, EEPROM.read(j));
    Da++;
  }

  // Save name
  for (pos = 0; pos < 10; pos++) {
    ew_byte(EM_RecipeName(numRecipe - 1) + pos, NameRecipe[pos]);
  }

  // Save entry
  ew_byte(EM_RecipeIndex(numRecipe - 1), 1);
  ew_byte(EM_ActiveRecipe, numRecipe);

#if langNL == true
  RecipeDoing("     Opslaan...");
#else
  RecipeDoing("      Saving...");
#endif
}



void RecipeDelete(void) {
  byte    numRecipe = 0;
  byte    RecipeUp = 0;
  byte    RecipeDown = 0;
  byte    i;

  if (RecipeStart(RecipeUp, RecipeDown, numRecipe))
    return;

  while (true) {
    RecipeSelect(numRecipe, RecipeUp, RecipeDown);

    if (btn_Press(ButtonStartPin, 50)) { // Quit
      return;
    }
    if (btn_Press(ButtonEnterPin, 50)) { // Ok
#if langNL == true
      if (RecipePrompt("Wissen Recept", numRecipe)) {
#else
      if (RecipePrompt(" Erase Recipe", numRecipe)) {
#endif
        return;
      }

#if langNL == true
      RecipeDoing("   Verwijderen...");
#else
      RecipeDoing("     Erasing...");
#endif
      ew_byte(EM_RecipeIndex(numRecipe - 1), 0);
      return;
    }
  }
}



/*
 * Initialze Recipes storage memory
 */
void InitRecipes(void) {
  byte  i, j;

  if (! WaitForConfirm(2, false, 0, 0, 0, P3_proceed)) {
    return;
  }

  for (i = 0; i < 10; i++) {
    ew_byte(EM_RecipeIndex(i), 0);
    for (j = 0; j < 10; j++)
      ew_byte(EM_RecipeName(i) + j, 32);
    for (j = 0; j < 52; j++)
      ew_byte(EM_RecipeData(i) + j, 0);
  }

#if langNL == true
  RecipeDoing("   EEPROM Wissen");
#else
  RecipeDoing("   EEPROM Format");
#endif
}



void set_Recipes() {
  byte recipeMenu = 0;
  Prompt(P2_clear);

  while (true) {

    lcd.setCursor(1, 2);
    switch (recipeMenu) {
      case 0:
#if langNL == true
        lcd.print(F("   Recept Laden   "));
#else
        lcd.print(F("   Recept Load    "));
#endif
        Prompt(P3_xGQO);
        if (btn_Press(ButtonDownPin, 50))
          recipeMenu = 1;
        if (btn_Press(ButtonEnterPin, 50))
          RecipeLoad();
        break;

      case 1:
#if langNL == true
        lcd.print(F("  Recept Opslaan  "));
#else
        lcd.print(F("   Recipe Save    "));
#endif
        Prompt(P3_SGQO);
        if (btn_Press(ButtonDownPin, 50))
          recipeMenu = 2;
        if (btn_Press(ButtonUpPin, 50))
          recipeMenu = 0;
        if (btn_Press(ButtonEnterPin, 50))
          RecipeSave();
        break;

      case 2:
#if langNL == true
        lcd.print(F("Recept Opslaan als"));
#else
        lcd.print(F("  Recipe Save as  "));
#endif
        Prompt(P3_SGQO);
        if (btn_Press(ButtonDownPin, 50))
          recipeMenu = 3;
        if (btn_Press(ButtonUpPin, 50))
          recipeMenu = 1;
        if (btn_Press(ButtonEnterPin, 50))
          RecipeSaveAs();
        break;

      case 3:
#if langNL == true
        lcd.print(F("  Recept Wissen   "));
#else
        lcd.print(F("   Recipe Erase   "));
#endif
        Prompt(P3_SGQO);
        if (btn_Press(ButtonDownPin, 50))
          recipeMenu = 4;
        if (btn_Press(ButtonUpPin, 50))
          recipeMenu = 2;
        if (btn_Press(ButtonEnterPin, 50))
          RecipeDelete();
        break;

      case 4:
#if langNL == true
        lcd.print(F("  Initialiseren   "));
#else
        lcd.print(F("   Initialise     "));
#endif
        Prompt(P3_SxQO);
        if (btn_Press(ButtonUpPin, 50))
          recipeMenu = 3;
        if (btn_Press(ButtonEnterPin, 50))
          InitRecipes();
        break;
    }
    if (btn_Press(ButtonStartPin, 50)) {
      lcd.clear();
      Prompt(P0_banner);
      return;
    }
  }
}



void setup_mode() {
  byte setupMenu = 0;

  while (true) {
    Prompt(P0_setup);
    Prompt(P2_clear);
    lcd.setCursor(1, 1);

    switch (setupMenu) {

      case (0):
#if langNL == true
        lcd.print(F(" PID Instellingen "));
#else
        lcd.print(F("  PID Parameters  "));
#endif
        Prompt(P3_xGQO);
        if (btn_Press(ButtonDownPin, 50))
          setupMenu = 1;
        if (btn_Press(ButtonEnterPin, 50))
          set_PID();
        break;

      case (1):
#if langNL == true
        lcd.print(F("Unit Instellingen "));
#else
        lcd.print(F(" Unit Parameters  "));
#endif
        Prompt(P3_SGQO);
        if (btn_Press(ButtonUpPin, 50))
          setupMenu = 0;
        if (btn_Press(ButtonDownPin, 50))
          setupMenu = 2;
        if (btn_Press(ButtonEnterPin, 50))
          set_Unit();
        break;

      case (2):
#if langNL == true
        lcd.print(F(" Maisch Programma "));
#else
        lcd.print(F(" Mash Automation  "));
#endif
        Prompt(P3_SGQO);
        if (btn_Press(ButtonUpPin, 50))
          setupMenu = 1;
        if (btn_Press(ButtonDownPin, 50))
          setupMenu = 3;
        if (btn_Press(ButtonEnterPin, 50))
          set_Auto_Mash();
        break;

      case (3):
#if langNL == true
        lcd.print(F("  Kook Programma  "));
#else
        lcd.print(F(" Boil Automation  "));
#endif
        Prompt(P3_SGQO);
        if (btn_Press(ButtonUpPin, 50))
          setupMenu = 2;
        if (btn_Press(ButtonDownPin, 50))
          setupMenu = 4;
        if (btn_Press(ButtonEnterPin, 50))
          set_Auto_Boil();
        break;

      case (4):
#if langNL == true
        lcd.print(F(" Recepten Beheer  "));
#else
        lcd.print(F("Recipe Management "));
#endif
        Prompt(P3_SxQO);
        if (btn_Press(ButtonUpPin, 50))
          setupMenu = 3;
        if (btn_Press(ButtonEnterPin, 50))
          set_Recipes();
        break;
    }
    if (btn_Press(ButtonStartPin, 50)) {
      lcd.clear();
      return;
    }
  }
}


#endif
