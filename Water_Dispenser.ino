#include <Servo.h>

const int targetDistance = 10;
const int targetAngle = 180;

int coinInserted = 0;
const int coinRequired = 1;

const int coinTrigPin = 13;
const int coinEchoPin = 12;
float coinDistance = 0;

const int waterTrigPin = 11;
const int waterEchoPin = 10;
float waterDistance = 0;

bool isWaterDispensed = false;

Servo servo;
const int servoPin = 9;
int angle;

void setup() {
  Serial.begin(9600);

  pinMode(coinTrigPin, OUTPUT);
  pinMode(coinEchoPin, INPUT);

  pinMode(waterTrigPin, OUTPUT);
  pinMode(waterEchoPin, INPUT);

  servo.attach(servoPin);
  servo.write(0);
}

float calculateDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  return (0.036 * pulseIn(echoPin, HIGH)) / 2;
}

void loop() {

  if (coinDistance < targetDistance && coinDistance > 0) {
    /* If inserted coind is already 2,
        dont increment coinInserted. */
    if (coinInserted == coinRequired) return;

    coinInserted += 1;
    delay(500);
    Serial.print("Coin inserted: ");
    Serial.println(coinInserted);
  }

  if (coinInserted == coinRequired) {
    Serial.println("Inserted coin value is 2");
    coinDistance = 0;
    waterDistance = calculateDistance(waterTrigPin, waterEchoPin);

    // If sensor sensed less than target distance but not negative
    if (waterDistance < targetDistance && waterDistance > 0) {
      Serial.println("Water Dispensing started.");

      // Turn ON the water dispenser
      for (angle = 0; angle < targetAngle; angle++) {
        servo.write(angle);
        delay(2);
      }

      // If the cup is filled, it will turn off and reset the dispenser
      while (waterDistance < targetDistance) {
        waterDistance = calculateDistance(waterTrigPin, waterEchoPin);
        Serial.println("Water Running");
        delay(100);
      }

      Serial.println("Water Dispensing ended.");
      
      // Turn OFF the water dispenser
      for (angle = targetAngle; angle > 0; angle--) {
        servo.write(angle);
        delay(5);
      }
      
      coinInserted = 0;
      waterDistance = 0;
      coinDistance = calculateDistance(coinTrigPin, coinEchoPin);
    }
  }
  else {
    coinDistance = calculateDistance(coinTrigPin, coinEchoPin);
    Serial.print("Coin distance: ");
    Serial.println(coinDistance);
  }

  delay(50);
}
