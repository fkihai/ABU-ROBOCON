
#define CW    1
#define CCW   2
#define Stop  3
                                     /*BE  KA    KI*/
                   
#define ENC_CHA   27                  //27 //31  //35
#define ENC_CHB   29                  //29 //33  //37

#define RPWM_PIN   5                 //5  //8   //9
#define LPWM_PIN   6                //6  //7   //10

/*ENC
 * BA = 27
 * BB = 29
 * KA = 31
 * KB = 33
 * KIA = 35
 * KIB = 37
 */

/*MOTOR
 * BR = 5
 * BL = 6
 * KR = 8
 * KL = 7
 * KIR = 9
 * KIL = 10
 */


/* DATA SERIAL */
String Data, CMD,
       sSTART, sRPS, sKP, sKI, sKD;
float KP, KI, KD;
int nSTART, nA, nB, nC, nD, nE,
    SET_RPS;

/* RPS */
volatile int32_t
ENC_COUNT = 0;
volatile long int
lENC_COUNT = 0;
float
PID = 0,
pwm = 0;
int
RPS = 0;
long ltime;

int i;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(ENC_CHB, INPUT_PULLUP);
  pinMode(ENC_CHB , INPUT_PULLUP);

  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ENC_CHA), ENC_FUNCT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_CHB ), ENC_FUNCT_B, CHANGE);
}

void loop() {
  // RPSMotor(-10, 0.1, 0.0);
  Run();
  // ReadEnc();
  // TrialPWM();
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
    RPSMotor(SET_RPS, KP, KD);
  }
  else if (CMD == "STOP") {
    SET_RPS = 0;
    KP      = 0;
    KI      = 0;
    KD      = 0;
    ENC_COUNT = lENC_COUNT;
    RPSMotor(0,0,0);
  }
}

void ReadEnc() {
  Serial.print("ENC: ");
  Serial.println(ENC_COUNT);
}

void TrialPWM() {
  analogWrite(RPWM_PIN, 10);
  digitalWrite(LPWM_PIN, LOW);
  delay(1000);
  digitalWrite(RPWM_PIN, LOW);
  analogWrite(LPWM_PIN, 10);
  delay(1000);
}


void RPSMotor(float rps, float kp, float kd)
{
  float
  Kp = kp, Kd = kd,
  pid, TS = 1, P, I, D,
       error, lError = 0;
  int cRPS;
  int i;
  RPS = rps;
  if ((millis() - ltime) > 20)
  {
    cRPS = abs(ENC_COUNT) - abs(lENC_COUNT);
    if (RPS > 0) {
      DIRECTION(CW);
    }
    else if (RPS < 0) {
      DIRECTION(CCW);
    }
    else if (RPS == 0) {
      DIRECTION(Stop);
      pwm = 0;
    }
    error = abs(RPS) - abs(cRPS);
    P = Kp * error;
    D = (Kd / TS) * (error - lError);
    PID = P + D;
    pwm = pwm + PID;
    ltime = millis(); lError = error; lENC_COUNT = ENC_COUNT;
    i = analogRead(A4);
    i = map(i,0,1023,0,60);
    Serial.println(i);
  }
}

void DIRECTION(int dir)
{
  switch (dir)
  {
    case CW:
      analogWrite(RPWM_PIN, round(pwm));
      digitalWrite(LPWM_PIN, LOW);
      break;
    case CCW:
      digitalWrite(RPWM_PIN, LOW);
      analogWrite(LPWM_PIN, round(pwm));
      break;
    case Stop:
      digitalWrite(RPWM_PIN, LOW);
      digitalWrite(LPWM_PIN, LOW);
      break;
  }
}

void ENC_FUNCT_A() {
  if (digitalRead(ENC_CHA) == HIGH) {
    if (digitalRead(ENC_CHB ) == LOW) {
      ENC_COUNT++;
    }
    else {
      ENC_COUNT--;
    }
  }

  else {
    if (digitalRead(ENC_CHB ) == HIGH) {
      ENC_COUNT++;
    }
    else {
      ENC_COUNT--;
    }
  }
}

void ENC_FUNCT_B() {
  if (digitalRead(ENC_CHB ) == HIGH) {
    if (digitalRead(ENC_CHA) == HIGH) {
      ENC_COUNT++;
    }
    else {
      ENC_COUNT--;
    }
  }

  else {
    if (digitalRead(ENC_CHA) == LOW) {
      ENC_COUNT++;
    }
    else {
      ENC_COUNT--;
    }
  }
}
