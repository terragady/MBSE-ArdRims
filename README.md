	MBSE-ArdRims
	------------

Arduino software for a single vessel brewsystem with some extra's. The code
is based on Open ArdBir, but the most of it is rewritten from scratch.
Recipe storage is incompatible with Open-ArdBir because of a bug in the
Open ArdBir code. Most, but not all of the EEPROM storage settings are
compatible, so upgrading to this package should be easy. But still, check
all settings before and after changing this software.


Differences between other systems and MBSE-ArdRims:

1.  Electric systems only, I have no plans to support Gas.
2.  20x4 displays only.
3.  During Mash stages, the PID is allways active, no override to full power
    when temperature is too far off.
4.  Logarithmic PWM power regulation. If the PID wants 1% power, real effective
    power can be set higher because 1% electric power does nothing. In the setup
    this is called "Log Factor". Can be set between 0 (nothing) and 20 (most)
    extra power boost. Because of this, PID regulation works much better.
5.  Mash steps setup is more user friendly. You first set steps that can be
    skipped on or off, and only if "on" you can set the parameters.
6.  No PID control during boil, heatup is always 100%, and when the boil temp
    is reached, a lower percentage can be used. If the temperature drops below
    the boil temp it goes immediatly back to 100% power.
7.  There are now 4 whirlpool stages possible. Settings are stored within
    the recipes. Each whirlpool can be timed active or off.
    1. 88..100 Celcius, holding at 93.
    2. 71..77 Celcius, holding at 74.
    3. 60..66 Celcius, holding at 63.
    4. After final cooling, no hold.
8.  Conditional defines to select the display language, English is the default
    and the Dutch language is selected using a define.
9.  Conditional defines to add a SSR and a DS18B20 sensor on a separate one-
    wire bus to control a Hot Liquer Tank. Heating is timeshared with the
    normal Mash heater.
10. Improved and more robust 1-wire bus reads for the sensor(s).
11. No timer control in manual mode. Use the wall clock.
12. Automatic mode is now state controlled. This saves memory and the code is
    better readable.
13. EEPROM storage has a signature and revision marks, so upgrades to any new
    versions should be smooth. This also means that old recipe storage from
    Open-ArdBir (and others) is erased because it was not stored correctly.
14. Writing to EEPROM only writes if the data is changed to save EEPROM life
    time. (Update mode).
15. The recipe menu is more friendly. It remembers the last used entry and
    there is now a separate Save and a Save-as choice.
16. Only Celcius now.
17. The cooling destination temperature can be set as a Auto/Recipe setting.
18. Better timing using the interrupt driven secTimer library. This works
    much better then the old millis() function.


Next to the standard Arduino libraries you need to install the following extra
libraries:

    OneWire
    PID
    secTimer

In the menu: Sketch -> Include Library -> Manage Libraries.
In the new windows you can search and install these libraries.

