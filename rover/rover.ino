/////////////////////////////////////////////////////
//                  UBICOMP 2016                   //
//              Team 12_02 presents:               //
//    /\   :: SUPER COOL ROBOT WAGENTJE ::   /\    //
//   /__\                                   /__\   //
//  /\  /\       Luuk Hafkamp              /\  /\  //
// /__\/__\      Franck van Domburg       /__\/__\ //
//               Mark van Dijken                   //
//                                                 //
/////////////////////////////////////////////////////

// *************
// * VARIABLES *
// *************

// Pairs for the pin numbers, by pairing them 
// we make sure not to use the wrong combos
const int LF[2] = {3, 4};  // Left-Forward   (LF)
const int LB[2] = {5, 6};  // Left-Backward  (LB)
const int RF[2] = {7, 8};  // Right-Forward  (RF)
const int RB[2] = {9, 10}; // Right-Backward (RB)

// Pins for distance sensor front
const int frontSensorTrig = A0;
const int frontSensorEcho = A1;

// Pins for back range meter
const int backSensorTrig = A2;
const int backSensorEcho = A3;

// Pins for light sensor
const int lightSensorPin = A4;

// Pin for sound thingy
const int speakerPin = 12;


// *********
// * SETUP *
// *********

void setup() {
  // start serial monitor thingy
  Serial.begin(9600);
  
  // Left-Forward (LF)
  pinMode(3, OUTPUT);   // Pin 3  = Blue wire => transisor A
  pinMode(4, OUTPUT);   // Pin 4  = Blue wire => transisor D
  
  // Left-Backward (LB)
  pinMode(5, OUTPUT);   // Pin 5  = Green wire => transisor B
  pinMode(6, OUTPUT);   // Pin 6  = Green wire => transisor C
  
  // Right-Forward (RF)
  pinMode(7, OUTPUT);   // Pin 7  = Yellow wire => transisor E
  pinMode(8, OUTPUT);   // Pin 8  = Yellow wire => transisor H
  
  // Right-Backward (RB)
  pinMode(9, OUTPUT);   // Pin 9  = White wire => transisor F
  pinMode(10, OUTPUT);  // Pin 10 = White wire => transisor G

  // font sensor pins
  pinMode(frontSensorTrig, OUTPUT);
  pinMode(frontSensorEcho, INPUT);
  // back sensor pins
  pinMode(backSensorTrig, OUTPUT);
  pinMode(backSensorEcho, INPUT);
  // speaker pin
  pinMode(speakerPin, OUTPUT);
 
}

// ****************
// * MAIN LOOP *
// ****************
void loop() {
  Serial.println(sensePickup());  
  
  boolean driving = false;

  playDrivingSound();
  
  if ( senseBack() < 200 ) {
    Serial.println("Detected movement in back");
    driving = true;
  }
  while( sensePickup() > 200 ) {
    Serial.println("I was picked up! HALP");
    playPickupSound();
    if (driving) {
      driving = false;
    }
    halt();
  }

  while(driving) {
    Serial.println("driving");
    driveForward();
    playDrivingSound();
    if ( senseFront() < 150) {
      Serial.println("Detected movement in front while driving");
      halt();
      while( senseFront() < 150 ) {
        Serial.println("Rotating");
        rotateClockwise();
      }
      halt();
    }

    while( sensePickup() > 200) {
      Serial.println("I was picked up! HALP");
      playPickupSound();
      if (driving) {
        driving = false;
      }
      halt();
    }
  }
}


