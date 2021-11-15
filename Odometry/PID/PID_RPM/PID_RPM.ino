#define CW    1
#define CCW   2
#define Stop  3

#define ENC_BE_CHA    27
#define ENC_BE_CHB    29

#define ENC_KA_CHA    31
#define ENC_KA_CHB    33
#define ENC_KI_CHA    35
#define ENC_KI_CHB    37

#define BE_RPWM_PIN      5
#define BE_LPWM_PIN      6
#define KA_RPWM_PIN      8
#define KA_LPWM_PIN      7
#define KI_RPWM_PIN      9
#define KI_LPWM_PIN     10

#define REVOLUTION 135
#define MINUTE     0.00166

/*PULSE*/
volatile int32_t
ENC_BE_COUNT = 0, ENC_KA_COUNT = 0, ENC_KI_COUNT = 0;
volatile long int
LENC_BE_COUNT = 0, LENC_KA_COUNT = 0, LENC_KI_COUNT = 0;
float PWM_BE = 0, PWM_KA = 0, PWM_KI = 0;
long ltime1, ltime2, ltime3;
long Previousmillis;

/* DATA HMI */
String Data, CMD,
       sSTART, sRPS, sKP, sKI, sKD;
float KP, KI, KD;
int nSTART, nA, nB, nC, nD, nE,
    SET_RPS;



void setup() {
  Serial.begin(115200);
  /* Encoder */
  pinMode(ENC_BE_CHA, INPUT_PULLUP);
  pinMode(ENC_KA_CHA, INPUT_PULLUP);
  pinMode(ENC_KI_CHA, INPUT_PULLUP);
  /* Motor */
  pinMode(BE_RPWM_PIN, OUTPUT);
  pinMode(BE_LPWM_PIN, OUTPUT);
  pinMode(KA_RPWM_PIN, OUTPUT);
  pinMode(KA_LPWM_PIN, OUTPUT);
  pinMode(KI_RPWM_PIN, OUTPUT);
  pinMode(KI_LPWM_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_BE_CHA), ENC_BE_FUNCT_A, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_KA_CHA), ENC_KA_FUNCT_A, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_KI_CHA), ENC_KI_FUNCT_A, RISING);
}

void loop() {
  Run();
  //  readENC();
  //  trialPWM(10);
  //  readRPM(255);
}
void Run() {
  if (Serial.available()) {
    Data = Serial.readStringUntil('\n');
  }
  nSTART = Data.indexOf("A");
  nA = Data.indexOf("B");
  nB = Data.indexOf("C");
  nC = Data.indexOf("D");
  nD = Data.indexOf("E");
  nE = Data.indexOf("F");

  sSTART = Data.substring(nSTART + 1, nA);
  sRPS = Data.substring(nA + 1, nB);
  sKP  = Data.substring(nB + 1, nC);
  sKI  = Data.substring(nC + 1, nD);
  sKD  = Data.substring(nD + 1, nE);

  CMD = sSTART;

  if (CMD == "START") {
    SET_RPS = sRPS.toFloat();
    KP      = sKP.toFloat();
    KI      = sKI.toFloat();
    KD      = sKD.toFloat();

    // RPMbelakang( SET_RPS, KP, KI, KD);
    RPMkanan   ( SET_RPS, KP, KI, KD);
    // RPMkiri    ( SET_RPS, KP, KI, KD);
  }
  else if (CMD == "STOP") {
    SET_RPS = 0;
    KP      = 0;
    KI      = 0;
    KD      = 0;
    // RPMbelakang( SET_RPS, KP, KI, KD);
    RPMkanan   ( SET_RPS, KP, KI, KD);
    // RPMkiri    ( SET_RPS, KP, KI, KD);
  }
}

void readENC() {
  Serial.print("BE: ");
  Serial.print(ENC_BE_COUNT);
  Serial.print("\t");
  Serial.print("KA: ");
  Serial.print(ENC_KA_COUNT);
  Serial.print("\t");
  Serial.print("KI: ");
  Serial.println(ENC_KI_COUNT);
}

void trialPWM(int Speed) {
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

    Serial.println(RPM);

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

    Serial.println(RPM);

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

    Serial.println(RPM);

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
  ENC_BE_COUNT++;
}


void ENC_KA_FUNCT_A() {
  ENC_KA_COUNT++;
}



void ENC_KI_FUNCT_A() {
  ENC_KI_COUNT++;
}
