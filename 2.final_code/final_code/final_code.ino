/************Final Code*****************/

#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial mySerial(10,11); // RX, TX pins
Servo myservo;                  // create servo object to control a servo

int x_deviation = 0, y_deviation = 0;
char x ,y;
char flag=0;
long duration1, Dis_X,duration2, Dis_Y,duration3, Dis_Z;

char circle_count_X, circle_count_Y;


/****************** Motor control pins ********************************/
#define MOTOR_X_RIGHT     2
#define MOTOR_X_LEFT      3
#define MOTOR_Y_FORWARD   5
#define MOTOR_Y_BACKWARD  4
#define MOTOR_Z_UP        7 
#define MOTOR_Z_DOWN      6
#define Suction_Motor     8
#define Servo_Pin         9

/**************** Define pins for ultrasonic sensors *****************/
#define trigPin1          12  
#define echoPin1          13 
#define trigPin2          A2
#define echoPin2          A3
#define triggerPin3       A0
#define echoPin3          A1



#define TOLERANCE 5

#define Motor_ON    2000
#define Motor_OFF   1000

#define MOTOR_Z_ON_init    2000
#define MOTOR_Z_OFF_init   1000
#define MOTOR_Z_ON_plate   2000
#define Motor_Z_OFF_plate  1000
#define MOTOR_Z_ON_soil    2000
#define MOTOR_Z_OFF_soil   1000


#define MOTOR_Suction_ON    2000
#define MOTOR_Suction_OFF   1000


#define Servo_ANG     180
#define Servo_ANG_RE   0

#define STATE_IDLE 0
#define STATE_RECEIVE_DATA 1
#define STATE_CONTROL_MOTORS 2
#define STATE_PUT_SEEDS 3

char state = STATE_IDLE;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(triggerPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(MOTOR_X_RIGHT, OUTPUT);
  pinMode(MOTOR_X_LEFT, OUTPUT);
  pinMode(MOTOR_Y_FORWARD, OUTPUT);
  pinMode(MOTOR_Y_BACKWARD, OUTPUT);
  pinMode(MOTOR_Z_UP, OUTPUT);
  pinMode(MOTOR_Z_DOWN, OUTPUT);
  pinMode(Suction_Motor, OUTPUT);
  myservo.attach(Servo_Pin);
  mySerial.begin(9600);

  GoToInitPos(); // Move to initial position
}

void loop() {
  while(mySerial.available()){

    UltraSonic_Read();

    if( Dis_Y <= 385 )  // CHECK THE LIMITS OF Y
    {

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
  }
    circle_count_Y++;

      while (Dis_Y <= (10+(5*circle_count_Y)))
      {
       digitalWrite(MOTOR_Y_FORWARD, HIGH);
      digitalWrite(MOTOR_Y_BACKWARD, LOW);
      delay(Motor_ON); 
      digitalWrite(MOTOR_Y_BACKWARD, LOW);
      digitalWrite(MOTOR_Y_FORWARD, LOW);
      Serial.println("We 're in position of Y");
      delay(Motor_OFF);  
      UltraSonic_Read();
      } 
      while (Dis_Y > (10+(5*circle_count_Y)))
      {
       digitalWrite(MOTOR_Y_FORWARD, LOW);
      digitalWrite(MOTOR_Y_BACKWARD, HIGH);
      delay(Motor_ON); 
      digitalWrite(MOTOR_Y_BACKWARD, LOW);
      digitalWrite(MOTOR_Y_FORWARD, LOW);
      Serial.println("We 're in position of Y");
      delay(Motor_OFF);  
      UltraSonic_Read();
      } 
     
    } 
    else 
  {

    circle_count_X++;

    while( Dis_X > 10 )    
   {
    digitalWrite(MOTOR_X_RIGHT, LOW);
    digitalWrite(MOTOR_X_LEFT, HIGH);    
    Serial.println("Getting to init position");
    delay(Motor_ON);  
    digitalWrite(MOTOR_X_RIGHT, HIGH);   
    digitalWrite(MOTOR_X_LEFT, HIGH); 
    Serial.println("We 're in position of X");
    delay(Motor_OFF); 
    UltraSonic_Read();
   }

   while(Dis_Y > 10)
   {

    digitalWrite(MOTOR_Y_BACKWARD, LOW);
    digitalWrite(MOTOR_Y_FORWARD, HIGH);
    Serial.println("Getting to init position");
    delay(Motor_ON); 
    digitalWrite(MOTOR_Y_BACKWARD, HIGH);
    digitalWrite(MOTOR_Y_FORWARD, HIGH);
    Serial.println("We 're in position of Y");
    delay(Motor_OFF);  
    UltraSonic_Read();
   }

    while(circle_count_X == 3)
    {
      digitalWrite(MOTOR_Y_BACKWARD, HIGH);
      digitalWrite(MOTOR_Y_FORWARD, HIGH);
      digitalWrite(MOTOR_X_RIGHT, HIGH);   
      digitalWrite(MOTOR_X_LEFT, HIGH); 

    }

    while( Dis_X < (10+(circle_count_X*10)))    
   {
    digitalWrite(MOTOR_X_RIGHT, HIGH);
    digitalWrite(MOTOR_X_LEFT, LOW);    
    Serial.println("Getting to init position");
    delay(Motor_ON);  
    digitalWrite(MOTOR_X_RIGHT, HIGH);   
    digitalWrite(MOTOR_X_LEFT, HIGH); 
    Serial.println("We 're in position of X");
    delay(Motor_OFF); 
    UltraSonic_Read();
   }
  }
 }
}
void read_from_rasspberrypi()
{
  // Check if data is available from Python
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
    digitalWrite(MOTOR_X_RIGHT, HIGH);
    digitalWrite(MOTOR_X_LEFT, LOW);
   // delay(MOTOR_ON);
  } else if (x_deviation < -TOLERANCE) {
    digitalWrite(MOTOR_X_RIGHT, LOW);
    digitalWrite(MOTOR_X_LEFT, HIGH);
    //delay(MOTOR_ON);
  } else {
    digitalWrite(MOTOR_X_RIGHT, HIGH);
    digitalWrite(MOTOR_X_LEFT, HIGH);
  }

  // Motor Y control
  if (y_deviation > TOLERANCE) {
    digitalWrite(MOTOR_Y_FORWARD, HIGH);
    digitalWrite(MOTOR_Y_BACKWARD, LOW);
   // delay(MOTOR_ON);
  } else if (y_deviation < -TOLERANCE) {
    digitalWrite(MOTOR_Y_FORWARD, LOW);
    digitalWrite(MOTOR_Y_BACKWARD, HIGH);
    //delay(MOTOR_ON);
  } else {
    digitalWrite(MOTOR_Y_FORWARD, HIGH);
    digitalWrite(MOTOR_Y_BACKWARD, HIGH);
  }

  // Stop all motors when deviations are within tolerance
  if (abs(x_deviation) <= TOLERANCE && abs(y_deviation) <= TOLERANCE) {
    digitalWrite(MOTOR_X_RIGHT, HIGH);
    digitalWrite(MOTOR_X_LEFT, HIGH);
    digitalWrite(MOTOR_Y_FORWARD, HIGH);
    digitalWrite(MOTOR_Y_BACKWARD, HIGH);
    delay(Motor_OFF);
    
    state = STATE_PUT_SEEDS;
  }
  else 
  {
    state = STATE_RECEIVE_DATA;
  }
}

