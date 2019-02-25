# Pengetahuan Dasar Pemrograman Modul Led/Dot Matrik Display (DMD) P10 dengan Arduino

## 1. Pendahuluan

Modul Display Led Matrik yg populer saat ini antara lain P10 .Kita dapat dengan mudah memprogram modul tersebut dengan Arduno karena telah tersedia library untuk itu.  library  tsb  dibuatkan oleh salah satu pembuat P10 yaitu Freetronic.

Dimensi modul  P10: 16 led x 32 Led

![modul dmd](./picture/arduino-freetronic.jpg)

## 2. Hardware

![hardware](./picture/modulp10kearduino.jpg)

## 3. Konektor antara Arduino dan modul P10

![soketp10](./picture/soket_p10.jpg)

penjelasan pin pada konektor

- OE: Output Enable untuk on/off semua LED
- A dan B: memilih kolom yg aktif.
- CLK: SPI clock
- SCLK: Latch data register
- Data: SERIAL DATA SPI

## 4. Circuit modul P10
 
![circuitep103](./picture/circuitep103.jpg)

Penjelasan :

Data akan dikirm dari arduino  secara  serial melalui soket HUB1.2 kemudian diterima oleh IC serial to paralel 74595.  Jika ada tambahan modul akan diambil dari keluaran 74595 yg terakhir yg dihubungkan ke soket X2-OUT. Untuk supply arus diberikan oleh ic driver penguat daya VT1,VT2,..dst. Untuk memilih kolom mana yg menyala diaktifkan oleh IC multiplexer.


## 5. Rangkain Modul P10 lebih dari 1 (Cascade )

Jika module P10 lebih dari 1, maka rangkaian dibentuk cascade seperti gambar berikut:

![modulp10cascade](./picture/modulp10cascade.jpg)

Gambar  dilihat dari arah  belakang modul P10

untuk konfigurasi panel diatas maka inisialisasi DMD  :
```c
DMD dmd(2,2);   // 2 modul P10 ke samping , 2 modul P10 ke bawah.

```

## 6. software

Step by step install library untuk menjalankan DMD P10

- Download DMD Libray untuk arduino di  https://github.com/freetronics/DMD
- Extraxt dan simpan DMD library di folder ” arduino/libraries”
- download TimerOne library  untuk arduino di http://code.google.com/p/arduino-timerone/downloads/list
- Extraxt dan tempatkan TimerOne di folder  arduino/libarries
- Restart IDE arduino
- lihat di Arduino IDE  File>Examples>DMD
- buka salah satu contoh & compile.

### a. Header file yg digunakan :
```c
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include “SystemFont5x7.h”
#include “Arial_Black_16_ISO_8859_1.h”
```

### b. Fungsi fungsi  yg ada di library DMD yang digunakan  antara lain:

```c
//Instantiate the DMD
DMD(byte panelsWide, byte panelsHigh);

//Set or clear a pixel at the x and y location (0,0 is the top left corner)
void writePixel( unsigned int bX, unsigned int bY, byte bGraphicsMode, byte bPixel );

//Draw a string
void drawString( int bX, int bY, const char* bChars, byte length, byte bGraphicsMode);

//Select a text font
void selectFont(const uint8_t* font);

//Draw a single character
int drawChar(const int bX, const int bY, const unsigned char letter, byte bGraphicsMode);

//Find the width of a character
int charWidth(const unsigned char letter);

//Draw a scrolling string
void drawMarquee( const char* bChars, byte length, int left, int top);

//Move the maquee accross by amount
boolean stepMarquee( int amountX, int amountY);

//Clear the screen in DMD RAM
void clearScreen( byte bNormal );

//Draw or clear a line from x1,y1 to x2,y2
void drawLine( int x1, int y1, int x2, int y2, byte bGraphicsMode );

//Draw or clear a circle of radius r at x,y centre
void drawCircle( int xCenter, int yCenter, int radius, byte bGraphicsMode );

//Draw or clear a box(rectangle) with a single pixel border
void drawBox( int x1, int y1, int x2, int y2, byte bGraphicsMode );

//Draw or clear a filled box(rectangle) with a single pixel border
void drawFilledBox( int x1, int y1, int x2, int y2, byte bGraphicsMode );

//Draw the selected test pattern
void drawTestPattern( byte bPattern );

//Scan the dot matrix LED panel display, from the RAM mirror out to the display hardware.
//Call 4 times to scan the whole display which is made up of 4 interleaved rows within the 16 total rows.
//Insert the calls to this function into the main loop for the highest call rate, or from a timer interrupt
void scanDisplayBySPI();
```

![koordinat p10](./picture/kordinat_p10.jpg)


## Contoh Kode program:
```c
/*–Includes——-*/
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include “SystemFont5x7.h”
#include “Arial_Black_16_ISO_8859_1.h”

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

/*
Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
called at the period set in Timer1.initialize();
*/
void ScanDMD()
{
dmd.scanDisplayBySPI();
}

/*————————————————————————-
setup
Called by the Arduino architecture before the main loop begins
————————————————————————-*/
void setup(void)
{

//initialize TimerOne’s interrupt/CPU usage used to scan and refresh the display
Timer1.initialize( 3000 ); //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
Timer1.attachInterrupt( ScanDMD ); //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

//clear/init the DMD pixels held in RAM
dmd.clearScreen( true ); //true is normal (all pixels off), false is negative (all pixels on)
Serial.begin(115200);
}

/*————————————————————————-
loop
Arduino architecture main loop
————————————————————————-*/
void loop(void)
{
dmd.clearScreen( true );
dmd.selectFont(Arial_Black_16_ISO_8859_1);

const char *MSG = 'apa  kabar';
dmd.drawMarquee(MSG,strlen(MSG),(32*DISPLAYS_ACROSS)-1,0);
long start=millis();
long timer=start;
while(1){
if ((timer+30) < millis()) {
dmd.stepMarquee(-1,0);
timer=millis();
}
}
}