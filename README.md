# IoT-Smart-Intercome

Kratki naziv tima: Siewomens 

Naziv teme:  Pametno zvono 

Područje primjene:  kućanstvo 

Opis teme:  

Ideja: Pametno zvono koristi senzor pokreta i kameru kako bi osoba mogla pozvoniti bez kontakta te prikazuje snimku posjetitelja stanaru putem korisničke aplikacije.  

Primjena: automatsko pozivanje zvona bez potrebe za fizičkim dodirivanjem, snimanje slika ili videozapisa posjetitelja na vratima u sigurnosne svrhe. 

Scenarij uporabe: osoba prilazi vratima i pokreće senzor pokreta. Senzor pokreta zatim aktivira ESP32 mikrokontroler. Ako je pokret detektiran, pali se ultrazvučni senzor te detektira koliko je objekt udaljen. Ako je objekt blizu, unutar pola metra, šalje se obavijest preko platforme te se neuronskom mrežom detektira osoba iz predefinirane slike. Ovisno o rezultatu, aktuator zvona se aktivira obavještavajući stanara o posjetitelju. 

Dodatna vrijednost rješenja: povećava sigurnost doma omogućujući snimanje posjetitelja, nema potrebe za fizičkim kontaktom sa zvonom što može biti korisno u pandemiji, kod osoba s teškoćama kretanja ili npr. kad su stanaru pune ruke i ne može fizički pozvoniti svojim ukućanima. 

Potencijal za komercijalizaciju: Pametno zvono ima veliki potencijal za komercijalizaciju, posebno kao dio pametne kuće i sigurnosnih rješenja. Može se prodavati kao samostalni uređaj ili integrirati u postojeće pametne sustave nadzora.  

Potrebna oprema:    

ESP32, senzor pokreta, aktuator (zvono), ultrazvučni senzor 

  

IoT-platforma:  HomeAssistant 

# Upute za postavljanje i pokretanje

Fizički senzori:  
Senzore spojite na odgovarajuće pinove definirane u dokumentu ESP32/combined_sense/combined_sense.ino. U Arduino IDE otvorite .ino sketch, podesite IP adresu MQTT brokera i korištenu mrežu te uploadajte kod.

Virtualni senzori:  
Spremite virtual_esp, raspakirajte ga te unutar esp.py također promijenite IP adresu MQTT brokera na isti način kao kod virtualnih senzora. Računalo mora biti spojeno na istu mrežu kao MQTT broker.  Zatim pokrenite pyhton skriptu esp.py.

Home Assistant platforma:  




Server:  
Kako bi se pokrenuo sever potrebno je unutar preuzete datoteke server, u mapu models, dodati datoteku yolov4.weights koja se može preuzeti putem ove poveznice "https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v3_optimal/yolov4.weights" (datoteku nije bilo moguće postaviti na git jer je prevelika). Nakon toga potrebno je pokrenuti app.py datoteku.

Korisnička aplikacija:  
Izvorni kod korisničke aplikacije nalazi se u zasebnom git repozitoriju, upute za postavljanje i pokretanje navedene su u dokumentaciji projekta.
