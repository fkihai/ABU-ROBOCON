#include <Wire.h>

#define CW    1
#define CCW   2
#define Stop  3

/* AT COMMAND */
#define AT_RESET    (byte(0x00))
#define AT_RPS      (byte(0x01))

/* DIRECTION */
#define DIR_CW      (byte(0x06))
#define DIR_CCW     (byte(0x04))

/* COM */
#define ADDR        64
#define REVOLUTION 538

#define ENC_BE_CHA    22
#define ENC_BE_CHB    24
#define ENC_KA_CHA    31
#define ENC_KA_CHB    33
#define ENC_KI_CHA    35
#define ENC_KI_CHB    37

#define BE_RPWM_PIN      7
#define BE_LPWM_PIN      6
#define KA_RPWM_PIN      8
#define KA_LPWM_PIN      5
#define KI_RPWM_PIN      9
#define KI_LPWM_PIN     10

/*PULSE*/
volatile int32_t
ENC_BE_COUNT = 0, ENC_KA_COUNT = 0, ENC_KI_COUNT = 0;
volatile long int
LENC_BE_COUNT = 0, LENC_KA_COUNT = 0, LENC_KI_COUNT = 0;
float PWM_BE = 0, PWM_KA = 0, PWM_KI = 0;
long ltime1, ltime2, ltime3;
long Previousmillis;

/* SLAVE */
byte Value[3];
byte Ctrl[3];

void setup() {
  Serial.begin(9600);
  Wire.begin(ADDR);
  Wire.onReceive(receiveEvent);
  /* Encoder */
  pinMode(ENC_BE_CHA, INPUT_PULLUP);
  pinMode(ENC_KA_CHA, INPUT_PULLUP);
  pinMode(ENC_KI_CHA, INPUT_PULLUP);
  pinMode(ENC_BE_CHB, INPUT_PULLUP);
  pinMode(ENC_KA_CHB, INPUT_PULLUP);
  pinMode(ENC_KI_CHB, INPUT_PULLUP);
  /* Motor */
  pinMode(BE_RPWM_PIN, OUTPUT);
  pinMode(BE_LPWM_PIN, OUTPUT);
  pinMode(KA_RPWM_PIN, OUTPUT);
  pinMode(KA_LPWM_PIN, OUTPUT);
  pinMode(KI_RPWM_PIN, OUTPUT);
  pinMode(KI_LPWM_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_BE_CHA), ENC_BE_FUNCT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_BE_CHB), ENC_BE_FUNCT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_KA_CHA), ENC_KA_FUNCT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_KA_CHB), ENC_KA_FUNCT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_KI_CHA), ENC_KI_FUNCT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_KI_CHB), ENC_KI_FUNCT_B, CHANGE);

  //  trialEnc();
  trialPWM(20);
  //  trialRPS(20);
  //  vSlave();
}

void loop() {

}

void receiveEvent(int bytes) {
  byte data[10];
  for (int i = 0; i <= 7; i++) {
    data[i] = Wire.read();
  }
  if (data[0] == AT_RPS) {
    Value[0] = data[1];
    Value[1] = data[2];
    Value[2] = data[3];
    Ctrl[0]  = data[4];
    Ctrl[1]  = data[5];
    Ctrl[2]  = data[6];
  } else if (data[0] == AT_RESET) {
    Value[0] = 0;
    Value[1] = 0;
    Value[2] = 0;
  }
}


void vSlave() {
  while (1) {

    int  beRPM = 0, kaRPM = 0, kiRPM = 0;

    beRPM = (int)(map(Value[0], 0, 255, 0, 400));
    kaRPM = (int)(map(Value[1], 0, 255, 0, 400));
    kiRPM = (int)(map(Value[2], 0, 255, 0, 400));

    if (Ctrl[0] == DIR_CCW) {
      beRPM = beRPM * -1;
    }    if (Ctrl[1] == DIR_CCW) {
      kaRPM = kaRPM * -1;
    }    if (Ctrl[2] == DIR_CCW) {
      kiRPM = kiRPM * -1;
    }

    RPMbelakang( beRPM, 0.11, 0.000019, 12.0);
    RPMkanan   ( kaRPM, 0.1,  0.000018, 10.0);
    RPMkiri    ( kiRPM, 0.1,  0.000019, 14.0);

    //    Serial.print(beRPM);
    //    Serial.print('\t');
    //    Serial.print(kaRPM);
    //    Serial.print('\t');
    //    Serial.println(kiRPM);
  }
}


