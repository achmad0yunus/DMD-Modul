/*–Includes——-*/
#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include <SystemFont5x7.h>
#include <Arial_Black_16_ISO_8859_1.h>

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
