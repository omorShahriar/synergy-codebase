#define RELAY_1 21
#define RELAY_2 38
#define TOUCH_1 2
#define TOUCH_2 7

byte lastButton1State = LOW;
byte led1State = LOW;
byte lastButton2State = LOW;
byte led2State = LOW;

unsigned long debounceDuration = 50;  // millis
unsigned long lastTimeButton1StateChanged = 0;
unsigned long lastTimeButton2StateChanged = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RELAY_1, OUTPUT);  // Relay 1
  pinMode(RELAY_2, OUTPUT);  // Relay 2
  pinMode(TOUCH_1, INPUT);   // Touch 1
  pinMode(TOUCH_2, INPUT);   // Touch 2
}

void loop() {
  if (millis() - lastTimeButton1StateChanged > 300) {
    byte button1State = digitalRead(TOUCH_1);
    if (button1State != lastButton1State) {
      lastTimeButton1StateChanged = millis();
      lastButton1State = button1State;
      if (button1State == LOW) {
        led1State = (led1State == HIGH) ? LOW : HIGH;
        digitalWrite(RELAY_1, !led1State);
      }
    }
  }
  if (millis() - lastTimeButton2StateChanged > 500) {
    byte button2State = digitalRead(TOUCH_2);
    if (button2State != lastButton1State) {
      lastTimeButton2StateChanged = millis();
      lastButton2State = button2State;
      if (button2State == LOW) {
        led2State = (led2State == HIGH) ? LOW : HIGH;
        digitalWrite(RELAY_2, !led2State);
      }
    }
  }
  Serial.println(led1State);
}
