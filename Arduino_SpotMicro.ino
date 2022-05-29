#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>
#include "settings.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

String currState = "init";


int degToPulse(int deg) {
  return map(deg, 0, 180, SERVOMIN, SERVOMAX);
}


void calculateCoeffs(const int checkState[4][3]) {
  
  bool isCalibrating = true;

  while (isCalibrating) {
    Serial.print("Input leg -> ");
    int leg = Serial.parseInt();
    Serial.print(" input serv ->");
    int serv = Serial.parseInt();

    int degree = checkState[leg][serv];
    int pulse = degToPulse(degree);
    pwm.setPWM(legPins[leg][serv], 0, pulse);
    Serial.print("leg ");
    Serial.print(leg);
    Serial.print(" ");
    Serial.print("serv ");
    Serial.print(serv);
    Serial.print(" -> ");
    Serial.println(degree);

    Serial.println("Wanna remember this value?");
    Serial.println("If yes, enter 1, otherwise 0");
    int ans = Serial.parseInt();
    if (ans) {
      EEPROM.put((leg*3 + serv)*2, degree);
    }
    else {
      while(!ans) {
        if (leg % 2 == 0 && serv == 0) {
          degree += 5;
        } else {
          degree -= 5;
        }
        pulse = degToPulse(degree);
        pwm.setPWM(legPins[leg][serv], 0, pulse);

        Serial.print("leg ");
        Serial.print(leg);
        Serial.print(" ");
        Serial.print("serv ");
        Serial.print(serv);
        Serial.print(" -> ");
        Serial.println(degree);
    
        Serial.println("Wanna remember this value?");
        Serial.println("If yes, enter 1, otherwise 0");
        ans = Serial.parseInt();
        if (ans) {
          EEPROM.put((leg*3 + serv)*2, degree);
        }
      }
    }
  }
  readCoeffs();
}


void readCoeffs() {
  int q = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      EEPROM.get(q, legCoeffs[i][j]);
      Serial.println(legCoeffs[i][j]);
      q+=2;
    }
  }
}


void setInitState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  // set the position of the lower servos
  for(int i = 0; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  delay(700);

  // set the position of the middle servos
  for(int i = 0; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  delay(700); 

  // set the position of the upper servos
  for(int i = 0; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
  }

  currState = "init";
}


void setSittingState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {
  
  // set the position of the back lower servos
  for(int i = 2; i < 4; i++) {
    int to = degToPulse(abs(state[i][0] - coeffs[i][0]));
    int from = degToPulse(abs(stayState[i][0] - coeffs[i][0]));
    if (to > from) {
      for (int j = from; j <= to; j+=15) {
        pwm.setPWM(legPins[i][0], 0, j);
        delay(50);
      }
    }
    else {
      for (int j = from; j >= to; j-=15) {
        pwm.setPWM(legPins[i][0], 0, j);
        delay(50);
      }
    }
  }

  delay(100);

  // set the position of the back middle servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
    delay(100);
  }

  delay(200);

  // set the position of the back upper servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
    delay(100);
  }

  delay(200);

  // set the position of the front lower servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
    delay(100);
  }

  // set the position of the front middle servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
    delay(100);
  }

  // set the position of the front upper servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
    delay(100);
  }

  currState = "sitting";
}


void setLieState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  if(currState == "staying") {
    setSittingState(sitState, coeffs, pwm);
  }

  // set the position of the front lower servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  // set the position of the front middle servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }


  // set the position of the front upper servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
  }

  // set the position of the front lower servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  // set the position of the front middle servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  // set the position of the front upper servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
  }

  currState = "lying";
}


void setStayState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  if(currState == "sitting") {
    setLieState(lieState, coeffs, pwm);
  }

  // set the position of the back lower servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  // set the position of the back middle servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  // set the position of the back upper servos
  for(int i = 2; i < 4; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
  }

  delay(500);

  // set the position of the front lower servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  // set the position of the front middle servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  // set the position of the front upper servos
  for(int i = 0; i < 2; i++) {
    int pulse = degToPulse(abs(state[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][2], 0, pulse);
  }

  delay(700);

  currState = "staying";
}


