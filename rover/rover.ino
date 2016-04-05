// SUPER COOL ROBOT WAGENTJE
// UBICOMP 2016 - Team 12_02: 
//    Mark van Dijken
//    Luuk Hafkamp
//    Franck van Domburg


// TODO:
// 0. test current code
// 1. Hook up sensor code
// 2. ????
// 3. PROFIT!


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

// Begin stationary
    
// if backsensor detects someone
  // movement forward until backsensor doesn't detect anyone

// if frontsensor detects obstacle
  // rotate until no obstacles are in sight then move forward for n seconds

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
  pinMode(backSensorEcho, INPUT);
  // back sensor pins
  pinMode(backSensorTrig, OUTPUT);
  pinMode(backSensorEcho, INPUT);
}

// ****************
// * MAIN LOOP *
// ****************
void loop() {
  boolean driving = false;
  
  if ( senseFront() ) {
    Serial.println("Detected movement in front");
    rotateClockwise();
    delay(400);
    halt();
  }
  if ( senseBack() ) {
    Serial.println("Detected movement in back");
    driving = true;
  }

  while(driving) {
    Serial.println("driving");
    driveForward();
    playDrivingSound();
    
    if ( senseFront() ) {
      Serial.println("Detected movement in front");
      halt();
      rotateClockwise();
      delay(400);
      halt();
    }

    while( sensePickup() ) {
      playPickupSound();
      halt();
    }
  }
}


// ****************
// * FRONT SENSOR *
// ****************
boolean senseFront() {
  long duration, distance;
  digitalWrite(frontSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(frontSensorTrig, HIGH);
  delayMicroseconds(100);
  digitalWrite(frontSensorTrig, LOW);
  duration = pulseIn(frontSensorEcho, HIGH); // return the time between high low
  distance = duration / 20.9;
  return distance < 150;
}

// ****************
// * BACK SENSOR *
// ****************
boolean senseBack() {
  long duration, distance;
  digitalWrite(backSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(backSensorTrig, HIGH);
  delayMicroseconds(100);
  digitalWrite(backSensorTrig, LOW);
  duration = pulseIn(backSensorEcho, HIGH); // return the time between high low
  distance = duration / 20.9;
  return distance < 300;
}

// ******************
// * PICK-UP SENSOR *
// ******************
boolean sensePickup() {
  return false;
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

// the the sensor code was partially copied from:
//  http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/step3/Upload-the-sketch/
