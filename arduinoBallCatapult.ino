#define PIN_BUZZER 4

#define PIN_LED_STATUS_R 13
#define PIN_LED_STATUS_G 2
#define PIN_LED_STATUS_B false

#define PIN_LED_DISTANCE_R 5
#define PIN_LED_DISTANCE_G 6
#define PIN_LED_DISTANCE_B false

#define PIN_POTENTIOMETER A0

#define PIN_ARM_LOCK_CHECK 8

#define PIN_DOG_BUTTON 7

#define PIN_SERVO_ROTATE 11
#define PIN_SERVO_ARM_LOCK 3

#include <Servo.h>
Servo servoRotate;
Servo servoArmLock;

int rotationTime = 2; // servo rotating time in seconds
int potentiometerValue = 0;
int oldPotentiometerValue = 0;

void setup() {
  //Serial.begin(9600);  
  //Serial.println("catapult turned on");
    
  servoRotate.attach(PIN_SERVO_ROTATE);
  servoRotate.write(90);
  // 104 servo max left
  // 90 stop servo
  // 81 sero max right  

  servoArmLock.attach(PIN_SERVO_ARM_LOCK, 700, 1200);
  servoArmLock.write(0); // lock the arm
 
  pinMode(PIN_ARM_LOCK_CHECK, INPUT_PULLUP);
  pinMode(PIN_DOG_BUTTON, INPUT_PULLUP);

  // start short two beeps after power on
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZER, LOW); 
  delay(200); 
  digitalWrite(PIN_BUZZER, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZER, LOW);  
  // end short two beeps after power on   

  // start status led after power on
  pinMode(PIN_LED_STATUS_R, OUTPUT);
  pinMode(PIN_LED_STATUS_G, OUTPUT);
  if (PIN_LED_STATUS_B != false) pinMode(PIN_LED_STATUS_B, OUTPUT);
  setColor('s', 0, 1, 0); 
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
  }
  

  if ( digitalRead(PIN_ARM_LOCK_CHECK) == LOW ) { // arm is ready to shoot
    
    setColor('s', 0, 1, 0); // green led
    servoArmLock.write(0); // lock arm
    servoRotate.write(90); // stop servo rotate
    delay(100);
    servoRotate.write(104); // rollback a little bit the rubber
    delay(500);
    servoRotate.write(90); // stop servo rotate
  
    if ( digitalRead(PIN_DOG_BUTTON) == LOW ) { // dog button pushed, arm is loading 
      
      //Serial.println(rotationTime);
      setColor('s', 1, 0, 0); // red led
      servoRotate.write(81); // start to strech a rubber in catapult
      delay(rotationTime * 1000);
      servoArmLock.write(180); // arm release lock
      //Serial.println("servo triggered");
      servoRotate.write(90); // stop servo rotate
      delay(2000);
      
    }
    
  } else { // arm is lowering
    setColor('s', 0, 1, 0); // blinking green led
    delay(500);
    setColor('s', 0, 0, 0);
    delay(500);
    servoArmLock.write(180); // open arm lock
    servoRotate.write(104);
  }
  
  delay(100);
}

void setColor(char ledType, int red, int green, int blue) {
  if (ledType == 's') { // status led
    digitalWrite(PIN_LED_STATUS_R, red);
    digitalWrite(PIN_LED_STATUS_G, green);
    if (PIN_LED_STATUS_B != false) digitalWrite(PIN_LED_STATUS_B, blue);    
  } else { // distance led
    analogWrite(PIN_LED_DISTANCE_R, red);
    analogWrite(PIN_LED_DISTANCE_G, green);
    if (PIN_LED_DISTANCE_B != false) analogWrite(PIN_LED_DISTANCE_B, blue);      
  }
}

/*
 * @rotationTime is taken from getRotationTime() (seconds)
 * 
 */
void setDistanceLedColor(int rotationTime) {
  switch (rotationTime) {
    case 5:
      setColor('d', 0, 50, 0); // green
      break;
    case 6:
      setColor('d', 100, 20, 0); // light orange
      break;
    case 7:
      setColor('d', 255, 20, 0); // strong orange
      break;
    case 8:
      setColor('d', 100, 0, 0); // red
      break;
  }  
}

int getRotationTime(int potentiometerValue) {
  if ( potentiometerValue >= 0 && potentiometerValue <= 99 ) { // 0 min distance
    rotationTime = 5; // seconds
  } else if ( potentiometerValue >= 100 && potentiometerValue <= 449 ) { // 1 distance
    rotationTime = 6;
  } else if ( potentiometerValue >= 450 && potentiometerValue <= 799 ) { // 2 distance
    rotationTime = 7;
  } else if ( potentiometerValue >= 800 && potentiometerValue <= 1023 ) { // 3 max distance
    rotationTime = 8;
  }   
  return rotationTime;
}