void Put_Seeds() {
  
    UltraSonic_Read();

    while (Dis_Z <= 60) {
      MoveMotorUp();
      UltraSonic_Read();
    }
    MoveMotorDown();
    CheckSeeds();
    ReturnInitPosition();
    GetMoreSeeds();
    ReturnFinalPosition();
  state = STATE_IDLE;
}
void MoveMotorDown() {
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, LOW);
  delay(MOTOR_Z_ON_soil);
  digitalWrite(MOTOR_Z_UP, LOW);
  digitalWrite(MOTOR_Z_DOWN, LOW);
  delay(MOTOR_Z_OFF_soil);
  digitalWrite(Suction_Motor, LOW);
}

void MoveMotorUp() {
  digitalWrite(MOTOR_Z_UP, LOW);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
  delay(100); // Adjust this delay as needed
  digitalWrite(MOTOR_Z_UP, HIGH);
  digitalWrite(MOTOR_Z_DOWN, HIGH);
}
void CheckSeeds() {
  myservo.write(Servo_ANG);
  while (Dis_Z >= 40) {
  MoveMotorDown();
  UltraSonic_Read();
  digitalWrite(Suction_Motor, LOW);
  }
  digitalWrite(Suction_Motor, HIGH);
}
void ReturnInitPosition() {
  while (Dis_Z <= 63) {
    MoveMotorUp();
    UltraSonic_Read();
    digitalWrite(Suction_Motor, HIGH);
  }
  myservo.write(Servo_ANG_RE);
}
void ReturnFinalPosition() {
  while (Dis_Z <= 63) {
     MoveMotorUp();
    UltraSonic_Read();
    digitalWrite(Suction_Motor, LOW);
     myservo.write(Servo_ANG_RE);
  }
}
void GetMoreSeeds() {
  while (Dis_Z >= 30) {
    MoveMotorDown();
    UltraSonic_Read();
    digitalWrite(Suction_Motor, HIGH);
  }
  digitalWrite(Suction_Motor, LOW);
}
void GoToInitPos() {
  // Move to initial position
  while (Dis_X > 10) {
    digitalWrite(MOTOR_X_RIGHT, HIGH);
    digitalWrite(MOTOR_X_LEFT, LOW);
    delay(Motor_ON);
    digitalWrite(MOTOR_X_RIGHT, LOW);
    digitalWrite(MOTOR_X_LEFT, LOW);
    delay(Motor_OFF);
    UltraSonic_Read();
  }

  while (Dis_Y > 10) {
    digitalWrite(MOTOR_Y_BACKWARD, HIGH);
    digitalWrite(MOTOR_Y_FORWARD, LOW);
    delay(Motor_ON);
    digitalWrite(MOTOR_Y_BACKWARD, LOW);
    digitalWrite(MOTOR_Y_FORWARD, LOW);
    delay(Motor_OFF);
    UltraSonic_Read();
  }
}

void UltraSonic_Read()
{
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
   if(duration1==0)
  { 
  }
  else
  {
    Dis_X = (duration1 * 0.034 / 2);
  }
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  if(duration2==0)
  { 
  }
  else
  {
    Dis_Y = (duration2 * 0.034 / 2);
  }

  digitalWrite(triggerPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  if(duration3==0)
  { 
  }
  else
  {
    Dis_Z = (duration3 * 0.034 / 2);
  }


// Print distances to Serial Monitor
  Serial.print("Distance from Sensor 1: ");
  Serial.print(Dis_X);
  Serial.println(" cm");

  Serial.print("Distance from Sensor 2: ");
  Serial.print(Dis_Y);
  Serial.println(" cm");

  Serial.print("Distance from Sensor 3: ");
  Serial.print(Dis_Z);
  Serial.println(" cm");
}
