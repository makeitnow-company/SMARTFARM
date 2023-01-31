#define SOIL 36 //GPIO 36번 토양센서 연결

void setup() {
  Serial.begin(9600);
}

void loop() {
  int soil_value = analogRead(SOIL);
  int soil_humidity = map(soil_value, 1300, 3100, 100, 0);
  Serial.print("토양 습도 : ");
  Serial.print(soil_humidity);
  Serial.println("%");
  delay(300);
}
