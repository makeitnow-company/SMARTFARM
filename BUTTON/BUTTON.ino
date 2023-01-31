#define BUTTON 23

void setup() {
  pinMode(BUTTON,INPUT);
  Serial.begin(9600);
}

void loop() {
  int btn_value = digitalRead(BUTTON);
  Serial.println(btn_value);
  delay(300);
}
