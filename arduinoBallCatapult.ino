//String readString;
#include <Servo.h>
Servo rotatingServo;  // create servo object to control a servo

int rotationTime = 2; // min rotating time
int potentiometerValue = 0;
int oldPotentiometerValue = 0;

void setup() {
  Serial.begin(9600);
  rotatingServo.write(90); //set initial servo position
  rotatingServo.attach(3);  //the pin for the servo control
  Serial.println("catapult started"); // so I can keep track of what is loaded
}

void loop() {
/*  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }*/

int potentiometerValue = analogRead(A0);

// 1023-800 : 0
// 799-450 : 1
// 449-100 : 2
// 99-0 : 3

// 104 servo max left
// 90 stop servo
// 81 sero max right
Serial.println(potentiometerValue);

/*  if (readString.length() >0) {
    Serial.println(readString);  //so you can see the captured string
    int n = readString.toInt();  //convert readString into a number

    // auto select appropriate value, copied from someone elses code.
    if(n >= 500)
    {
      Serial.print("writing Microseconds: ");
      Serial.println(n);
      myservo.writeMicroseconds(n);
    }
    else
    {   
      Serial.print("writing Angle: ");
      Serial.println(n);
      myservo.write(n);
    }

    readString=""; //empty for next input
  }*/

if ( potentiometerValue != oldPotentiometerValue ) {

  oldPotentiometerValue = potentiometerValue;

  if ( potentiometerValue <= 1023 && potentiometerValue >= 800 ) { // 0 min distance
    rotationTime = 2; // 2 sec
  } else if ( potentiometerValue <= 799 && potentiometerValue >= 450 ) { // 1 distance
    rotationTime = 3; // 3 sec
  } else if ( potentiometerValue <= 449 && potentiometerValue >= 100 ) { // 2 distance
    rotationTime = 4; // 4 sec
  } else if ( potentiometerValue <= 99 && potentiometerValue >= 0 ) { // 3 max distance
    rotationTime = 5; // 5 sec
  }

  rotatingServo.write(81);
  delay(rotationTime * 1000);
  rotatingServo.write(90); // stop servo rotating
  Serial.println(potentiometerValue);
  
}


  
  delay(1000);
}