void trialEnc() {
  while (1) {
    Serial.print("BE_ENC: ");
    Serial.print(ENC_BE_COUNT);
    Serial.print("\t");

    Serial.print("KA_ENC: ");
    Serial.print(ENC_KA_COUNT);
    Serial.print("\t");

    Serial.print("KI_ENC: ");
    Serial.println(ENC_KI_COUNT);
    delay(100);
  }
}

void trialPWM(int Speed) {
  while (1) {
    analogWrite(BE_RPWM_PIN,  Speed);
    digitalWrite(BE_LPWM_PIN, LOW);
    analogWrite(KA_RPWM_PIN,  Speed);
    digitalWrite(KA_LPWM_PIN, LOW);
    analogWrite(KI_RPWM_PIN,  Speed);
    digitalWrite(KI_LPWM_PIN, LOW);
    delay(1000);
    analogWrite(BE_LPWM_PIN,  Speed);
    digitalWrite(BE_RPWM_PIN, LOW);
    analogWrite(KA_LPWM_PIN,  Speed);
    digitalWrite(KA_RPWM_PIN, LOW);
    analogWrite(KI_LPWM_PIN,  Speed);
    digitalWrite(KI_RPWM_PIN, LOW);
    delay(1000);
  }
}

void trialRPS(int Speed) {
  while (1) {
    RPMbelakang( Speed, 0.11, 0.000019, 17.0);
    RPMkanan   ( Speed, 0.1,  0.000018, 16.0);
    RPMkiri    ( Speed, 0.1,  0.000019, 17.0);
  }
}

void readRPM(int Speed) {
  float RPM;
  int PULSE;

  analogWrite(BE_RPWM_PIN,  Speed);
  digitalWrite(BE_LPWM_PIN, LOW);

  if ((millis() - Previousmillis) >= 100) {
    Previousmillis = millis();
    PULSE = ENC_BE_COUNT;
    RPM = float((PULSE * 600) / REVOLUTION );
    Serial.println(RPM);
    ENC_BE_COUNT = 0;
  }
}

void RPMbelakang(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM , RPM;
  int PULSE , TS = 1000;

  setRPM = rpm;
  if ((millis() - ltime1) >= 100) {
    ltime1 = millis();

    PULSE = ENC_BE_COUNT;
    RPM = float((PULSE * 600) / REVOLUTION );

    if (setRPM > 0) {
      BE_DIRECTION(CW);
    }
    else if (setRPM < 0) {
      BE_DIRECTION(CCW);
    }
    else if (setRPM == 0) {
      BE_DIRECTION(Stop);
      PWM_BE = 0;
    }

    // output P
    Error = abs(setRPM) -  abs(RPM);
    P = kp * Error;
    // output I
    cumError  += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    PWM_BE = PWM_BE + PID;
    lError = Error;

    ENC_BE_COUNT = 0;

    if (PWM_BE > 255) PWM_BE = 255;
    else if (PWM_BE < 0) PWM_BE = 0;

  }
}

void RPMkanan(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM , RPM;
  int PULSE , TS = 1000;
  setRPM = rpm;
  if ((millis() - ltime2) >= 100) {
    ltime2 = millis();

    PULSE = ENC_KA_COUNT;
    RPM = float((PULSE * 600) / REVOLUTION );

    if (setRPM > 0) {
      KA_DIRECTION(CW);
    }
    else if (setRPM < 0) {
      KA_DIRECTION(CCW);
    }
    else if (setRPM == 0) {
      KA_DIRECTION(Stop);
      PWM_KA = 0;
    }

    // output P
    Error = abs(setRPM) -  abs(RPM);
    P = kp * Error;
    // output I
    cumError  += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    PWM_KA = PWM_KA + PID;
    lError = Error;

    ENC_KA_COUNT = 0;

    if (PWM_KA > 255) PWM_KA = 255;
    else if (PWM_KA < 0) PWM_KA = 0;

  }
}

