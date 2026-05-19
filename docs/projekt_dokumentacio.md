# Okos beléptető

## Projekt leírás:

Ez a projekt a Neumann János Egyetem mérnökinformatikus BSc képzésén, a Digitális Technika II. tárgy beadandójaként készült.

A projekt célja egy Arduino-alapú okos beléptetőrendszer megvalósítása Tinkercadben, egyéb eszközök felhasználásával.

## Az okos beléptető use-case:

Az okos beléptető célja egy olyan proof of concept rendszer kialakítása, amely a szállásadók számára megkönnyíti a vendégek találkozás- és kulcsmentes bejutását az ingatlanba. A rendszer emellett képes jelezni, ha a szálláson való tartózkodás hiányában nyitva marad egy ablak, ezáltal hozzájárulva az esetleges beázások megelőzéséhez.

### Funkciók és azok megvalósítása:

- A rendszer bekapcsolt állapotban nem ír ki semmit. Amint a Keypadon egy gomb megnyomásra kerül, üdvözli a felhasználót és várja a PIN kódot.
- A rendszer külön nyitó és záró PIN kódot tárol.
- Hibás PIN esetén hibaüzenet jelenik meg.
- Helyes PIN esetén az ajtó nyílik.
- A zármechanizmust a Micro Servo szimbolizálja, amely a zárt és nyitott állapotot különböző pozíciókkal modellezi.
- Zár nyitását követően a rendszer figyeli, hogy megtörténik-e az ajtó becsukása. Ezt a rendszer egy pushButton komponenssel modellezi Tinkercadban. Képzeljünk el egy gombot, ami az ajtó becsukásakor benyomódik, ezzel jelezve az ajtó zárását.
- Az ajtó bezárásakor a zár magától záródik.
- Az ajtó nyitásakor a rendszer észleli a távozást, amennyiben a záró PIN került beütésre.
- Távozás után, amennyiben az ablak nyitva maradt és a rendszer esőt érzékel, a rendszer küld egy push notificationt. Ennek célja lehet pl. egy Google Cloudban CloudRunon futó container alkalmazás, amely értesíti a tulajdonost, hogy az ablak nyitva van. Tinkercad korlátai miatt ezt egy hamis POST üzenettel helyettesítettem.
- Az esőérzékelés szimulációja potméterrel lett megvalósítva.

### Folyamatábra

![LCD-Arduino bekötés](imgs/Folyamatábra.svg)


## Tinkercad részletek:

### Felhasznált komponensek:

- Arduino Uno R3
- Keypad 4x4
- LCD
- Micro Servo

### LCD:

#### LCD bekötése:

![LCD-Arduino bekötés](imgs/LCD-Arduino_bekotes.png)
|LCD Pin|Arduino Uno R3 Pin|
|-----|--------------|
|SCL|A5|
|SDA|A4|
|VCC|5V|
|GND|GND|

#### Keypad bekötése:

![LCD-Arduino bekötés](imgs/Keypad-Arduino_bekotes.png)
|Keypad Pin|Arduino Uno R3 Pin|
|-----|--------------|
|Row 1|11|
|Row 2|10|
|Row 3|9|
|Row 4|8|
|Column 1|7|
|Column 2|6|
|Column 3|5|
|Column 4|4|