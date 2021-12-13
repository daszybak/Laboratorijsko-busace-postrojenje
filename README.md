#  Diplomski rad: Laboratorijsko bušaće postrojenje

## Slike laboratorijskog bušaćeg postrojenja

### Nova konstrukcija

![IMG_20210219_125907](https://user-images.githubusercontent.com/59419133/145835334-f8267140-ce56-41ad-9ddf-9861950c7d10.jpg)

### Opis dijelova

![image](https://user-images.githubusercontent.com/59419133/145835729-65884c01-6d11-489e-9e94-1a759d025e3f.png)

## Opis projekta

Svrha projekta je bila redizajn starog laboratorijskog bušaćeg postrojenja promjenom konstrukcije i senzora. Stara električna kontrolna kutija se koristi u novoj iteraciji laboratorijskog bušaćeg postrojenja, ali ona prethodno nije sadržavala dokumentaciju. Obrnutim inženjeringom sam izvukao konfiguracije PLC-a, servo kontrolera i vektorskog kontrolera te cijeli proces dokumentirao u diplomskom radu. Također sam naručio nove PLC module za nove senzore. Te napisao kod za ručno upravljanje laboratorijskog bušaćeg postrojenja.

*Nakon mog rada nastavljen je doktorat. Moj projekat je omogućio drugim studentima da uče na simulaciji bušenja pomoću laboratorijskog bušaćeg postrojenja.*
*Na ovaj projekt će se vezati mnogi završni, diplomski, i doktorski radovi.*

### Električna kontrolna kutija

![image](https://user-images.githubusercontent.com/59419133/145836951-e8f94065-86b0-4b75-b82b-3761c6ed6564.png)

## Kod

### FU_RS422(nije moj, ali sam ga ja dokumentirao)
Program koji se koristi za komunikaciju s vektorskim kontrolerom. Koristi Modbus RTU protokol. 

-Paljenje i gašenje vektorskog kontrolera
-Automatsko podeševanje vektorskog kontrolera slanjem ulazne frekvencije preko PLC-a
-Ručno podešavanje vektorskog kontrolera preko potentiometra

### Servo_RS232(nije moj, ali sam ga ja modificirao i dokumentirao)

Program koji se koristi za komunikaciju sa servo kontrolerom. Slična komunikacija kao i FU_RS422.

-Paljenje i gašenje servo kontrolera
-Automatsko i ručno podešavanje servo kontrolera
-Čitanje i pisanje BIAS koda koji se vrti na servo kontroleru

Cijeli BIAS kod koji se vrtio na servo kontroleru sam očitao u heksadekadskoj notaciji preko PLC-a jer zbog vremenskog limita nije bilo moguće resetirati servo kontroler i napisati novi kod za njega.

### WITS_TCP0 (nije moj, ali sam ga ja dokumentirao)

Program za komunikaciju preko TCP protokola. Omugućava slanje podataka s PLC-a na kompjuter u wellsite information transfer specification formatu

### Measure(nije moj, ali sam ga ja modificirao i dokumentirao)

Program za konvertiranje digitalnih(raw) vrijednosti u stvarne vrijednosti koje su očitane na senzorima kao što je mjerna čelija.
Za encoder koji nije podešen za vrijeme mog rada opisao sam kako ga podesiti unutar diplomskog rada.

### ManualServo(moj)

Program za upravljanje laboratorijskog bušaćeg postrojenja preko prekidača koji se nalaze na električnoj kontrolnoj kutiji.

![image](https://user-images.githubusercontent.com/59419133/145840664-817a23b7-5e1e-4f09-87b3-31a1e3b904e6.png)

