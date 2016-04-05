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


// Variables for rover state

// - Possible sensor states:
  // - back sensor: 
    //  0: no one is close
    //  1: someone is close
  // - front sensor: 
    //  0: way is clear
    //  1: obstacle in the way

// - Possible movement states of rover:
  //  0. stationary
  //  1. forward
  //  2. backward
  //  3. clockwise
  //  4. counterclockwise


// Begin stationary

// optionally? 
// periodically rotate to check suroundings
    
// if backsensor detects someone
  // movement forward until backsensor doesn't detect anyone

// if frontsensor detects obstacle
  // rotate until no obstacles are in sight then move forward for n seconds

void setup() {
  // start serial monitor thingy
  Serial.begin(9600);
  
  // Left-Forward (LF)
  pinMode(3, OUTPUT);   // A: Blue wire +
  pinMode(4, OUTPUT);   // D: Blue wire-
  
  // Left-Backward (LB)
  pinMode(5, OUTPUT);   // B: Green wire +
  pinMode(6, OUTPUT);   // C: Green wire -
  
  // Right-Forward (RF)
  pinMode(7, OUTPUT);   // E: Yellow wire +
  pinMode(8, OUTPUT);   // H: Yellow wire -
  
  // Right-Backward (RB)
  pinMode(9, OUTPUT);   // F: White wire +
  pinMode(10, OUTPUT);  // G: White wire -

  // font sensor pins
  pinMode(frontSensorTrig, OUTPUT);
  pinMode(backSensorEcho, INPUT);
  // back sensor pins
  pinMode(backSensorTrig, OUTPUT);
  pinMode(backSensorEcho, INPUT);
}

void loop() {
  if (pingBackSensor() < 300) { // This is where the LED On/Off happens
    Serial.println("something is close");
    driveForward();
  } else  {
    Serial.println("nothing in range");
    halt(); 
  }
  delay(500);

  
  Serial.println("LOOPED!");
  // halt();
  // driveForward();
  // delay(1000); // delays pause execution of code so wont work with motion sensors
  // halt();
  // driveBackward();
  // delay(1000); // delays pause execution of code so wont work with motion sensors
  // halt();
  // rotateClockwise();
  // delay(1000);
  // halt();
  // rotateCounterClockwise();
  // delay(1000);
}

// ****************
// * FRONT SENSOR *
// ****************
long pingBackSensor() {
  // back distance sensor 
  long duration, distance;
  digitalWrite(backSensorTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(backSensorTrig, HIGH);
  delayMicroseconds(100);
  digitalWrite(backSensorTrig, LOW);
  duration = pulseIn(backSensorEcho, HIGH); // return the time between high low
  distance = duration / 20.9;
  Serial.println("De distance");
  Serial.println(distance);
  return distance;
}

// ****************
// * DRIVING PART *
// ****************
void driveForward() {
  activatePins(LF);
  activatePins(RF); 
}
void driveBackward() {
  activatePins(LB);
  activatePins(RB);
}
void rotateClockwise() {
  activatePins(LF);
  activatePins(RB);
}
void rotateCounterClockwise() {
  activatePins(LB);
  activatePins(RF);
}

// basic pin activation function
void activatePins(const int pair[]) {
  // CKECK IF THIS WORKS
  checkIfSafe(pair);
  digitalWrite(pair[0], HIGH);
  digitalWrite(pair[1], HIGH);
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
    Serial.println(i);
  }
  Serial.println("All engines halted");
  delay(1000); // wait a bit to make sure everything is off
}

// the the sensor code was partially copied from:
//  
