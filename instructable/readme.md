# Instructable

Een instructable is een stappenplan - zonder verdere uitleg - hoe je vertrekkend van de bill of materials en gebruik makend van de technische tekeningen de robot kan nabouwen. Ook de nodige stappen om de microcontroller te compileren en te uploaden staan beschreven.  

### stap 1
bestel alle componenten uit de bill of materials  

### stap 2 
ontwerp een elektronisch schema voor alle bestelde componenten. En bekijk datasheets hoe je deze correct aansluit en gebruikt.

### stap 3
Wanneer alle componenten zijn toegekomen. Kan je deze 1 voor 1 testen. Dit door eenvoudige schakelingen en programma's te schrijven.
Je hebt een proof of concept nodig voor:
- Werking microcontroller;
- H-brug en motoren aansturen;
- interrupt via een drukknop;
- Uitlezen sensor waarde;
- draadloze communicatie.

### Stap 4
Ontwerp een elektronisch schema voor de hele linefollower. 

### Stap 5
Wanneer het elektronisch schema getekent is kan er gewerkt worden aan plan B. 
Plan B ga je de linefollower maken op een breadbord. Hierbij kan getest worden of alle componenten goed hun werk doen. 
Je kan bv. gebruik maken van een plankje om alles op te bevestigen. 

### Stap 6 
Wanneer de constructie klaar is kan je werken aan het basis programma. Hierbij gaan we alle proof of concepts combineren tot 1 groot geheel. 

### Stap 7
sensor afstellen
Het resultaat van de proof of concept voor de sensor array zijn de individuele waardes van de sensoren, en zou er als volgt kunnen uitzien: 6 (of meer, afhankelijk van het aantal sensoren in de array) metingen.

Kalibratie
Ruwe sensor gegevens rechtstreeks met elkaar vergelijken is vrij onnauwkeurig. Wanneer je dit doet ga je er van uit dat alle componenten identiek zijn. Dit is natuurlijk niet zo, iedere component (led, transistor of diode, voorschakel weerstanden) is gemaakt binnen bepaalde toleranties. Deze toleranties zorgen ervoor dat de verschillende individuele sensoren verschillende meetresultaten zullen opleveren, zelfs wanneer de fysieke te meten waardes identiek zijn (in dit geval het terug gekaatste licht).

Om deze meetfouten ietwat uit te middelen kunnen we onze individuele sensoren kalibreren. Dit is eigenlijk niet meer dan de sensoren uitlezen onder gekende omstandigheden (de sensoren uitlezen wanneer we weten dat deze "wit" en "zwart" moeten meten). Op die manier hebben we voor iedere sensor referentiewaardes waarmee we kunnen vergelijken.

Ga als volgt te werk:

- definieer een array voor de kalibratie zwart waardes in de EEPROM structuur
- defineer een array voor de kalibratie wit waardes in de EEPROM structuur
- maak een nieuwe SerialCommand eventhandler aan voor het wit/zwart kalibreren van de sensoren
- lees in deze eventhandler de sensorwaardes uit (eventueel meerdere keren) en schrijf de meetresultaten naar de juiste array. Vergeet de resultaten niet op te slaan     in het EEPROM geheugen.

### Stap 8 
Normalisatie
Uit de kalibratie blijkt dat de zwart- en witwaardes voor de verschillende individuele sensoren niet gelijk is. Een direct gevolg is dat we ook de uitgelezen waardes van de individuele sensoren eigenlijk niet rechtstreeks met elkaar mogen vergelijken.

We moeten de sensor gegevens eerst normaliseren. 

Met behulp van de map functie kunnen we een sensorwaarde zo gaan herschalen dat de calibratiewaardes van die sensor overeenkomen met een gekozen minimum en maximum (in onderstaande code 0 en 1000). Aangezien iedere sensor zijn eigen calibratiewaardes heeft zal deze herschaling dus ook voor iedere sensor uniek zijn. Na herschaling heeft iedere sensor een zelfde minimum en maximum waarde (+ bijhorende herschaalde meetwaarde) en mogen de verschillende sensorwaardes met elkaar vergeleken worden.

controleer:
de meest linkse sensor moet overeen komen met normalised[0], de meest rechtse sensor moet overeenkomen met de laatste waarde in die array.
sensor waardes zouden de onder- en bovengrens van de mapping functie niet mogen overschrijden. Doen ze dit toch, dan is dit een reden om te herkalibreren.

### Stap 9
Interpolatie

