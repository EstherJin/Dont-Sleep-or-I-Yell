#include<Wire.h>
#include <pitches.h>
#include <math.h>
#include <Servo.h>

const int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double currPitch = 0, currRoll = 0; 
double oldPitch = 0, oldRoll = 0;

int const buzzer = 4;
bool buttonPressed = true;
long lastUpdate = millis();

int numSeconds = 0;
int numSecondsUntilVibrate = 10;
int numSecondsUntilBuzz = 15;

Servo myServo;
int servoPos = 0;
long lastServoTurn = millis();
bool movingCC = false;

long beepTime = 0;
int notAwakeThreshold = 10;

int buttonPin = 7;

void setup(){
pinMode(buttonPin, INPUT_PULLUP);
myServo.attach(6);
pinMode(buzzer, OUTPUT);
Wire.begin();
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
Serial.begin(9600);
}


void loop(){
if(digitalRead(buttonPin) == LOW){
  while(digitalRead(buttonPin) == LOW){
  }
  buttonPressed = !buttonPressed;
}

if(buttonPressed == false){
  if(numSeconds >= numSecondsUntilVibrate){
    myServo.attach(6);
    if(numSeconds >= numSecondsUntilBuzz){
        if(millis() - beepTime >= 1100){
          beepTime = millis();
          tone(buzzer, NOTE_D5, 1000);
      }
    }
    if(millis() - lastServoTurn >= 20){
      if(servoPos == 0){
        servoPos = 180;
      }
      else{
        servoPos = 0;
      }
      myServo.write(servoPos);
    }
    notAwakeThreshold = 20;
  }
  else{
    notAwakeThreshold = 10;
    noTone(buzzer);
    myServo.detach();
  }
  
  if((millis() - lastUpdate) >= 1000){
  lastUpdate = millis();
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);

  int AcXoff,AcYoff,AcZoff;
  double t,tx,tf;

  //Acceleration data correction
  AcXoff = -950;
  AcYoff = -300;
  AcZoff = 0;

  //read accel data
  AcX=(Wire.read()<<8|Wire.read()) + AcXoff;
  AcY=(Wire.read()<<8|Wire.read()) + AcYoff;
  AcZ=(Wire.read()<<8|Wire.read()) + AcYoff;

  //get pitch/roll
  getAngle(AcX,AcY,AcZ);

  //send the data out the serial port
  Serial.print("Angle: ");
  Serial.print("Pitch = "); Serial.print(currPitch);
  Serial.print(" | Roll = "); Serial.println(currRoll);

  if(isPencilMoving(currRoll, currPitch, oldRoll, oldPitch) == true){
    numSeconds = 0;
    Serial.println("Still awake");
  }
  else{
    numSeconds++;
  }

  oldRoll = currRoll;
  oldPitch = currPitch;
  }
  }
  else{
    tone(buzzer, 0, 1);
  }
}

//convert the accel data to pitch/roll
void getAngle(int Vx,int Vy,int Vz) {
double x = Vx;
double y = Vy;
double z = Vz;

currPitch = atan(x/sqrt((y*y) + (z*z)));
currRoll = atan(y/sqrt((x*x) + (z*z)));
//convert radians into degrees
currPitch = currPitch * (180.0/3.14);
currRoll = currRoll * (180.0/3.14) ;
}

bool isPencilMoving(double currRoll0, double currPitch0, double oldRoll0, double oldPitch0){
  if(fabs(currRoll0 - oldRoll0) >= notAwakeThreshold or fabs(currPitch0 - oldPitch0) >= notAwakeThreshold){
    return true;
  }
  else{
    return false;
  }
}

void SongOfStorms(){
  int melody[]{
  NOTE_D5, NOTE_F5, NOTE_D6,
  NOTE_D5, NOTE_F5, NOTE_D6,
  NOTE_E6, NOTE_F6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_C6, NOTE_A5,
  NOTE_A5, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_A5,
  NOTE_A5, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_E5,
  NOTE_D5, NOTE_F5, NOTE_D6,
  NOTE_D5, NOTE_F5, NOTE_D6,
  NOTE_E6, NOTE_F6, NOTE_E6, NOTE_F6, NOTE_E6, NOTE_C6, NOTE_A5,
  NOTE_A5, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_A5,
  NOTE_A5, NOTE_D5, NOTE_F5, NOTE_G5, NOTE_E5 
  };

  int tempo[]{
  12, 12, 3,
  12, 12, 3,
  5, 10, 12, 12, 12, 12, 3,
  6, 6, 12, 12, 2,
  6, 6, 12, 12, 2,
  12, 12, 3,
  12, 12, 3,
  5, 10, 12, 12, 12, 12, 3,
  6, 6, 12, 12, 2,
  6, 6, 12, 12, 2
  };

int songDuration = sizeof(melody);
  for(int thisNote = 0;thisNote < songDuration; thisNote++){
    if(buttonPressed == false && numSeconds >= numSecondsUntilBuzz){
    int noteDuration = 1000/tempo[thisNote];
    tone(buzzer,melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration*1.80;
    delay(pauseBetweenNotes);
    }
  }
}

