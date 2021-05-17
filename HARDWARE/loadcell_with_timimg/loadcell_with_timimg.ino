#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
//#include "Timer.h"

#define DT D6
#define SCK   D5 

//Orange D6 DT (DATA)
//Yellow D5 SCK (CLOCK)


//pins:
const int HX711_dout = DT; //mcu > HX711 dout pin
const int HX711_sck = SCK; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;;
long prevmills = 0;

const unsigned long SECOND = 1000;
const unsigned long MIN = 60*SECOND;
const unsigned long HOUR = 3600*SECOND;
long interval = 1*MIN;

//Timer tt;
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200); delay(10);
  Serial.println();
  Serial.println("Starting...");
  LoadCell.begin();
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = -57.55; // uncomment this if you want to set the calibration value in the sketch
  EEPROM.get(calVal_eepromAdress, calibrationValue);

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) 
  {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else 
  {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
  //tt.every(2000,loadcellModule);
  //tt.oscillate(LED_BUILTIN,1000,LOW);
}

// the loop function runs over and over again forever

void blink_led()
{
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
  

void loop() 
{
 //tt.update();
 loadcellModule();
} 

void loadcellModule()
{
  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;
  unsigned long currentmills = millis();
  
  // get smoothed value from the dataset:
  if (newDataReady) 
  {
    if (millis() > t + serialPrintInterval) 
    {
      if( currentmills - prevmills > interval )
      {
        float i = LoadCell.getData();
        Serial.print("Load_cell output val: ");
        Serial.println(i);
        prevmills = currentmills;
        newDataReady = 0;
        t = millis();
      }
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) 
  {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (LoadCell.getTareStatus() == true) 
  {
    Serial.println("Tare complete");
  }
}
