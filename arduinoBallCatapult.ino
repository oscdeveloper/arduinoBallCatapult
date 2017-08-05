#define PIN_BUZZER 4

#define PIN_LED_STATUS_R 11
#define PIN_LED_STATUS_G 10
#define PIN_LED_STATUS_B false

#define PIN_LED_DISTANCE_R 9
#define PIN_LED_DISTANCE_G 6
#define PIN_LED_DISTANCE_B false

#define PIN_POTENTIOMETER A0

#define PIN_ARM_CHECK 8

#define PIN_DOG_BUTTON 7

#define PIN_POWER_SERVO 3

#include <Servo.h>
Servo rotatingServo;  // create servo object to control a servo

int rotationTime = 2; // min rotating time
int potentiometerValue = 0;
int oldPotentiometerValue = 0;

void setup() {
  Serial.begin(9600);  
  rotatingServo.attach(PIN_POWER_SERVO);  //the pin for the servo control
  //rotatingServo.write(90); //set initial servo position
  Serial.println("catapult started"); // so I can keep track of what is loaded

  pinMode(PIN_ARM_CHECK, INPUT_PULLUP);

  pinMode(PIN_DOG_BUTTON, INPUT_PULLUP);

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

  if ( potentiometerValue != oldPotentiometerValue ) {
  
    oldPotentiometerValue = potentiometerValue;

    rotationTime = getRotationTime(potentiometerValue);
  
    setDistanceLedColor(rotationTime); 
  
    //rotatingServo.write(81);
    //delay(rotationTime * 1000);
    //rotatingServo.write(90); // stop servo rotating
    //Serial.println(potentiometerValue);
    
  }

  

if ( digitalRead(PIN_ARM_CHECK) == LOW ) { // arm is ready to shoot
  setColor('s', 0, 10, 0); // green led

  if ( digitalRead(PIN_DOG_BUTTON) == LOW ) { // dog button pushed, arm is loading 
Serial.println(rotationTime);
    setColor('s', 10, 0, 0); // red led
    //delay(500);
    //setColor('s', 0, 0, 0);
    //delay(500);
    //rotatingServo.write(81);
    delay(rotationTime * 1000);
    //rotatingServo.write(90); // stop servo rotating
    Serial.println("servo trigger");
    
  }
  
} else { // arm is lowering
  setColor('s', 30, 1, 0); // orange led
  delay(500);
  setColor('s', 0, 0, 0);
  delay(500);
}

// 104 servo max left
// 90 stop servo
// 81 sero max right
Serial.println(potentiometerValue);

//myservo.write(n);




  
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
      setColor('d', 0, 1, 0); // green
      break;
    case 3:
      setColor('d', 5, 1, 0); // light orange
      break;
    case 4:
      setColor('d', 30, 1, 0); // strong orange
      break;
    case 5:
      setColor('d', 50, 0, 0); // red
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

