# xCS11_SDU

This library allows for updating firmware on the [CS11](https://github.com/xinabox/xCS11) via the SD using the SAMD SDU(Secue Digital Update).

## Example
```blocks
#include <xCS11_SDU.h> 
#define BLUE 4

// the setup function runs once when you power the board 
void setup() { 
 // initialize digital pin BLUE as an output. 
 pinMode(BLUE, OUTPUT); 
} 

// the loop function runs over and over again forever 
void loop() { 
 digitalWrite(BLUE, HIGH);   // turn the LED ON
 delay(1000);                       // wait for a second 
 digitalWrite(BLUE, LOW);    // turn the LED OFF
 delay(1000);                       // wait for a second
}
```

## Requirements
  - [☒Core Library](https://github.com/xinabox/xCore)
  - [Arduino IDE](https://www.arduino.cc/en/main/software)
  - [☒CHIP CS11](https://github.com/xinabox/xCS11)
  
## Installation Guide
To install the libraries please follow the guide provided on the [Arduino Website](https://www.arduino.cc/en/Guide/Libraries) under "**Importing a .zip Library**".
