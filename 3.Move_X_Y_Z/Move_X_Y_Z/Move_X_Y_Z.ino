#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 10); // RX, TX pins
#include <Servo.h>
Servo myservo;

long duration3, Dis_Z, distance_3;
long prev_distance_3 = 0; // Store previous distance

int x_deviation = 0, y_deviation = 0;
char x, y;

#define MOTOR_FORWARD    5
#define MOTOR_BACKWARD   4
#define MOTOR_RIGHT      2
#define MOTOR_LEFT       3
#define MOTOR_Z_UP       6 
#define MOTOR_Z_DOWN     7
#define Suction_Motor    8
#define Servo_Pin        9

#define triggerPin3       A0
#define echoPin3          A1

#define TOLERANCE 1
#define MOTOR_ON  100
#define MOTOR_OFF 50

#define Servo_ANG         160
#define Servo_ANG_RE      30

#define STATE_IDLE 0
#define STATE_RECEIVE_DATA 1
#define STATE_CONTROL_MOTORS 2
#define STATE_PUT_SEEDS 3

char state = STATE_IDLE;


void setup() {
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
  pinMode(MOTOR_BACKWARD, OUTPUT);
  pinMode(MOTOR_FORWARD, OUTPUT);

  digitalWrite(MOTOR_LEFT, HIGH);
  digitalWrite(MOTOR_RIGHT, HIGH);
  digitalWrite(MOTOR_BACKWARD, HIGH);
  digitalWrite(MOTOR_FORWARD, HIGH);

  pinMode(triggerPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(MOTOR_Z_UP, OUTPUT);
  pinMode(MOTOR_Z_DOWN, OUTPUT);
  pinMode(Suction_Motor, OUTPUT);

  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, HIGH);

  
  myservo.attach(Servo_Pin);
  myservo.write(Servo_ANG_RE);
  digitalWrite(Suction_Motor, LOW);

  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  switch (state) {
    case STATE_IDLE:
      read_from_rasspberrypi();
      break;

    case STATE_RECEIVE_DATA:
      read_from_rasspberrypi();
      break;

    case STATE_CONTROL_MOTORS:
      controlMotors();
      break;

    case STATE_PUT_SEEDS:
      Put_Seeds();
      break;
      default:
      break;
  }
  
}

void read_from_rasspberrypi() {
  if (mySerial.available() >= 2) {
    x = mySerial.read();
    y = mySerial.read();

    x_deviation = static_cast<int>(x);
    y_deviation = static_cast<int>(y);

    Serial.print("X Deviation: ");
    Serial.println(x_deviation);
    Serial.print("Y Deviation: ");
    Serial.println(y_deviation);
    state = STATE_CONTROL_MOTORS;

  }
}

void controlMotors() {  // Motor X control
  if (x_deviation > TOLERANCE) {
    digitalWrite(MOTOR_RIGHT, HIGH);
    digitalWrite(MOTOR_LEFT, LOW);
    delay(MOTOR_ON);
    digitalWrite(MOTOR_RIGHT, HIGH);
    digitalWrite(MOTOR_LEFT, HIGH);
  } else if (x_deviation < -TOLERANCE) {
    digitalWrite(MOTOR_RIGHT, LOW);
    digitalWrite(MOTOR_LEFT, HIGH);
    delay(MOTOR_ON);
    digitalWrite(MOTOR_RIGHT, HIGH);
    digitalWrite(MOTOR_LEFT, HIGH);
  } else {
    digitalWrite(MOTOR_RIGHT, HIGH);
    digitalWrite(MOTOR_LEFT, HIGH);
  }

  // Motor Y control
  if (y_deviation > TOLERANCE) {
    digitalWrite(MOTOR_FORWARD, LOW);
    digitalWrite(MOTOR_BACKWARD, HIGH);
    delay(MOTOR_ON);
    digitalWrite(MOTOR_FORWARD, HIGH);
    digitalWrite(MOTOR_BACKWARD, HIGH);
  
  } else if (y_deviation < -TOLERANCE) {
    digitalWrite(MOTOR_FORWARD, HIGH);
    digitalWrite(MOTOR_BACKWARD, LOW);
    delay(MOTOR_ON);
    digitalWrite(MOTOR_FORWARD, HIGH);
    digitalWrite(MOTOR_BACKWARD, HIGH);
  }
   else {
    digitalWrite(MOTOR_FORWARD, HIGH);
    digitalWrite(MOTOR_BACKWARD, HIGH);
  }

  // Stop all motors when deviations are within tolerance
  if (abs(x_deviation) <= TOLERANCE && abs(y_deviation) <= TOLERANCE) {
    digitalWrite(MOTOR_FORWARD, HIGH);
    digitalWrite(MOTOR_BACKWARD, HIGH);
    digitalWrite(MOTOR_LEFT, HIGH);
    digitalWrite(MOTOR_RIGHT, HIGH);
    delay(MOTOR_OFF);
    
    state = STATE_PUT_SEEDS;
  }
  else 
  {
    state = STATE_RECEIVE_DATA;
  }
}

void Put_Seeds() {
    ReturnFinalPosition();
    GetSeeds();
    ReturnInitPosition();
    PutSeedsInSoil();
    ReturnFinalPosition();
    state = 5;
}

void MoveMotorDown() {
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, LOW);
  delay(MOTOR_ON);
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
  delay(MOTOR_OFF);
}

void MoveMotorUp() {
  digitalWrite(MOTOR_Z_UP, LOW);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
  delay(300); 
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
  delay(MOTOR_OFF);
}

void GetSeeds() {
  myservo.write(Servo_ANG);
  delay(1000);
  while (Dis_Z >= 24) {
    MoveMotorDown();
    UltraSonic_Read();
    digitalWrite(Suction_Motor, LOW);
  }
  digitalWrite(Suction_Motor, HIGH);
  delay(50);
  myservo.write(Servo_ANG);
  delay(50);
}

void ReturnInitPosition() {
  myservo.write(Servo_ANG);
  delay(50);
  digitalWrite(Suction_Motor, HIGH);
  delay(500);
  while (Dis_Z <= 36) {
    MoveMotorUp();
    UltraSonic_Read();
    digitalWrite(Suction_Motor, HIGH);
  }
  digitalWrite(Suction_Motor, HIGH);
  delay(50);
  myservo.write(Servo_ANG_RE);
  delay(500);
}

void PutSeedsInSoil() {
  myservo.write(Servo_ANG_RE);
  delay(500);
  while (Dis_Z >= 17) {
    MoveMotorDown();
    UltraSonic_Read();
    digitalWrite(Suction_Motor, HIGH);
  }
  digitalWrite(Suction_Motor, LOW);
  delay(500);
  myservo.write(Servo_ANG_RE);
  delay(500);
}

void ReturnFinalPosition() {
  digitalWrite(Suction_Motor, LOW);
  delay(500);
  myservo.write(Servo_ANG_RE);
  delay(500);
  while (Dis_Z <= 58) {
    MoveMotorUp();
    UltraSonic_Read();
  }
  digitalWrite(Suction_Motor, LOW);
  delay(500);
  myservo.write(Servo_ANG_RE);
  delay(500);
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
    if (distance_3 >= 10 && distance_3 <= 65) {
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

