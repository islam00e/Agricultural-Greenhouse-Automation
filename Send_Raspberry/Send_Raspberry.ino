const int ledPin = 13; // Pin number for the LED (built-in LED on most Arduino Uno boards)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // Set the LED pin as an OUTPUT
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.print("Raspberry Pi says: ");
    Serial.println(data);
    
    // If data is received, light the LED
    digitalWrite(ledPin, HIGH);
    delay(1000); // Keep the LED on for 1 second (you can adjust this duration)
    digitalWrite(ledPin, LOW); // Turn off the LED
  }

  String message = "Hello from Arduino!";
  Serial.println(message);
  delay(1000);
}

