/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

 Ungefärligt antal timmar spenderade på labben (valfritt):

/**********************************************************************
 * Empirisk analys
 *
 * Fyll i tabellen nedan med riktiga körtider i sekunder när det känns
 * vettigt att vänta på hela beräkningen.
 * Ge uppskattningar av körtiden (baserat på tidskomplexiteten)
 * i övriga fall.
 *
 **********************************************************************/
    
      N       brute       sortering
 ----------------------------------
    150          6             3
    200         17             6
    300         43            16
    400         95            36
    800        741           133
   1600       5255           560
   3200      30450          2402   
   6400     218634         10412
  12800    *crash*         32654


/**********************************************************************
 * Teoretisk analys
 *
 * Ge ordo-uttryck för värstafallstiden för programmen som en funktion
 * av N. Ge en kort motivering.
 *
 * Matchar brute-lösningen sitt värstafall i praktiken, eller har den
 * ett medelfall som är bättre?
 *
 **********************************************************************/

Brute:         O(n⁴)

Sortering:     O(n²log*n)

Motivation:    Värsta fallet för funktionen är när den inte hittar några punkter, eftersom den då måste gå igenom 
               flest loopar och if-satser. Detta betyder att i fallet brute.cpp så går den alltid igenom alla kombinationer 
               av 4 punkter, medans i fast.cpp så kommer funktionen endast fortsätta söka efter punkter i de fall då 2
               punkter hittas på samma axel. Den behöver alltså inte söka igenom ett helt set om 4 punkter.


/**********************************************************************
 * Energianvändning
 *
 * Antag att du använder mönsterigenkänningsprogrammet för att analysera
 * data från en kamera. Kameran sitter i en byggnad och tar en bild
 * på stommen av byggnaden var 30:e minut. Bilden förbehandlas sedan
 * lite, innan punkter som representerar stommen skickas till
 * mönsterigenkänningsprogrammet. Hittas inte tillräckligt många raka
 * linjer så betyder det att något håller på att gå sönder, och
 * att byggnaden behöver noggrannare inspektion.
 *
 * Hur mycket energi sparar du på ett år om du använder din snabbare
 * sorteringslösning i stället för brute-lösningen? Du kan anta följande:
 * - Systemet körs 24/7 under hela året.
 * - Inget annat körs på det här systemet.
 * - Systemet drar 8 W när det inte gör något (idle)
 * - Systemet drar 36 W när det arbetar (med 1 kärna)
 * - Räkna med att ditt program körs var 30:e minut (= 2 gånger/timme)
 * - För- och efterbehandling är snabba, så vi kan bortse från dem
 * - Indata till programmet innehåller ca 6400 punkter
 * - Det är inte skottår (= 365 dagar)
 *
 * Att jämföra med drar en kombinerad kyl/frys ca 200 kWh per år
 * (enligt Energimyndigheten).
 *
 * Kom ihåg: energi mäts ofta i kWh, vilket är:
 *  energi (kWh) = effekt (kW) * tid (h)
 *
 * Tips: ett sätt att räkna på är att först räkna förbrukningen av
 * ett system som inte gör något på ett helt år, sedan lägga till
 * den extra förbrukningen (36 W - 8 W = 28 W) för tiden som systemet
 * är aktiv.
 *
 * (Siffrorna är löst baserade på en Intel i9-9900K, vi räknar bara på
 * CPU:n för enkelhets skull, besparingarna blir sannolikt större om
 * vi räknar på större delar av systemet, även om andra komponenter
 * också drar ström i "idle".)
 *
 **********************************************************************/
 
     365 * 24 h = 8760 h
     ( 8760 h * 8 W ) / 1000 = 70.08kWh per år (idle)

Förbrukning av brute på ett år: 99.87 kWh

     8760 h * 2 = 17520
     17520 * 218.6 s = 3'829'872 s
     ( 3'829'872 s * 28 W ) / ( 1000 * 3600) = 29.79 kWh


Förbrukning av sortering på ett år: 71.5 kWh

     17520 * 10.4 s = 182'208 s
     ( 182'208 * 28W ) / ( 1000 * 3600 ) = 1.42 kWh

Skillnad: 28.37 kWh

