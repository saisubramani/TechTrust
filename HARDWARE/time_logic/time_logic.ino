long prevmills = 0;
long interval = 10000;
void setup() {  
  // put your setup code here, to run once:
  Serial.begin(115200);delay(10);
  Serial.println("starting..");
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop() {
  unsigned long currentmills = millis();
  if( currentmills - prevmills > interval )
  {
    prevmills = currentmills;
    Serial.println("ON");
    blink_led();
  }

}
void blink_led()
{
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(200);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)
}
