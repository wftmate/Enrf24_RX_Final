//==========================================================
// This program receives a text string on the ENRF_24 wireless
// device and displays the text to an LCD screen.
//==========================================================

// new comment

//#include <SoftwareSerial.h> // Not sure this is needed
#include <ParallaxLCD.h>
#include <SPI.h>
#include <Enrf24.h>
#include <nRF24L01.h>
#include <string.h>

// ENRF24 Radio Setup Stuff
Enrf24 radio(P2_0, P2_1, P2_2);  // CE, CSN, IRQ pins
const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

//LCD setup stuff
#define ROWS 4
#define COLS 20
#define PIN  P1_3
ParallaxLCD lcd(PIN,ROWS,COLS); // desired pin, rows, cols

void printLCD(char*);   // Something about declaring the function use for LCD printing

void setup(){
    // ENRF24 setup Stuff
    Serial.begin(9600);  // Serial port used to display scan results
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    radio.begin();  // Default 1Mbps, put 250000 or 2000000 to test those bitrates
    radio.setRXaddress((void*)rxaddr);

    pinMode(P1_0, OUTPUT);    // Make P1_0 an output pin
    digitalWrite(P1_0, LOW);  // set P1_0 to 0
    radio.enableRX();  // Wake up the transceiver

    // Parallax LCD Setup Code
    lcd.setup(1);
    delay(100);
    lcd.backLightOn();
    lcd.empty();
    Serial.print("Waiting for Transmission");
    printLCD("Waiting for Transmission");
    lcd.on();
}

void loop(){
// ENRF24 Receiver Stuff
    // String inbuf;
    char inbuf[33];                 // char buffer for received characters

    //radio.radioState();

    while(!radio.available(true));      // Is there a Tx around?
    if(radio.read(inbuf)){              // If statement also reads input buffer
        Serial.print("Received Text: ");
        Serial.println(inbuf);          // Print buffer contents to console
        printLCD(inbuf);                // Print buffer contents to LCD Screen
    }
    else{
        printLCD("NO TRANSCEIVER PRESENT");
    }
}

//void printLCD(String string){
void printLCD(char string[33]){
    lcd.empty();
    if(string=="Waiting for Transmission"){
        lcd.at(1,0,string); // print string to LCD screen
    }
    else{
        lcd.at(0,0,"Received Text: ");
        lcd.at(1,0,string); // print string to LCD screen
        delay(2000);
    }
}
