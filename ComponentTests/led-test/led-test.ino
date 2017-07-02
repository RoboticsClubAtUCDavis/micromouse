const int LED_PIN = 2;
const int DET_PIN = 14;

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT); 
    pinMode(LED_PIN, OUTPUT); 
    pinMode(DET_PIN, INPUT); 
    digitalWrite(13,HIGH);
    analogReadResolution(12);
}

void loop() {

for(int i = 0 ; i < 70; i++)
{
    delay(10);
    Serial.printf("%u,%u,%u\n",analogRead(DET_PIN),0,4095);
}
digitalWrite(LED_PIN,HIGH);
for(int i = 0 ; i < 70; i++)
{
    delay(10);
    Serial.printf("%u,%u,%u\n",analogRead(DET_PIN),0,4095);
}
//digitalWrite(LED_PIN,LOW);

}