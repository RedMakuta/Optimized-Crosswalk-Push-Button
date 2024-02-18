/*
    Vibration Motor with Arduino
    For more details, visit: https://techzeero.com/arduino-tutorials/vibration-motor-with-arduino/
*/
const int buttonPin = 2;  // the number of the pushbutton pin
const int motorPin = 3; //motor transistor is connected to pin 3
const int motorPin1 = 4;
int buttonState = 0;  //variable for reading the pushbutton status
int bpress = 0;
void setup()
{
  pinMode(motorPin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(13, OUTPUT); // red 
  pinMode(12, OUTPUT); // green
  pinMode(buttonPin, INPUT);
}

void loop()
{
  digitalWrite(12, HIGH);   
  ReadButton();            
  digitalWrite(12, LOW);   
  delay(100); 
  digitalWrite(13, HIGH);  
  if (bpress == 1){
  Vibration();
  }
  else{
    delay(5000);        
  }
  digitalWrite(13, LOW); 
  bpress = 0;  
  delay(100);

}


void Vibration() {
  digitalWrite(motorPin, HIGH); //vibrate 
  delay(625);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(625); //wait 50 seconds.
  digitalWrite(motorPin1, HIGH); //vibrate 
  delay(625);  // delay one second
  digitalWrite(motorPin1, LOW);  //stop vibrating
  delay(625); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate 
  delay(625);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(625); //wait 50 seconds.
  digitalWrite(motorPin1, HIGH); //vibrate 
  delay(625);  // delay one second
  digitalWrite(motorPin1, LOW);  //stop vibrating
  delay(625); //wait 50 seconds.
}

void ReadButton() {
  int i;
  for(i=0; i<=5000; i++){
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
    bpress = 1;
  } 
    delay(1);
    
  }
}
