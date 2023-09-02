#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 columns and 4 rows
const int vermelho = 2;
const int amarelo = 3;
const int verde = 4;
const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 11; // Buzzer connected to digital pin 11
const int interruptorPin = 7; // Pino do interruptor

class DistanceSensor {
  public:
    int trigPin;
    int echoPin;
    LiquidCrystal_I2C &lcd;
    int distance_cm;

    DistanceSensor(int trig, int echo, LiquidCrystal_I2C &lcd) : lcd(lcd) {
      trigPin = trig;
      echoPin = echo;
    }

    void measureDistance() {
      int duration;
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance_cm = duration * 0.034 / 2; // Conversion to centimeters

      Serial.print("Distance: ");
      Serial.print(distance_cm);
      Serial.println(" cm");

      lcd.setCursor(3, 3); // Set cursor position for distance value
      lcd.print("     "); // Clear previous value
      lcd.setCursor(0, 3);
      lcd.print("Cm ");
      lcd.print(distance_cm); // Print new distance value in cm
    }
};

DistanceSensor sensor(trigPin, echoPin, lcd);

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(vermelho, OUTPUT);
  pinMode(amarelo, OUTPUT);
  pinMode(verde, OUTPUT);
  pinMode(interruptorPin, INPUT_PULLUP); // Habilita o resistor de pull-up interno
  lcd.init(); // initialize the LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Medidor distancia");
  lcd.setCursor(0, 1);
  lcd.print("Para balizas");
  lcd.setCursor(0, 2);
  lcd.print("Distancia em cm");
}

void loop() {
  // Verifica o estado do interruptor
  int estadoInterruptor = digitalRead(interruptorPin);

  if (estadoInterruptor == LOW) { // Se o interruptor estiver ligado
    while (estadoInterruptor == LOW) { // Continue enquanto o interruptor estiver ligado
      sensor.measureDistance();
      int distance_cm = sensor.distance_cm;

      int y;
      if (distance_cm <= 3) {
        y = 1;
      } else if (distance_cm <= 6 && distance_cm >= 4) {
        y = 2;
      } else if (distance_cm >= 7 && distance_cm <= 17) {
        y = 3;
      } else if (distance_cm > 17) {
        y = 4;
      }

      switch (y) {
      case 1:
        while (distance_cm <= 3 && estadoInterruptor == LOW) {
          sensor.measureDistance();
          distance_cm = sensor.distance_cm;
          digitalWrite(vermelho, HIGH);
          tone(buzzerPin, 1300);
          delay(50);
          digitalWrite(vermelho, LOW);
          noTone(buzzerPin);
          delay(50);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, LOW);
          estadoInterruptor = digitalRead(interruptorPin);
          digitalWrite(vermelho, LOW);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, LOW);
        }
        break;
      case 2:
        while (distance_cm <= 6 && distance_cm >= 4 && estadoInterruptor == LOW) {
          sensor.measureDistance();
          distance_cm = sensor.distance_cm;
          digitalWrite(vermelho, HIGH);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, LOW);
          tone(buzzerPin, 1250);
          delay(100);
          noTone(buzzerPin);
          delay(100);
          estadoInterruptor = digitalRead(interruptorPin);
          digitalWrite(vermelho, LOW);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, LOW);
        }
        break;

      case 3:
        while (distance_cm >= 7 && distance_cm <= 17 && estadoInterruptor == LOW) {
          sensor.measureDistance();
          distance_cm = sensor.distance_cm;
          digitalWrite(vermelho, LOW);
          digitalWrite(amarelo, HIGH);
          digitalWrite(verde, LOW);
          tone(buzzerPin, 1200);
          delay(150);
          noTone(buzzerPin);
          delay(200);
          estadoInterruptor = digitalRead(interruptorPin);
          digitalWrite(vermelho, LOW);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, LOW);
        }
        break;

      case 4:
        while (distance_cm > 17 && estadoInterruptor == LOW) {
          sensor.measureDistance();
          distance_cm = sensor.distance_cm;
          digitalWrite(vermelho, LOW);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, HIGH);
          tone(buzzerPin, 1150);
          delay(150);
          noTone(buzzerPin);
          delay(300);
          estadoInterruptor = digitalRead(interruptorPin);
          digitalWrite(vermelho, LOW);
          digitalWrite(amarelo, LOW);
          digitalWrite(verde, LOW);
        }
        break;
    }

      // Verifique o estado do interruptor novamente
      estadoInterruptor = digitalRead(interruptorPin);
    }
  } else {
    estadoInterruptor = digitalRead(interruptorPin);
  }
}
