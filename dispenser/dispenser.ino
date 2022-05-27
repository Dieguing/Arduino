/*Dispenser control*/

bool flag = true;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
}

// the loop function runs over and over again forever
void loop() {
  delay(500);
  if (flag)
  {
    digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(35000);
    digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(60000);
    digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(30000);
    digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    flag = false;
  }
  

}
