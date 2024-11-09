#include <Servo.h>

long duration3, Dis_Z, distance_3;
long prev_distance_3 = 0; // Store previous distance


void UltraSonic_Read();

#define triggerPin3       A0  // Analog pin 0
#define echoPin3          A1  // Analog pin 1

void setup() {
  Serial.begin(9600);    // Initialize the serial communication
  pinMode(triggerPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

void loop() {
  UltraSonic_Read();
}

void UltraSonic_Read() {
  digitalWrite(triggerPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  if (duration3 != 0) {
    distance_3 = (duration3 * 0.034 / 2);
    if (distance_3 >= 17 && distance_3 <= 80) {
      Dis_Z = distance_3;
    } else {
      // If new distance is out of range, use previous distance
      Dis_Z = prev_distance_3;
    }
    prev_distance_3 = Dis_Z; // Update previous distance
    Serial.print("Distance from Sensor: ");
    Serial.print(Dis_Z);
    Serial.println(" cm");
  } else {
    Serial.println("No echo received!");
  }
  delay(100);
}

