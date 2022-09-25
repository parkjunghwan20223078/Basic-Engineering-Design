#define PIN_LED 7

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED,0);
  delay(1000);
  digitalWrite(PIN_LED,1);
}
int i = 0;
void loop() {
    digitalWrite(PIN_LED, 0);
    delay(100);
    digitalWrite(PIN_LED, 1);
    delay(100);
    i += 1;
    if (i > 5) {
      while(1) {
        digitalWrite(PIN_LED,1);
      }
    }
      
  }
