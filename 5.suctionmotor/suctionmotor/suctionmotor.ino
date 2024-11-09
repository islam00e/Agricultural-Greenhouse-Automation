#define SUCTION_MOTOR  8

// 6 up
// 7 down
void setup() {
  // put your setup code here, to run once:
   pinMode(SUCTION_MOTOR, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(SUCTION_MOTOR, HIGH);    
  delay(500);
  digitalWrite(SUCTION_MOTOR, LOW);    
   delay(2000);

}
