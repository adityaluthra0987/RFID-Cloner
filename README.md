# RFID-Cloner
I created this project to learn about IoT and to test it in a controlled environment. I will document everything I did, what I learned, and the components used in the process(Entirely for fun). I also used AI to understand the basics of it.
# Components 
1. Arduino Uno
2. RFID Card Reader - RC522
3. BreadBoard - Half Size(Optional)
4. LCD Display 20x4 I2C
5. RFID Cards 125 kHz

# Arduino UNO	RFID Module
- GND	| GND
- 3.3V	| 3.3V
- Digital Pin 9	 | RST
- Digital Pin 10 | SDA
- Digital Pin 11 | MOSI
- Digital Pin 12 | MISO
- Digital Pin 13 | SCK

# Arduino UNO	20x4 I2C LCD
- GND	| GND
- 5V	| VCC
- A4	| SDA
- A5  | SCL

![alt text](https://github.com/adityaluthra0987/RFID-Cloner/blob/main/Screenshot%20from%202024-07-15%2011-15-28.png)

# How to use
- git clone the repo or download RFID_Clone.ino
- Then copy paste the code in ardiuno ide(or whatever you prefrence is)
- upload it in the board
- instructions will be shown in the LCD Display 
