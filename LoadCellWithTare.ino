/* Arduino Code for Waggies digital weighting scale
 * Load cell input + thresholding + button iteration
 * Written by: Zoryana Harhas, Micheal McIntire, Colin Moyer, Brendan Cohen
 * Last updated: 22 April 2022
*/

#include <HX711_ADC.h>                                    // include library for load cell
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>                                       // include library for EEPROM memory 
#endif

const int HX711_dout = 3;                                 // mcu > HX711 dout pin
const int HX711_sck = 2;                                  // mcu > HX711 sck pin

HX711_ADC LoadCell(HX711_dout, HX711_sck);                // HX711 constructor

const int calVal_eepromAdress = 0;
unsigned long t = 0;

float i = 0;                    // float variable to hold decimal value of digital weight reading
int j = 0;                      // int variable to hold integer value of typecasted weight reading

int blueLED = 6;                // blue plus sign LEDs connected to digital pin 6 on Arduino
int redLED = 5;                 // red minus sign LEDs connected to digital pin 5
int greenLED = 4;               // green smiley face LEDs connected to digital pin 4

int switchPin = 7;              // blue treat selection button is connected to digital pin 7
int led1Pin = 8;                //
int led2pin = 9;                //
int led3pin = 10;               //
int led4pin = 11;               //
// int led5pin = ;              // uncomment and type in pin # to add another treat type (adds another LED to signify treat mode)
// int led6pin = ;              // uncomment and type in pin # to add another treat type (adds another LED to signify treat mode)  
int tarePin = 13;               // red tare button is connected to digital pin 13
int tareLED = 12;               // red tare LED is connected to digital pin 12   

int val;                        // variable for reading the treat selection pin status
int val2;                       // variable for reading the delayed status
int buttonState;                // variable to hold the button state
int Mode = 0;                   // variable to hold which treat mode is being 
int tareval;                    // variable for reading the tare pin status
int tareval2;                   // variable for reading the delayed status
int tareButtonState;            // variable for holding the tare button state


void setup() {
      pinMode(switchPin, INPUT);                  // set the treat selection button pin as input
      pinMode(led1Pin, OUTPUT);                   // 
      pinMode(led2pin, OUTPUT);                   //
      pinMode(led3pin, OUTPUT);                   //
      pinMode(led4pin, OUTPUT);                   //
      //pinMode(led5pin, OUTPUT);                 // uncomment to add another treat type
      //pinMode(led6pin, OUTPUT);                 // uncomment to add another treat type
      buttonState = digitalRead(switchPin);       // read the initial state of the treat selection button pin
      tareButtonState = digitalRead(tarePin);     // read the initial state of the tare button pin

      Serial.begin(57600); delay(10);             // set serial monitor baud rate to 57600 (make sure to set it to this when opening serial monitor)
      Serial.println();
      Serial.println("Starting...");

      pinMode(blueLED, OUTPUT);                   // set blueLED in pin 6 as an output that can be set to HIGH or LOW (turn on & off)
      pinMode(redLED, OUTPUT);                    // set redLED in pin 5 as an output\
      pinMode(greenLED, OUTPUT);                  // set greenLED in pin 4 as an output
      pinMode(tareLED, OUTPUT);                   // set red tareLED

      LoadCell.begin();
      float calibrationValue;                              // calibration value (see example file "Calibration.ino")
      calibrationValue = 1056.51;                          // uncomment this if you want to set the calibration value in the sketch
      #if defined(ESP8266)|| defined(ESP32)
      #endif

      unsigned long stabilizingtime = 2000;                // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
      boolean _tare = true;                                // set this to false if you don't want tare to be performed in the next step
      LoadCell.start(stabilizingtime, _tare);              // start getting digital input from load cell
      if (LoadCell.getTareTimeoutFlag()) {                 // if there is no input...
        Serial.println("Timeout, check MCU>HX711 wiring and pin designations");       // print error message in serial monitor
        while (1);
      }
      else {
        LoadCell.setCalFactor(calibrationValue);           // set calibration value (float)
        Serial.println("Startup is complete");             // print confirmation in serial monitor
      }
  
}

