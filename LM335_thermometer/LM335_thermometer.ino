// Digital thermometer with Arduino and LM335 temperature sensor

#define  LM335_pin  0                          // LM335 output pin is connected to Arduino A0 pin
 
void setup() {
  Serial.begin(9600);
}
 
char message1[] = "Celsius=  00°C";
char message2[] = "Kelvin = 000 K";
char message3[] = "Fahren =  00°F";
int  K, C, F;

void loop() {
  delay(1000);                                 // Wait 1s between readings
  
  K = analogRead(LM335_pin) * 0.451;      // Read analog voltage and convert it to Kelvin (0.489 = 500/1023)
  F = (K - 273) * 9 / 5 + 32;
  C = K - 273;                      // Convert Kelvin to degree Celsius
      
  convert();
  
  Serial.println(message1);
  Serial.println(message2);
  Serial.println(message3);
}

void convert ()
{
  if(C < 0){
    C = abs(C);                    // Absolute value
    message1[9] = '-';                         // Put minus '-' sign
  }
  else
    message1[9]  = ' ';                        // Put space ' '
  if (C > 99)
    message1[9]  = '1';                        // Put 1 (of hundred)
    
  message1[10]  = (C / 10) % 10  + 48;
  message1[11]  =  C % 10  + 48;
  
  message2[9]  = (K / 100) % 10 + 48;
  message2[10]  = (K / 10) % 10 + 48;
  message2[11]  = K % 10 + 48;

  if (F < 100)
    message3[9]  = ' ';                        // Put space ' '
  else
    message3[9]  = (F / 100) % 10 + 48;
  message3[10]  = (F / 10) % 10 + 48;
  message3[11]  = F % 10 + 48;
}
