#include <DHT.h>

#define DHTPIN 14 //GPIO 14번에 DHT11센서 연결
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  dht.begin();
}

void loop(){
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("temp: ");
  Serial.print(t);
  Serial.print(",");
  Serial.print("humi: ");
  Serial.println(h);
}
