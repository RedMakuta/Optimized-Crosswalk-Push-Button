const int left = 7;  // the number of the pushbutton pin
const int middle = 6; //motor transistor is connected to pin 3
const int right = 5;
void setup() {
  pinMode(left, OUTPUT);
  pinMode(middle, OUTPUT);
  pinMode(right, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.println("left");
  digitalWrite(left, HIGH);   
  delay(300);          
  digitalWrite(left, LOW);
  delay(50);

  Serial.println("middle");
  digitalWrite(middle, HIGH);   
  delay(300);          
  digitalWrite(middle, LOW);
  delay(50);

  Serial.println("right");
  digitalWrite(right, HIGH);   
  delay(300);          
  digitalWrite(right, LOW);
  delay(50);

  delay(2000);
}
