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

// Pins for light sensor
const int lightSensorPin = A4; // select the input pin for ldr

// Pin for sound thingy
const int speakerPin = 12;

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
    while( senseFront() ) {
      rotateClockwise();
    }
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
      while( senseFront() ) {
        rotateClockwise();
      }
      halt();
    }

    while( sensePickup() ) {
      Serial.println("I was picked up! HALP");
      playPickupSound();
      if (driving) {
        driving = false;
      }
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
  int lightSensorValue = 1023;
  lightSensorValue = analogRead(lightSensorPin);
  return lightSensorValue > 370;
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
#define C0 16.35
#define Db0 17.32
#define D0  18.35
#define Eb0 19.45
#define E0  20.60
#define F0  21.83
#define Gb0 23.12
#define G0  24.50
#define Ab0 25.96
#define LA0 27.50
#define Bb0 29.14
#define B0  30.87
#define C1  32.70
#define Db1 34.65
#define D1  36.71
#define Eb1 38.89
#define E1  41.20
#define F1  43.65
#define Gb1 46.25
#define G1  49.00
#define Ab1 51.91
#define LA1 55.00
#define Bb1 58.27
#define B1  61.74
#define C2  65.41
#define Db2 69.30
#define D2  73.42
#define Eb2 77.78
#define E2  82.41
#define F2  87.31
#define Gb2 92.50
#define G2  98.00
#define Ab2 103.83
#define LA2 110.00
#define Bb2 116.54
#define B2  123.47
#define C3  130.81
#define Db3 138.59
#define D3  146.83
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Gb3 185.00
#define G3  196.00
#define Ab3 207.65
#define LA3 220.00
#define Bb3 233.08
#define B3  246.94
#define C4  261.63
#define Db4 277.18
#define D4  293.66
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Gb4 369.99
#define G4  392.00
#define Ab4 415.30
#define LA4 440.00
#define Bb4 466.16
#define B4  493.88
#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32
#define Eb7 2489.02
#define E7  2637.02
#define F7  2793.83
#define Gb7 2959.96
#define G7  3135.96
#define Ab7 3322.44
#define LA7 3520.01
#define Bb7 3729.31
#define B7  3951.07
#define C8  4186.01
#define Db8 4434.92
#define D8  4698.64
#define Eb8 4978.03
// DURATION OF THE NOTES 
#define BPM 120    //  you can change this value changing all the others
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

void playDrivingSound() {
  tone(speakerPin, LA6, Q/4); 
  delay(1+Q/4);
  tone(speakerPin, Bb6, Q/4); 
  delay(1+Q/4);
  tone(speakerPin, B6, Q/4); 
  delay(1+Q/4);
  tone(speakerPin, C7, Q/4); 
  delay(1+Q/4);
  tone(speakerPin, Db7, Q/4); 
  delay(1+Q/4);
  tone(speakerPin, D7, Q/4); 
  delay(1+Q/4);
}
void playPickupSound() {
  tone(speakerPin, D3, Q/4); 
  delay(10+Q/4);
  tone(speakerPin, D4, Q/4); 
  delay(10+Q/4);
  tone(speakerPin, D1, Q/4); 
  delay(100+Q/4);
  tone(speakerPin, D4, Q/4); 
  delay(200+Q/4);
  tone(speakerPin, D1, Q/4); 
  delay(10+Q/4);
  tone(speakerPin, D1, Q/4); 
  delay(10+Q/4);
  tone(speakerPin, D4, Q/4); 
  delay(200+Q/4);
  tone(speakerPin, D4, Q/4); 
  delay(200+Q/4);
  tone(speakerPin, D3, Q/4); 
  delay(10+Q/4);
  tone(speakerPin, D3, Q/4); 
  delay(10+Q/4);
  tone(speakerPin, D4, Q/4); 
  delay(200+Q/4);
  tone(speakerPin, D1, Q/4); 
}

// the the sensor code was partially copied from:
// http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/step3/Upload-the-sketch/

// Light detection && Sound playing from: 
// http://www.instructables.com/id/How-to-easily-play-music-with-buzzer-on-arduino-Th/

