#define MOTOR_CW 13
#define MOTOR_CCW 13

void setup() {
  // put your setup code here, to run once:
   pinMode(MOTOR_CW, OUTPUT);
   pinMode(MOTOR_CCW, OUTPUT);
}

void loop() {
 
  digitalWrite(MOTOR_CCW, HIGH);    
  digitalWrite(MOTOR_CW, LOW);   /*MOVE LEFT */
  delay(1000);
  digitalWrite(MOTOR_CCW, HIGH);    
  digitalWrite(MOTOR_CW, HIGH);
  delay(1000);
  /*digitalWrite(MOTOR_CCW, LOW);      //MOVE RIGHT
  digitalWrite(MOTOR_CW, HIGH);
  delay(500);
  digitalWrite(MOTOR_CCW, HIGH);    
  digitalWrite(MOTOR_CW, HIGH);
  delay(8000);*/
}