kwadratische interpolatie
kwadratische interpolatie vertrekt van het idee dat we door 3 punten exact één parabool (= kwadratische vergelijking) kunnen tekenen. Indien we voor deze 3 punten de zwartste sensor en beide omliggende sensoren nemen, dan moet het dal van die parabool de positie van de lijn onder onze sensoren zijn.

ga als volgt te werk:
herschaal de genormaliseerde waardes indien nodig zodat zwart de grootste waardes geeft
bereken de som van de sensorwaardes vermenigvuldigd met hun positie
deel het resultaat door de som van de sensorwaardes

### Stap 10
Wanneer het basis programma af is kan je er voor zorgen dat de linefollower effectief de lijn zal volgen. 
Door middel van de parameters power en diff kunnen we kijken of motoren voorwaarts draaien. Het is belangerijk hiervoor de waarde KP gelijk aan nul te stellen. 
Dat wil zeggen dat we het signaal van de sensor compleet negeren. 
Ga als volgt te werk:
- geef de parameter power een waarde tussen 0 en 255, bvb. 100.
- geef de parameter diff een waarde tussen 0 en 1, bvb. 0.5
- geef de parameter Kp de waarde 0. 
- start de robot.

Draait de motor achteruit dan moet je je pinnummers aanpassen. 

### Stap 11
Nu we weten dat onze wielen in de juiste richting draaien kunnen we een waarde toekennen aan Kp.
- geef de parameter Kp een waarde groter dan 0, bvb. 1
- start de robot, plaats deze op een parcours
Je zal zien dat de robot de lijn zal volgen of toch zal proberen. 
Indien je niet het gewenste resultaat krijgt kan je proberen de waarde kp stuk voor stuk te verhogen. 
Je kan missschien de waarde diff en power ook aanpassen. 

Door deze 3 parameters juist te kiezen zal het mogelijk zijn de robot 1, 3 of meer toeren te laten rijden.

mogelijke problemen kunnen optreden:
- rijdt de robot te traag / valt de robot stil => verhoog de parameter power
- stuurt de robot niet snel genoeg bij => verhoog Kp
- waggelt de robot over de zwarte lijn => verlaag Kp
- versnelt de robot in de bochten => verlaag diff
- vertraagt de robot in de bochten / valt de robot stil in de bochten => verhoog diff

### Stap 12
Integrerend Regelen
Bij integrerend regelen gaan we kijken hoe lang de fout zich blijft voordoen. Hoe langer de fout zich voordoet, hoe sterker we gaan bijregelen. Simpel gezegd tellen we gewoon alle fouten op. Hoe langer een fout zich voordoen, hoe groter deze som zal worden en hoe sterker we dus gaan bijsturen.

Ga als volgt te werk:
- maak een nieuwe parameter Ki
- In de onSet functie ga je er van uit dat de gebruiker de waarde Ki ingeeft met eenheid seconden. Herreken de waarde Ki naar de ingegeven cyclus tijd
- In de onDebug functie wil de gebruiker de waarde Ki terug met eenheid seconden zien. Herreken Ki alvorens deze door te sturen.
- Wanneer de gebruiker de cyclustijd veranderd moet Ki herrekend worden zodat alles blijft kloppen.

Het is belangrijk dat de integrerende code enkel wordt uitgevoerd wanneer de robot aan het rondrijden is. Gebeurt dit niet, dan zal de iTerm binnen de kortste keren tot zijn maximum zijn opgelopen, en dit omdat de robot enkel kan bijsturen wanneer deze aan het rijden is.

Bij een volgende start zou een volledig opgelopen iTerm natuurlijk roet in het eten gooien. Om diezelfde reden is het belangrijk dat je de iTerm variabele op 0 zet wanneer je de robot terug start. 

### Stap 13
Differentiërend Regelen
Bij differentiërend regelen gaan we kijken hoe snel de fout veranderd. Is het verschil met de fout uit de vorige cyclus groot, dan gaan we sterk bijsturen. Is het verschil eerder klein dan sturen we minder sterk bij.

### Stap 14
Tuning
 
Een PID regelaar is zo ontworpen dat je de 3 verschillende acties (proportioneel, integreren en differentiëren) los van elkaar kan instellen met de parameters Kp, Ki en Kd. Je hoeft ze niet perse alle 3 te gebruiken. De acties zijn eenvoudig uit te schakelen door de bijhorende parameters gelijk te stellen aan 0.

Voor de juiste parameters te vinden werk je best stap voor stap.

### Stap 15
De laatste stap is test, test en test met verschillende parameters. 
Zorg ervoor dat je voeling krijgt met de robot. 
Leg eventueel geteste resultaten vast. 
