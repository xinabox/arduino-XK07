/*
 Usage
 This example demonstrates how to use the SAMD SDU library to update a 
 sketch on an Arduino/Genuino Zero clone from XinaBox, CS11

 xChip:
 * CS11

 Steps to update sketch via SD card:

 1) Upload this sketch or another sketch that includes the SDU library
    via #include <xCS11_SDU.h>

 2) Update the sketch as desired. For this example the sketch prints out
    the compiled date and time so no updates are needed.

 3) In the IDE select: Sketch -> Export compiled Binary

 4) Copy the .bin file from the sketch's folder to the SD card and rename
    the file to UPDATE.bin. Eject the SD card from your PC.

 5) Insert the SD card into the board, shield or breakout and press the
    reset button or power cycle the board. The SDU library will then update
    the sketch on the board with the contents of UPDATE.bin

 created 23 March 2017
 by Sandeep Mistry
 Modified by Brendan van Breda
*/

/*
 Include the SDU library 
 
 This will add some code to the sketch before setup() is called
 to check if an SD card is present and UPDATE.bin exists on the
 SD card.
 
 If UPDATE.bin is present, the file is used to update the sketch
 running on the board. After this UPDATE.bin is deleted from the
 SD card.
*/
#include <arduino-CS11_SDU.h>

String message;

void setup() {
  SerialUSB.begin(115200);

  delay(5000); // wait a bit

  message += "Sketch compile date and time: ";
  message += __DATE__;
  message += " ";
  message += __TIME__;

  // print out the sketch compile date and time on the serial port
  SerialUSB.println(message);
}

void loop() {
  // add you own code here
}