void loop(){
      val = digitalRead(switchPin);        // read input value and store it in val (checking if button has been pressed, binary value of 0 or 1, or LOW or HIGH)
      delay(10);                           // delay for 10 milliseconds
      val2 = digitalRead(switchPin);       // read the input again to check for bounces
      if (val == val2) {                   // compare these two values to make sure we got consistant readings
        if (val != buttonState) {          // if the button state has changed...
          if (val == LOW) {                // & if the button was previously at 0 or LOW (not pressed)...
            if (Mode == 0) {               // & if it is currently on mode 0...          
              Mode = 1;                    // set to mode 1 (chicken)
            } else {                       // otherwise...
                if (Mode == 1) {           // if it was already on mode 1...
                Mode = 2;                  // set to mode 2 ()
            } else {                       // otherwise...
                if (Mode == 2) {           // if it was already on mode 3...
                Mode = 3;                  // set to mode 3 ()
            } else {                       // otherwise...
                if (Mode == 3) {           // if it was already on mode 3...
                Mode = 4;                  // set to mode 4 ()
            } else {                       // otherwise...
                if (Mode == 4) {           // if it was already on mode 4...
                Mode = 1;                  // set back to mode 1 (comment this out if adding another treat selection mode)
                //Mode = 5;                // uncomment to add another treat selection mode
                  }
          
            //Uncomment everything below to add another treat selection mode:
              //else {
                //if(Mode == 5) {
                    //Mode = 1;
                  //}
              //}
        
            }
           }
          }
         }
        }
        buttonState = val;                 // save the new state in buttonState variable
      }

                                          // Turning respectove treat mode LEDs on/off:
      if (Mode == 0) {                    // all off
        digitalWrite(led1Pin, LOW);       
        digitalWrite(led2pin, LOW);
        digitalWrite(led3pin, LOW);
        digitalWrite(led4pin, LOW);
      }

      if (Mode == 1) { 
        digitalWrite(led1Pin, HIGH);      // turn on red LED (chicken mode)
        digitalWrite(led2pin, LOW);
        digitalWrite(led3pin, LOW);
        digitalWrite(led4pin, LOW);
      }

      if (Mode == 2) { 
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2pin, HIGH);      // turn on ____ LED (____ mode)
        digitalWrite(led3pin, LOW);
        digitalWrite(led4pin, LOW);
      }
      if (Mode == 3)  { 
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2pin, LOW);
        digitalWrite(led3pin, HIGH);      // turn on ____ LED (____ mode)
        digitalWrite(led4pin, LOW);
      }    
      if (Mode == 4)  { 
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2pin, LOW);
        digitalWrite(led3pin, LOW);
        digitalWrite(led4pin, HIGH);      // turn on ____ LED (____ mode)
      }
      //if (Mode == 5)  { 
        //digitalWrite(led1Pin, LOW);
        //digitalWrite(led2pin, LOW);
        //digitalWrite(led3pin, LOW);
        //digitalWrite(led4pin, LOW);
        //digitalWrite(led5pin, HIGH)       // turn on ____ LED (____ mode)
      }
    }

     tareval = digitalRead(tarePin);               // read input value and store it in val (checking if button has been pressed, binary value of 0 or 1, or LOW or HIGH)
     delay(10);                                    // delay for 10 milliseconds
     tareval2 = digitalRead(tarePin);              // read the input again to check for bounces
     if (tareval == tareval2) {                    // make sure we got 2 consistant readings!
       if (tareval != tareButtonState) {           // the button state has changed!
         if (val == LOW) {                         // check if the button is pressed
           LoadCell.tare();
           Serial.println("should be taring");
           digitalWrite(tareLED, HIGH); 
           delay(2000);
           digitalWrite(tareLED, LOW);
         }
       }
     }


    static boolean newDataReady = 0;
    const int serialPrintInterval = 0;            //increase value to slow down serial print activity

    // check for new data/start next conversion:
    if (LoadCell.update()) newDataReady = true;

    // get smoothed value from the dataset:
    if (newDataReady) {
      if (millis() > t + serialPrintInterval) {
        float i = LoadCell.getData();
        Serial.print("Load_cell output val: ");
        Serial.println(i);
        newDataReady = 0;
        t = millis();
        int j = (int) i;                          //Typecasting float into int --> makes it easier to use boolean operators and compare values
        j = j*100;
        Serial.print("Typecasted int: ");
        Serial. println(j);

        if(Mode == 1){   
          if (j <= 22679) {
            digitalWrite(blueLED, HIGH);         // Turn on the blue LED
          } else {
            digitalWrite(blueLED, LOW);          // Turn off the blue LED
          }
  
          if (j > 22679 && j <= 23530) {
            digitalWrite(greenLED, HIGH);         // Turn on the green LED
          } else {
            digitalWrite(greenLED, LOW);          // Turn off the green LED
          }

          if (j > 23530) {
          digitalWrite(redLED, HIGH);             // Turn on the red LED
          } else {
            digitalWrite(redLED, LOW);            // Turn off the red LED
          }
          delay(100); 
        }
        else if(Mode == 2){
          Serial.println("MODE 2");
          if (j <= 22679) {
            digitalWrite(blueLED, HIGH);         // Turn on the blue LED
          } else {
            digitalWrite(blueLED, LOW);          // Turn off the blue LED
          }
  
          if (j > 22679 && j <= 23530) {
            digitalWrite(greenLED, HIGH);         // Turn on the green LED
          } else {
            digitalWrite(greenLED, LOW);          // Turn off the green LED
          }

          if (j > 23530) {
          digitalWrite(redLED, HIGH);         // Turn on the red LED
          } else {
            digitalWrite(redLED, LOW);          // Turn off the red LED
          }
          delay(100);           
        }
        else if(Mode == 3){
          Serial.println("MODE 3");
          if (j <= 23813) {
            digitalWrite(blueLED, HIGH);         // Turn on the blue LED
          } else {
            digitalWrite(blueLED, LOW);          // Turn off the blue LED
          }
  
          if (j > 23813 && j <= 24664) {
            digitalWrite(greenLED, HIGH);         // Turn on the green LED
          } else {
            digitalWrite(greenLED, LOW);          // Turn off the green LED
          }

          if (j > 24664) {
          digitalWrite(redLED, HIGH);         // Turn on the red LED
          } else {
            digitalWrite(redLED, LOW);          // Turn off the red LED
          }
          delay(100);           
        }
        else if(Mode == 4){
          Serial.println("MODE 4");
          if (j <= 22679) {
            digitalWrite(blueLED, HIGH);         // Turn on the blue LED
          } else {
            digitalWrite(blueLED, LOW);          // Turn off the blue LED
          }
  
          if (j > 22679 && j <= 22963) {
            digitalWrite(greenLED, HIGH);         // Turn on the green LED
          } else {
            digitalWrite(greenLED, LOW);          // Turn off the green LED
          }

          if (j > 22963) {
          digitalWrite(redLED, HIGH);         // Turn on the red LED
          } else {
            digitalWrite(redLED, LOW);          // Turn off the red LED
          }
          delay(100);           
        }
        else{
          Serial.println("everything off");
        }
      }
    }


    // receive command from serial terminal, send 't' to initiate tare operation:
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 't') LoadCell.tareNoDelay();
    }

    // check if last tare operation is complete:
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
    }

    
}
