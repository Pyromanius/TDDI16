Bildmatchning
=============

- Ungefärligt antal timmar spenderade på labben (valfritt): 


- Vad är tidskomplexiteten på "slow.cpp" och din implementation av "fast.cpp",
  uttryckt i antalet bilder (n).

slow: O(n^2)
fast: O(n)

Slow tar längre tid eftersom den jämför alla bilder med varandra. 
Fast använder istället hashing, vilket betyder att den kan köras fortare då varje bild enbart gås igenom en gång.


- Hur lång tid tar det att köra "slow.cpp" respektive "fast.cpp" på de olika
  datamängderna?
  Tips: Använd flaggan "--nowindow" för enklare tidsmätning.
  Tips: Det är okej att uppskatta tidsåtgången för de fall du inte orkar vänta
  på att de blir klara.
  Tips: Vid uppskattning av körtid för "slow.cpp" är det en bra idé att beräkna
  tiden det tar att läsa in (och skala ner) bilderna separat från tiden det tar att
  jämföra bilderna. (Varför?)

|--------+-----------+---------------+----------|
|        | inläsning |    slow.cpp   | fast.cpp |
|--------+-----------+---------------+----------|
| tiny   |    180    |      285      |   282    |
| small  |    1190   |      1433     |   1428   |
| medium |    5117   |      7235     |   5154   |
| large  |   104501  |     300000+   |  106578  |
|--------+-----------+---------------+----------|


- Testa olika värden på "summary_size" (exempelvis mellan 6 och 10). Hur
  påverkar detta vilka dubbletter som hittas i datamängden "large"?

Större summary_size tar längre tid men är mer exakt. Detta beror på att den då behöver gå igenom 
datapunkterna mer noggrannt och därför kommer funktionen att kräva längre tid. 

- Algoritmen som implementeras i "compute_summary" kan ses som att vi beräknar
  en hash av en bild. Det är dock inte helt lätt att hitta en bra sådan funktion
  som helt motsvarar vad vi egentligen är ute efter. Vilken eller vilka
  egenskaper behöver "compute_summary" ha för att vi ska kunna lösa problemet i
  labben? Tycker du att den givna funktionen uppfyller dessa egenskaper?

  Den behöver de egenskaper som den har just nu. Vi tänker att eftersom funktionen gör det den ska så fungerar det.
  Den behöver kunna krympa en bild utan att för mycket data går förlorat. Dne behöver alltså ha en egenskap för att kunna ställa om
  bilderna till ett godtyckligt antal pixlar och sedan en algoritm för att söka igenom och jämföra ljusstyrkan för dessa. 
  Det går säkerligen att förbättra funktionen genom att använda andra metoder.



- Ser du några problem med metoden som används i labben? Kan du komma på andra
  metoder att hitta bilder som är "ungefär" lika? Vad har de för för- och
  nackdelar jämfört med den som föreslås i labben? (Testa gärna om du har idéer)

Istället för att jämföra ljusstyrka, vilket kan bli missvisande om vi t.ex. har olikfärgade blommor men liknande kontrast,
så skulle man kunna jämföra bildernas RGB-värde och på så vis få ett mer korrekt resultat.

