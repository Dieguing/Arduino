void setup()
{ Serial.begin(115200); }
void loop()
{
unsigned a;
a= millis()/10;
float b;
b=sin(a)*100+100;
Serial.println(b);
delay(1);
}
