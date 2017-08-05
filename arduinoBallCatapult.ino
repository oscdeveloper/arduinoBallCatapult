#define PIN_BUZZER 4

#define PIN_LED_STATUS_R 11
#define PIN_LED_STATUS_G 10
#define PIN_LED_STATUS_B false

#define PIN_LED_DISTANCE_R 9
#define PIN_LED_DISTANCE_G 6
#define PIN_LED_DISTANCE_B false

#define PIN_POTENTIOMETER A0

//String readString;
//#include <Servo.h>
//Servo rotatingServo;  // create servo object to control a servo

int rotationTime = 2; // min rotating time
int potentiometerValue = 0;
int oldPotentiometerValue = 0;

void setup() {
  Serial.begin(9600);
  //rotatingServo.write(90); //set initial servo position
  //rotatingServo.attach(3);  //the pin for the servo control
  Serial.println("catapult started"); // so I can keep track of what is loaded

  // start short two beeps after power on
  pinMode(PIN_BUZZER, OUTPUT);
  //digitalWrite(PIN_BUZZER, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZER, LOW); 
  delay(200); 
  //digitalWrite(PIN_BUZZER, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZER, LOW);  
  // end short two beeps after power on   

  // start status led after power on
  pinMode(PIN_LED_STATUS_R, OUTPUT);
  pinMode(PIN_LED_STATUS_G, OUTPUT);
  if (PIN_LED_STATUS_B != false) pinMode(PIN_LED_STATUS_B, OUTPUT);
  setColor('s', 0, 10, 0); 
  //digitalWrite(10, HIGH);
  // end status led after power on

  // start distance led after power on
  pinMode(PIN_LED_DISTANCE_R, OUTPUT);
  pinMode(PIN_LED_DISTANCE_G, OUTPUT);
  if (PIN_LED_DISTANCE_B != false)  pinMode(PIN_LED_DISTANCE_B, OUTPUT);   
  potentiometerValue = analogRead(PIN_POTENTIOMETER); 
  setDistanceLedColor(getRotationTime(potentiometerValue)); 
  // end distance led after power on
  
}

void loop() {

  potentiometerValue = analogRead(PIN_POTENTIOMETER);

// 104 servo max left
// 90 stop servo
// 81 sero max right
Serial.println(potentiometerValue);

//myservo.write(n);

if ( potentiometerValue != oldPotentiometerValue ) {

  oldPotentiometerValue = potentiometerValue;

  setDistanceLedColor(getRotationTime(potentiometerValue)); 

  //rotatingServo.write(81);
  //delay(rotationTime * 1000);
  //rotatingServo.write(90); // stop servo rotating
  //Serial.println(potentiometerValue);
  
}


  
  delay(100);
}

void setColor(char ledType, int red, int green, int blue) {
  if (ledType == 's') { // status led
    analogWrite(PIN_LED_STATUS_R, red);
    analogWrite(PIN_LED_STATUS_G, green);
    if (PIN_LED_STATUS_B != false) analogWrite(PIN_LED_STATUS_B, blue);    
  } else { // distance led
    analogWrite(PIN_LED_DISTANCE_R, red);
    analogWrite(PIN_LED_DISTANCE_G, green);
    if (PIN_LED_DISTANCE_B != false) analogWrite(PIN_LED_DISTANCE_B, blue);      
  }
}

void setDistanceLedColor(int rotationTime) {
  switch (rotationTime) {
    case 2:
      setColor('d', 0, 1, 0); 
      break;
    case 3:
      setColor('d', 5, 1, 0); 
      break;
    case 4:
      setColor('d', 30, 1, 0); 
      break;
    case 5:
      setColor('d', 50, 0, 0); 
      break;
  }  
}

int getRotationTime(int potentiometerValue) {
  if ( potentiometerValue >= 0 && potentiometerValue <= 99 ) { // 0 min distance
    rotationTime = 2; // 2 sec
  } else if ( potentiometerValue >= 100 && potentiometerValue <= 449 ) { // 1 distance
    rotationTime = 3; // 3 sec
  } else if ( potentiometerValue >= 450 && potentiometerValue <= 799 ) { // 2 distance
    rotationTime = 4; // 4 sec
  } else if ( potentiometerValue >= 800 && potentiometerValue <= 1023 ) { // 3 max distance
    rotationTime = 5; // 5 sec
  }   
  return rotationTime;
}