// ******************
// * MOTION SENSORS *
// ******************
long senseFront() {
  long duration, distance;
  
  digitalWrite(frontSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(frontSensorTrig, HIGH);
  delayMicroseconds(100);
  digitalWrite(frontSensorTrig, LOW);
  duration = pulseIn(frontSensorEcho, HIGH); // return the time between high low
  distance = duration / 20.9 / 2; // distacne in cm
  
  return distance;
}
long senseBack() {
  long duration, distance;
  
  digitalWrite(backSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(backSensorTrig, HIGH);
  delayMicroseconds(100);
  digitalWrite(backSensorTrig, LOW);
  duration = pulseIn(backSensorEcho, HIGH); // return the time between high low
  distance = duration / 20.9 / 2; // // distacne in cm
  
  return distance;
}

// the the sensor code was partially copied from:
// http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/step3/Upload-the-sketch/

// ******************
// * PICK-UP SENSOR *
// ******************
int sensePickup() {
  int lightSensorValue = 1023;
  lightSensorValue = analogRead(lightSensorPin);
  Serial.println(lightSensorValue);
  return lightSensorValue;// > 300;
}

// ****************
// * DRIVING PART *
// ****************
void driveForward() {
  delay(100);
  activatePins(LF);
  activatePins(RF);
}
void driveBackward() {
  delay(100);
  activatePins(LB);
  activatePins(RB);
}
void rotateClockwise() {
  delay(100);
  activatePins(LF);
  activatePins(RB);
}
void rotateCounterClockwise() {
  delay(100);
  activatePins(LB);
  activatePins(RF);
}

// basic pin activation function
void activatePins(const int pair[]) {
  // CKECK IF THIS WORKS
  if (checkIfSafe(pair)) {
    digitalWrite(pair[0], HIGH);
    digitalWrite(pair[1], HIGH);
  }
}

// Check if the given pin pair can safely 
// be used without short circuiting
boolean checkIfSafe(const int pair[]) {

  // we start with the asumption 
  // that the pins are unsafe to use;
  boolean safeToUse = false;

  // check the first value of the pair 
  // to identify which other pins we need to check 
  // for example: if we want to activate pin 3 and 4, 
  // we must first assure that pin 5 and 6 are LOW
  switch (pair[0]) {
    case 3:
      // if the oposite pins are LOW then it's safe to use the given pair
      if (digitalRead(5) == LOW && digitalRead(6) == LOW) 
      {
        safeToUse = true;
      }
      break;
    case 5:
      if (digitalRead(3) == LOW && digitalRead(4) == LOW) 
      {
        safeToUse = true;
      }
      break;
    case 7:
      if (digitalRead(9) == LOW && digitalRead(10) == LOW) 
      {
        safeToUse = true;
      }
      break;
    case 9:
      if (digitalRead(7) == LOW && digitalRead(8) == LOW) 
      {
        safeToUse = true;
      }
      break;
    default:
      // if nothing else matches something went wrong probably.
      Serial.println("safeToUse() says: This shouldn't happen");
    break;
  }

  // return whatever the value of safeToUse is
  return safeToUse;
}

// Set every pin to LOW
void halt() {
  for(int i = 3; i <= 10; i++) {
    digitalWrite(i, LOW);
  }
  Serial.println("All engines halted");
  delay(100); // wait a bit to make sure everything is off
}

// ****************
// * SOUND PART *
// ****************

// used notes driving sound
#define D1  36.71
#define D3  146.83
#define D4  293.66

// used notes pickup sound
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32

// duration of notes
#define BPM 120       // you can change this value changing all the others
#define Q 60000/BPM   // quarter 1/4 
#define S Q/4         // sixteenth 1/16 

void playDrivingSound() {
  tone(speakerPin, D3, S); 
  delay(10+S);
  tone(speakerPin, D4, S); 
  delay(10+S);
  tone(speakerPin, D1, S); 
  delay(100+S);
  tone(speakerPin, D4, S); 
  delay(200+S); 
}
void playPickupSound() {
  tone(speakerPin, LA6, S); 
  delay(1+S);
  tone(speakerPin, Bb6, S); 
  delay(1+S);
  tone(speakerPin, B6, S); 
  delay(1+S);
  tone(speakerPin, C7, S); 
  delay(1+S);
  tone(speakerPin, Db7, S); 
  delay(1+S);
  tone(speakerPin, D7, S); 
  delay(1+S);
}

// Sound playing code was taken from: 
// http://www.instructables.com/id/How-to-easily-play-music-with-buzzer-on-arduino-Th/
