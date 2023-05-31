#include <Arduino.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <SoftwareSerial.h>

#define TRIGGER_PIN1 30
#define ECHO_PIN1 31
#define TRIGGER_PIN2 6
#define ECHO_PIN2 7
#define MAX_DISTANCE 200
#define DHTPIN 8
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial arduino(34, 35);

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE);
// NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE);

String isWaterAvailable;
String isFoodAvailable;
String finalData;

int chk;
const int relay1 = 24;
const int relay2 = 26;
const int relay3 = 28;

float hum;
float temp;

long duration1, duration2, distance1, distance2;

void kipasdanpenghangat() {  // fix
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");
  if (temp >= 42.00) {  // batas maksimal tertinggi : 42
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, HIGH);

  } else if (temp <= 22.00) {  // batas maksimal terendah : 22
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, LOW);
  } else {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }
}

void airotomatis() {  // fix
  distance1 = sonar1.ping_cm();
  Serial.print(distance1);
  Serial.println(" cm");
  if (distance1 >= 9) {
    digitalWrite(relay3, HIGH);
    Serial.println("Air Kurang");
    isWaterAvailable = "false";
  } else if (distance1 <= 8 && distance1 >= 1) {
    digitalWrite(relay3, LOW);
    Serial.println("Air tersedia");
    isWaterAvailable = "true";
  }
}

void statuspakan() {  // Kalibrasi
  digitalWrite(TRIGGER_PIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);
  duration2 = pulseIn(ECHO_PIN2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  Serial.print(distance2);
  Serial.println(" cm");
  if (distance2 >= 6) {
    Serial.println("Pakan Kurang");
    isFoodAvailable = "false";
  } else if (distance2 <= 5 && distance2 >= 1) {
    Serial.println("Pakan tersedia");
    isFoodAvailable = "true";
  }
}

void setup() {
  Serial.begin(9600);
  arduino.begin(9600);
  // Suhu dan kelembaban
  dht.begin();
  // relay
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  // ultrasonik
  pinMode(TRIGGER_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
}

void loop() {
  kipasdanpenghangat();
  airotomatis();
  statuspakan();
  finalData = String("30Cs") + "#" + String("40%") + "#" + isFoodAvailable +
              "#" + "true" + "!";
  // finalData = String(temp) + "#" + String(hum) + "#" + isFoodAvailable + "#"
  // + isWaterAvailable + "!";
  Serial.println(" ");
  Serial.print(finalData);
  arduino.print(finalData);

  Serial.println(" ");
  delay(1000);
}