void setStayAfterLeftTiltState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  // set the position of the lower right servos
  for(int i = 0; i < 4; i += 2) {
    int pulse = degToPulse(abs(lieState[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  // set the position of the upper right servos
  for(int i = 0; i < 4; i += 2) {
    int pulse = degToPulse(abs(lieState[i][2] - coeffs[i][2]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  delay(100);

  setStayState(state, coeffs, pwm);

  currState = "staying";
}


void setStayAfterRightTiltState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  // set the position of the lower left servos
  for(int i = 1; i < 4; i += 2) {
    int pulse = degToPulse(abs(lieState[i][0] - coeffs[i][0]));
    pwm.setPWM(legPins[i][0], 0, pulse);
  }

  // set the position of the middle left servos
  for(int i = 1; i < 4; i += 2) {
    int pulse = degToPulse(abs(lieState[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  delay(100);

  setStayState(state, coeffs, pwm);

  currState = "staying";
}


void setForwardState(const int maxAngle[4][3], const int minAngle[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {
  if (currState != "staying") {
    setStayState(stayState, coeffs, pwm);
  }

  // set the position of the middle servos
  for (int i = 0; i < 4; i++) {
    int pulse = degToPulse(abs(maxAngle[i][1] - coeffs[i][1]));
    pwm.setPWM(legPins[i][1], 0, pulse);
  }

  // loop for walking
  for (int i = 0; i < 10; i++) {

    int pulse = degToPulse(abs(maxAngle[0][0] - coeffs[0][0]));
    pwm.setPWM(legPins[0][0], 0, pulse);
    
    pulse = degToPulse(abs(maxAngle[3][0] - coeffs[3][0]));
    pwm.setPWM(legPins[3][0], 0, pulse);

    pulse = degToPulse(abs(minAngle[1][0] - coeffs[1][0]));
    pwm.setPWM(legPins[1][0], 0, pulse);

    pulse = degToPulse(abs(minAngle[2][0] - coeffs[2][0]));
    pwm.setPWM(legPins[2][0], 0, pulse);

    delay(300);

    pulse = degToPulse(abs(minAngle[0][0] - coeffs[0][0]));
    pwm.setPWM(legPins[0][0], 0, pulse);

    pulse = degToPulse(abs(minAngle[3][0] - coeffs[3][0]));
    pwm.setPWM(legPins[3][0], 0, pulse);

    pulse = degToPulse(abs(maxAngle[1][0] - coeffs[1][0]));
    pwm.setPWM(legPins[1][0], 0, pulse);

    pulse = degToPulse(abs(maxAngle[2][0] - coeffs[2][0]));
    pwm.setPWM(legPins[2][0], 0, pulse);

    delay(300);

  }
  setStayState(stayState, coeffs, pwm);

  currState = "staying";
}


void setBackwardState(const int maxAngle[4][3], const int minAngle[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {
  if (currState != "staying") {
    setStayState(stayState, coeffs, pwm);
  }

  // loop for walking
  for (int i = 0; i < 10; i++) {

    int pulse = degToPulse(abs(maxAngle[0][1] - coeffs[0][1]));
    pwm.setPWM(legPins[0][1], 0, pulse);
    
    pulse = degToPulse(abs(maxAngle[3][1] - coeffs[3][1]));
    pwm.setPWM(legPins[3][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[1][1] - coeffs[1][1]));
    pwm.setPWM(legPins[1][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[2][1] - coeffs[2][1]));
    pwm.setPWM(legPins[2][1], 0, pulse);

    delay(250);

    pulse = degToPulse(abs(minAngle[0][1] - coeffs[0][1]));
    pwm.setPWM(legPins[0][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[3][1] - coeffs[3][1]));
    pwm.setPWM(legPins[3][1], 0, pulse);

    pulse = degToPulse(abs(maxAngle[1][1] - coeffs[1][1]));
    pwm.setPWM(legPins[1][1], 0, pulse);

    pulse = degToPulse(abs(maxAngle[2][1] - coeffs[2][1]));
    pwm.setPWM(legPins[2][1], 0, pulse);

    delay(250);

  }
  setStayState(stayState, coeffs, pwm);

  currState = "staying";
}


void setTurnRightState(const int maxAngle[4][3], const int minAngle[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {
  if (currState != "staying") {
    setStayState(stayState, coeffs, pwm);
  }

  // loop for walking
  for (int i = 0; i < 10; i++) {

    int pulse = degToPulse(abs(maxAngle[2][1] - coeffs[2][1]));
    pwm.setPWM(legPins[2][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[2][2] - coeffs[2][2]));
    pwm.setPWM(legPins[2][2], 0, pulse);

    pulse = degToPulse(abs(maxAngle[3][0] - coeffs[3][0]));
    pwm.setPWM(legPins[3][0], 0, pulse);

    pulse = degToPulse(abs(maxAngle[3][1] - coeffs[3][1]));
    pwm.setPWM(legPins[3][1], 0, pulse);

    pulse = degToPulse(abs(maxAngle[3][2] - coeffs[3][2]));
    pwm.setPWM(legPins[3][2], 0, pulse);
    
    delay(150);

    pulse = degToPulse(abs(minAngle[2][1] - coeffs[2][1]));
    pwm.setPWM(legPins[2][1], 0, pulse);

    pulse = degToPulse(abs(maxAngle[2][2] - coeffs[2][2]));
    pwm.setPWM(legPins[2][2], 0, pulse);

    pulse = degToPulse(abs(minAngle[3][0] - coeffs[3][0]));
    pwm.setPWM(legPins[3][0], 0, pulse);

    pulse = degToPulse(abs(minAngle[3][1] - coeffs[3][1]));
    pwm.setPWM(legPins[3][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[3][2] - coeffs[3][2]));
    pwm.setPWM(legPins[3][2], 0, pulse);

    delay(150);
  }
  setStayState(stayState, coeffs, pwm);

  currState = "staying";
}


void setTurnLeftState(const int maxAngle[4][3], const int minAngle[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {
  if (currState != "staying") {
    setStayState(stayState, coeffs, pwm);
  }

  // loop for walking
  for (int i = 0; i < 10; i++) {

    int pulse = degToPulse(abs(minAngle[0][0] - coeffs[0][0]));
    pwm.setPWM(legPins[0][0], 0, pulse);

    pulse = degToPulse(abs(minAngle[0][1] - coeffs[0][1]));
    pwm.setPWM(legPins[0][1], 0, pulse);

    pulse = degToPulse(abs(maxAngle[1][0] - coeffs[1][0]));
    pwm.setPWM(legPins[1][0], 0, pulse);

    pulse = degToPulse(abs(maxAngle[1][1] - coeffs[1][1]));
    pwm.setPWM(legPins[1][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[0][2] - coeffs[0][2]));
    pwm.setPWM(legPins[0][2], 0, pulse);
    
    delay(150);

    pulse = degToPulse(abs(maxAngle[0][0] - coeffs[0][0]));
    pwm.setPWM(legPins[0][0], 0, pulse);

    pulse = degToPulse(abs(maxAngle[0][1] - coeffs[0][1]));
    pwm.setPWM(legPins[0][1], 0, pulse);

    pulse = degToPulse(abs(minAngle[1][0] - coeffs[1][0]));
    pwm.setPWM(legPins[1][0], 0, pulse);

    pulse = degToPulse(abs(minAngle[1][1] - coeffs[1][1]));
    pwm.setPWM(legPins[1][1], 0, pulse);

    pulse = degToPulse(abs(maxAngle[0][2] - coeffs[0][2]));
    pwm.setPWM(legPins[0][2], 0, pulse);

    delay(150);
  }
  setStayState(stayState, coeffs, pwm);

  currState = "staying";
}


void setTiltRightState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  int pulse = degToPulse(abs(state[0][0] - coeffs[0][0]));
  pwm.setPWM(legPins[0][0], 0, pulse);

  pulse = degToPulse(abs(state[2][0] - coeffs[2][0]));
  pwm.setPWM(legPins[2][0], 0, pulse);

  pulse = degToPulse(abs(state[0][1] - coeffs[0][1]));
  pwm.setPWM(legPins[0][1], 0, pulse);

  pulse = degToPulse(abs(state[2][1] - coeffs[2][1]));
  pwm.setPWM(legPins[2][1], 0, pulse);

  pulse = degToPulse(abs(state[0][2] - coeffs[0][2]));
  pwm.setPWM(legPins[0][2], 0, pulse);

  pulse = degToPulse(abs(state[2][2] - coeffs[2][2]));
  pwm.setPWM(legPins[2][2], 0, pulse);
  

  pulse = degToPulse(abs(state[1][0] - coeffs[1][0]));
  pwm.setPWM(legPins[1][0], 0, pulse);

  pulse = degToPulse(abs(state[3][0] - coeffs[3][0]));
  pwm.setPWM(legPins[3][0], 0, pulse);

  pulse = degToPulse(abs(state[1][1] - coeffs[1][1]));
  pwm.setPWM(legPins[1][1], 0, pulse);

  pulse = degToPulse(abs(state[3][1] - coeffs[3][1]));
  pwm.setPWM(legPins[3][1], 0, pulse);

  pulse = degToPulse(abs(state[1][2] - coeffs[1][2]));
  pwm.setPWM(legPins[1][2], 0, pulse);

  pulse = degToPulse(abs(state[3][2] - coeffs[3][2]));
  pwm.setPWM(legPins[3][2], 0, pulse);

  
  currState = "tiltRight";
}


void setTiltLeftState(const int state[4][3], const int coeffs[4][3], Adafruit_PWMServoDriver pwm) {

  int pulse = degToPulse(abs(state[1][0] - coeffs[1][0]));
  pwm.setPWM(legPins[1][0], 0, pulse);

  pulse = degToPulse(abs(state[3][0] - coeffs[3][0]));
  pwm.setPWM(legPins[3][0], 0, pulse);

  pulse = degToPulse(abs(state[1][1] - coeffs[1][1]));
  pwm.setPWM(legPins[1][1], 0, pulse);

  pulse = degToPulse(abs(state[3][1] - coeffs[3][1]));
  pwm.setPWM(legPins[3][1], 0, pulse);

  pulse = degToPulse(abs(state[1][2] - coeffs[1][2]));
  pwm.setPWM(legPins[1][2], 0, pulse);

  pulse = degToPulse(abs(state[3][2] - coeffs[3][2]));
  pwm.setPWM(legPins[3][2], 0, pulse);
  

  pulse = degToPulse(abs(state[0][0] - coeffs[0][0]));
  pwm.setPWM(legPins[0][0], 0, pulse);

  pulse = degToPulse(abs(state[2][0] - coeffs[2][0]));
  pwm.setPWM(legPins[2][0], 0, pulse);

  pulse = degToPulse(abs(state[0][1] - coeffs[0][1]));
  pwm.setPWM(legPins[0][1], 0, pulse);

  pulse = degToPulse(abs(state[2][1] - coeffs[2][1]));
  pwm.setPWM(legPins[2][1], 0, pulse);

  pulse = degToPulse(abs(state[0][2] - coeffs[0][2]));
  pwm.setPWM(legPins[0][2], 0, pulse);

  pulse = degToPulse(abs(state[2][2] - coeffs[2][2]));
  pwm.setPWM(legPins[2][2], 0, pulse);

  currState = "tiltLeft";
}


// Function for testing robot servos
void setServoFromSerial(const int coeffs[4][3]) {
  // Drive each servo one at a time using setPWM()
  bool isContinuing = true;

  while (isContinuing) {
    Serial.print("Input leg -> ");
    int leg = Serial.parseInt();
    Serial.print(" input serv ->");
    int serv = Serial.parseInt();
    Serial.println(" input degree ");
    int degree = Serial.parseInt();
    
    int pulse = degToPulse(abs(degree - coeffs[leg][serv]));
    pwm.setPWM(legPins[leg][serv], 0, pulse);
    Serial.print("leg ");
    Serial.print(leg);
    Serial.print(" ");
    Serial.print("serv ");
    Serial.print(serv);
    Serial.print(" -> ");
    Serial.println(abs(degree - coeffs[leg][serv]));
  }
}


// Function for setting movement patterns
void setStateFromSerial() {
  
  Serial.println("Choose your state from ");
  for(int i = 0; i < sizeof(statesList)/sizeof(*statesList); i++) {
    Serial.print(statesList[i]);
    Serial.print(", ");
  }
  Serial.println();

  int wantedState = Serial.parseInt();


  if(wantedState == 1) {
    if (currState == "tiltLeft") {
      setStayAfterLeftTiltState(stayState, legCoeffs, pwm);
    }
    else if (currState == "tiltRight") {
      setStayAfterRightTiltState(stayState, legCoeffs, pwm);
    }
    else {
      setStayState(stayState, legCoeffs, pwm);
    }
  } 
  else if(wantedState == 2) {
    setSittingState(sitState, legCoeffs, pwm);
  }
  else if(wantedState == 3) {
    setForwardState(runForwardMaxAngleState, runForwardMinAngleState, legCoeffs, pwm);
  }
  else if(wantedState == 4) {
    setBackwardState(runBackwardMaxAngleState, runBackwardMinAngleState, legCoeffs, pwm);
  }
  else if(wantedState == 5) {
    setTurnRightState(turnRightMaxAngleState, turnRightMinAngleState, legCoeffs, pwm);
  }
  else if(wantedState == 6) {
    setTurnLeftState(turnLeftMaxAngleState, turnLeftMinAngleState, legCoeffs, pwm);
  }
  else if(wantedState == 7) {
   setTiltRightState(tiltRightState, legCoeffs, pwm);
  }
  else if(wantedState == 8) {
    setTiltLeftState(tiltLeftState, legCoeffs, pwm);
  }
  else {
    setLieState(lieState, legCoeffs, pwm);
  }

  Serial.print(wantedState);
  Serial.println(" state successfuly set");
}


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100000000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);

  Serial.println("Wanna calibrate servos?");
  Serial.println("If yes, enter 1, otherwise 0");
  
  int ans = Serial.parseInt();
  if (ans) {
    calculateCoeffs(calibrateState);
  } else {
    readCoeffs();
  }

  Serial.println("Wanna set angles manually?");
  Serial.println("If yes, enter 1, otherwise 0");

  ans = Serial.parseInt();
  if (ans) {
    setServoFromSerial(legCoeffs);
  }

  Serial.println("Wanna proceed to controlling of robot?");
  Serial.println("If yes, enter 1, otherwise 0");
  ans = Serial.parseInt();
  if (ans) {
    setInitState(initState, legCoeffs, pwm); 
  }
}


void loop() {
  setStateFromSerial();
}
