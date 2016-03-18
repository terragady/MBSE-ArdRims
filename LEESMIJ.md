	MBSE-ArdRims
	------------

Arduino software voor een eenpans brouw systeem met wat extras. De code is
gebaseerd op Open ArdBir, maar het meeste is geheel opnieuw geschreven.
De recepten opslag is not uitwisselbaar met Ope-ArdBir door een bug in de
Open-ArdBir code. De meeste, maar niet alle EEPROM opslag instellingen zijn
uitwisselbaar zodat het in gebruik nemen van deze software eenvoudig is.
Maar, controleer alle instellingen wanneer je deze software installeerd of
als je andere software gaaat gebruiken!


Verschillen tussen andere software en MBSE-ArdRims:

1.  Alleen voor electrische systemen, ik heb geen plannen voor gas.
2.  Alleen 20x4 LCD displays.
3.  Tijdens de maisch stappen is de PID altijd in gebruik, er is geen vol
    gas methode als de temperatuur te ver af is.
4.  Logaritmische PWM vermogen regeling. Als de PID 1% vermogen wil, dan
    kan het effectieve vermogen groter zijn omdat je met 1% vermogen niets
    kan verwarmen. In de setup heet dit "Log Factor". Deze kan ingesteld worden
    tussen 0 (niets) en 20 (maximaal) extra vermogen. Hierdoor werkt de PID
    regeling een stuk beter. Het nut hangt sterk af van je installatie.
5.  Maisch stappen setup is vriendelijker. Eerst bepaal je of een bepaalde
    maisch stap wel of niet doorlopen moet worden, en als het "aan" is dan
    pas kun je de temperatuur en tijd instellen.
6.  Geen PID regeling tijdens koken. Verhitten naar de kook is altijd 100%.
    Als de kook temperatuur bereikt is kan dit minder zijn, dit is in te
    stellen. Als de temperatuur onder de kook temperatuur komt wordt weer
    100% gebruikt.
7.  Er zijn nu 4 Whirlpool fases mogelijk. De instellingen worden opgeslagen
    in het geheugen voor de recepten. Iedere whirlpool fase kan zijn:
    1. 88..100 Celcius, mininum temperatuur 93.
    2. 71..77 Celcius, minimum temperatuur 74.
    3. 60..66 Celcius, minimum temperatuur 63.
    4. Na de laatste koeling fase, geen minimum temperatuur.
8.  Een conditionele define in de sketch voor de display taal. Standaard is
    dit Engels, Nederlands kan aangezet worden.
9.  Een conditionele define om een extra SSR en DS18B20 sensor te gebruiken
    voor spoelwater. De verwarming gebeurt als er niet verwarmt wordt in de
    maisch ketel, dus het maximale stroomverbruik is die van de zwaarste ketel.
10. Verbeterde en meer robuuste uitlezing van de 1-wire sensoren.
11. Geen timer tijdens de handbediening, kijk zelf maar op de klok.
12. De code voor Automatic mode is nu State code. Dit scheelt geheugen en de
    code is beter te onderhouden.
13. De EEPROM opslag heeft een handtekening en versie markeringen. Hierdoor 
    worden upgrades naar nieuwere versies eenvoudiger. Omdat dit nu zo is,
    worden de recepten van andere systemen (Open-Ardbir oa) gewist! Deze
    waren verkeerd opgeslagen.
14. Alleen gewijzigde waarden worden in EEPROM geschreven (update mode), dit
    moet de levensduur van de EEPROM aanzienlijk verlengen.
15. Het recepten menu is gebruiksvriendelijker. Het onthoud nu het laatst
    gebruikte recept. Ook is er nu een aparte Save en Save-as keuze.
16. Alleen nog Celcius.
17. De koel temperatuur wordt opgeslagen als een Auto/Recept instelling.
18. Meer zuivere tijd tellers.

