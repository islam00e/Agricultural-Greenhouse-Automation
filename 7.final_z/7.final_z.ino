#include <Servo.h>
Servo myservo;

long duration3, Dis_Z;

#define MOTOR_Z_UP       6 
#define MOTOR_Z_DOWN     7
#define Suction_Motor    8
#define Servo_Pin        9

#define triggerPin3       A1
#define echoPin3          A0

#define MOTOR_ON  50
#define MOTOR_OFF 50

#define Servo_ANG         60
#define Servo_ANG_RE      180

#define STATE_IDLE 0
#define STATE_RECEIVE_DATA 1
#define STATE_CONTROL_MOTORS 2
#define STATE_PUT_SEEDS 3

char state = STATE_PUT_SEEDS;


void setup() {


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
  //mySerial.begin(9600);
}

void loop() {
  switch (state) {
    case STATE_IDLE:
      break;

    case STATE_RECEIVE_DATA:
      break;

    case STATE_CONTROL_MOTORS:
      break;

    case STATE_PUT_SEEDS:
      Put_Seeds();
      break;
  }
  
}

void Put_Seeds() {
  
    ReturnFinalPosition();
    GetSeeds();
    ReturnInitPosition();
    PutSeedsInSoil();
    ReturnFinalPosition();

    state = STATE_IDLE;
}

void MoveMotorDown() {
  digitalWrite(MOTOR_Z_UP, LOW);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
  delay(MOTOR_ON);
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
  delay(MOTOR_OFF);
}

void MoveMotorUp() {
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, LOW);
  delay(5000); 
}


void GetSeeds() {
  myservo.write(Servo_ANG);
  delay(1000);
  while (Dis_Z >= 21) {
  MoveMotorDown();
  UltraSonic_Read();
  digitalWrite(Suction_Motor, LOW);
  }
  digitalWrite(Suction_Motor, HIGH);
  delay(500);
}


void ReturnInitPosition() {
  digitalWrite(Suction_Motor, HIGH);
  delay(1000);
  
    MoveMotorUp();
    
    digitalWrite(Suction_Motor, HIGH);
  
  delay(500);
  myservo.write(Servo_ANG_RE);
  delay(100);
}

void PutSeedsInSoil() {
  myservo.write(Servo_ANG_RE);
  delay(1000);
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

 
    MoveMotorUp();
   

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
    Dis_Z = (duration3 * 0.034 / 2);
    Serial.print("Distance from Sensor: ");
    Serial.print(Dis_Z);
    Serial.println(" cm");
  } else {
    Serial.println("No echo received!");
  }
  delay(100);
}
