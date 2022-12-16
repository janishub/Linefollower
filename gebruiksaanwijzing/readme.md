# Gebruiksaanwijzing

### opladen / vervangen batterijen
Er wordt gewerkt met Li-ion oplaadbare batterijen. 
Voor het opladen is er een speciale oplader voorzien. 
De batterijen worden in seie geschakeld om zo een spanning van 7,4VDC te bekomen (elk 3,7VDC). 

### draadloze communicatie
Als draadloze communicatie is er zowel wifi als bleuthooth mogelijk. 
Voor de communicatie tussen gsm en robot is er voor bleuthooth gekozen. 

#### verbinding maken
Verbinding tussen robot en computer kan via een usb kabel of bleuthooth. 

Verbinding maken tussen gsm en robot is mogelijk via bleuthooth. 
Op u smartphone kan u een app downloaden, serial bleuthooth terminal. 
De app kan u in de app store downloaden. 

#### commando's
debug [on/off]  
kunnen we kijken welke de ingestelde parameters zijn van de robot. 

start  
met het start commando kunnen we robot simpelweg starten.

stop  
stop stoppen we de robot met rijden.

set cycle [µs]  
Is de cyclus tijd die de robot doet.

set power [0..255]  
Hiermee kunnen we de snelheid van de robot instellen.

set diff [0..1]  
Is de waarde die er voor zorgt dat de robot de bochten goed kan nemen. En geen vermogen zal verliezen. 

set kp [0..]  
set ki [0..]  
set kd [0..]  

calibrate black  
Hierbij gaan we de sensor de zwarte waardes kaliberen.

calibrate white  
De sensor wit waarden laten kaliberen. 

### kalibratie
Om deze meetfouten ietwat uit te middelen kunnen we onze individuele sensoren kalibreren. Dit is eigenlijk niet meer dan de sensoren uitlezen onder gekende omstandigheden (de sensoren uitlezen wanneer we weten dat deze "wit" en "zwart" moeten meten). Op die manier hebben we voor iedere sensor referentiewaardes waarmee we kunnen vergelijken.

### settings
De robot rijdt stabiel met volgende parameters: power 120; diff 0,05, KP = 10; ki: 0,20 kp: 0,20.

### start/stop button
Er is een monostabiele start/stop knop voorzien. Deze wordt gebruikt om de robot te starten en stoppen. 
De button is geprogrammeerd als interrups, wil zeggen dat bij elke druk op de knop veranderd de status van start naar stop of van stop naar start. 
