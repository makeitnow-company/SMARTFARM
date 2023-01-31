#define SOIL 36 //GPIO 36번 토양센서 연결

void setup() {
  Serial.begin(9600);
}

void loop() {
  int soil_value = analogRead(SOIL);
  Serial.println(soil_value);
  delay(300);
}