void RPMkiri(int rpm, float kp, float ki, float kd) {
  float PID, P, I, D;
  float Error, lError, cumError, rateError;
  float setRPM , RPM;
  int PULSE , TS = 1000;

  setRPM = rpm;
  if ((millis() - ltime3) >= 100) {
    ltime3 = millis();

    PULSE = ENC_KI_COUNT;
    RPM = float((PULSE * 600) / REVOLUTION );

    if (setRPM > 0) {
      KI_DIRECTION(CW);
    }
    else if (setRPM < 0) {
      KI_DIRECTION(CCW);
    }
    else if (setRPM == 0) {
      KI_DIRECTION(Stop);
      PWM_KI = 0;
    }

    // output P
    Error = abs(setRPM) -  abs(RPM);
    P = kp * Error;
    // output I
    cumError  += Error * TS;
    I = ki * cumError;
    // output D
    rateError = (Error - lError) / TS;
    D = kd * rateError;
    PID = P + I + D;
    PWM_KI = PWM_KI + PID;
    lError = Error;

    ENC_KI_COUNT = 0;

    if (PWM_KI > 255) PWM_KI = 255;
    else if (PWM_KI < 0) PWM_KI = 0;

  }
}
void BE_DIRECTION(int dir)
{
  switch (dir)
  {
    case CW:
      analogWrite(BE_RPWM_PIN, round(PWM_BE));
      digitalWrite(BE_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(BE_RPWM_PIN, LOW);
      analogWrite(BE_LPWM_PIN, round(PWM_BE));
      break;
    case Stop:
      digitalWrite(BE_RPWM_PIN, LOW);
      digitalWrite(BE_LPWM_PIN, LOW);
      break;
  }
}
void KA_DIRECTION(int dir)
{
  switch (dir)
  {
    case CW:
      analogWrite(KA_RPWM_PIN, round(PWM_KA));
      digitalWrite(KA_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(KA_RPWM_PIN, LOW);
      analogWrite(KA_LPWM_PIN, round(PWM_KA));
      break;
    case Stop:
      digitalWrite(KA_RPWM_PIN, LOW);
      digitalWrite(KA_LPWM_PIN, LOW);
      break;
  }
}
void KI_DIRECTION(int dir)
{
  switch (dir)
  {
    case CW:
      analogWrite(KI_RPWM_PIN, round(PWM_KI));
      digitalWrite(KI_LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(KI_RPWM_PIN, LOW);
      analogWrite(KI_LPWM_PIN, round(PWM_KI));
      break;
    case Stop:
      digitalWrite(KI_RPWM_PIN, LOW);
      digitalWrite(KI_LPWM_PIN, LOW);
      break;
  }
}

void ENC_BE_FUNCT_A() {
  if (digitalRead(ENC_BE_CHA) == HIGH) {
    if (digitalRead(ENC_BE_CHB) == LOW) {
      ENC_BE_COUNT++;
    }
    else {
      ENC_BE_COUNT--;
    }
  }
  else {
    if (digitalRead(ENC_BE_CHB) == HIGH) {
      ENC_BE_COUNT++;
    }
    else {
      ENC_BE_COUNT--;
    }
  }
}

void ENC_BE_FUNCT_B() {
  if (digitalRead(ENC_BE_CHA) == HIGH) {
    if (digitalRead(ENC_BE_CHB) == HIGH) {
      ENC_BE_COUNT++;
    }
    else {
      ENC_BE_COUNT--;
    }
  }

  else {
    if (digitalRead(ENC_BE_CHB) == LOW) {
      ENC_BE_COUNT++;
    }
    else {
      ENC_BE_COUNT--;
    }
  }
}

void ENC_KA_FUNCT_A() {
  if (digitalRead(ENC_KA_CHA) == HIGH) {
    if (digitalRead(ENC_KA_CHB) == LOW) {
      ENC_KA_COUNT++;
    }
    else {
      ENC_KA_COUNT--;
    }
  }

  else {
    if (digitalRead(ENC_KA_CHB) == HIGH) {
      ENC_KA_COUNT++;
    }
    else {
      ENC_KA_COUNT--;
    }
  }
}

void ENC_KA_FUNCT_B() {
  if (digitalRead(ENC_KA_CHA) == HIGH) {
    if (digitalRead(ENC_KA_CHB) == HIGH) {
      ENC_KA_COUNT++;
    }
    else {
      ENC_KA_COUNT--;
    }
  }
  else {
    if (digitalRead(ENC_KA_CHB) == LOW) {
      ENC_KA_COUNT++;
    }
    else {
      ENC_KA_COUNT--;
    }
  }
}

void ENC_KI_FUNCT_A() {
  if (digitalRead(ENC_KI_CHA) == HIGH) {
    if (digitalRead(ENC_KI_CHB) == LOW) {
      ENC_KI_COUNT++;
    }
    else {
      ENC_KI_COUNT--;
    }
  }

  else {
    if (digitalRead(ENC_KI_CHB) == HIGH) {
      ENC_KI_COUNT++;
    }
    else {
      ENC_KI_COUNT--;
    }
  }
}

void ENC_KI_FUNCT_B() {
  if (digitalRead(ENC_KI_CHA) == HIGH) {
    if (digitalRead(ENC_KI_CHB) == HIGH) {
      ENC_KI_COUNT++;
    }
    else {
      ENC_KI_COUNT--;
    }
  }

  else {
    if (digitalRead(ENC_KI_CHB) == LOW) {
      ENC_KI_COUNT++;
    }
    else {
      ENC_KI_COUNT--;
    }
  }
}
