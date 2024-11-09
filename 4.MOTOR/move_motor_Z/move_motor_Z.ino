#define MOTOR_UP    13
#define MOTOR_DOWN  13
void setup() {
  // put your setup code here, to run once:
   pinMode(MOTOR_UP, OUTPUT);
   pinMode(MOTOR_DOWN, OUTPUT);
}

void loop() {
 
 /*copy the function name here to be in loop */

  //Move_UP_DOWN_FOR_1sec(); 
  //Move_DOWN_FOR_1sec();
  Move_UP_FOR_1sec();

}

void Move_UP_DOWN_FOR_1sec()
{
  digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, HIGH);   
  delay(50);
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, HIGH);
  delay(1000);

  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, LOW);   
  delay(50);
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, HIGH);
  delay(1000);
}

void Move_UP_FOR_1sec()
{ 
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, LOW);   
  delay(5000);
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, HIGH);
  delay(5000);
}
void Move_DOWN_FOR_1sec()
{
  digitalWrite(MOTOR_DOWN, LOW);    
  digitalWrite(MOTOR_UP, HIGH);   
  delay(1000);
  digitalWrite(MOTOR_DOWN, HIGH);    
  digitalWrite(MOTOR_UP, HIGH);
  delay(5000);

}

