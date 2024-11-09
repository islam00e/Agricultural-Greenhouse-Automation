#define MOTOR_UP    6
#define MOTOR_DOWN  7

void setup() {
  // put your setup code here, to run once:
   pinMode(MOTOR_UP, OUTPUT);
   pinMode(MOTOR_DOWN, OUTPUT);
}

void loop() {
 
 /*copy the function name here to be in loop */

 //Move_UP_DOWN_FOR_1sec();
 //Move_UP_FOR_1sec();
 Move_UP_TO_THE_LIMSW();
 //Move_DOWN_FOR_1sec();

}

void Move_UP_DOWN_FOR_1sec()
{
   digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, LOW);   
  delay(1000);
  digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, LOW);
  delay(2000);

  digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, HIGH);   
  delay(1000);
  digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, LOW);
  delay(2000);
}

void Move_UP_FOR_1sec()
{
   digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, LOW);   
  delay(1000);
  digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, LOW);
  delay(2000);
}
void Move_DOWN_FOR_1sec()
{
   digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, HIGH);   
  delay(500);
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, HIGH);
  delay(2000);
}
void Move_UP_TO_THE_LIMSW()
{
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, LOW);   
  delay(5000);
}

