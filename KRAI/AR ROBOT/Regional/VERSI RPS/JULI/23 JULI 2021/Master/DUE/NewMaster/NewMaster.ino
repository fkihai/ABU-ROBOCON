#include <Arduino_due_FreeRTOS.h>

/* SERIAL */
#define Gamepad      Serial3
#define SlaveSerial  Serial2
#define serial       Serial

/* ODOMETRY */
#define MATH_PI 3.14286
#define WHEEL_DIAMETER 5.8
#define ENC_RESOLUTION 2600.0
#define KLL WHEEL_DIAMETER *MATH_PI

/* ENCODER */
#define encDepan_CHA 25
#define encDepan_CHB 23


#define encKiri_CHA 29
#define encKiri_CHB 27

#define encBelakang_CHA 31
#define encBelakang_CHB 33

#define encKanan_CHA 35
#define encKanan_CHB 37


#define encX_CHA 45
#define encX_CHB 43

#define encY_CHA 39
#define encY_CHB 41

#define LimitSwitch 12
#define Prox 11

/* MOTOR*/
#define DE_Rpwm_PIN 3
#define DE_Lpwm_PIN 2

#define KI_Rpwm_PIN 4
#define KI_Lpwm_PIN 5

#define BE_Rpwm_PIN 6
#define BE_Lpwm_PIN 7
#define KA_Rpwm_PIN 9
#define KA_Lpwm_PIN 8

#define maju 1
#define mundur 2
#define berhenti 3

struct ENC
{
  volatile long int pulseCnt;
  long int lastpulse;
} EncDepan, EncBelakang, EncKanan, EncKiri, EncX, EncY;

struct Motor
{
  float PID, pwm, RPS;
  long Time;
} DE, BE, KA, KI;

struct Position
{
  float X = 0, Y = 0;
} Pos;

struct COM
{
  char Data = 'Z';
} Slave, Joystik;

int vrps1 = 0, vrps2 = 0,
    vrps3 = 0, vrps4 = 0;

/* ZoneAuto1 */
bool GoPos = false;
bool Go = false;
bool GoLmpr = false;
bool Ambil = false;
bool Ctrl = false;
bool Ctrl1 = false;
bool flag = false;
bool Run = false;

/* Fungsi AmbilPanah () */
bool Auto = false;
long ltime1 = 0;
long ltime2 = 0;

/* Zone Full Auto */
bool Auto1 = false;
bool Auto2 = false;
bool GoTransfer = false;
bool GoToRack = false;
bool GoRun = false;
bool Putar = false;
bool CtrlRun = false;


/* Auto Zone */
bool Start = false;
bool Zone1 = false;
bool Zone2 = false;
bool Zone3 = false;
bool MauStop = false;
/* Auto Move (Right dan Left) */
char param = 'Z';
bool Geser = false;
bool Move = false;

void PinConfig()
{
  pinMode(encDepan_CHA, INPUT_PULLUP);
  pinMode(encDepan_CHB, INPUT_PULLUP);
  pinMode(encBelakang_CHA, INPUT_PULLUP);
  pinMode(encBelakang_CHB, INPUT_PULLUP);
  pinMode(encKanan_CHA, INPUT_PULLUP);
  pinMode(encKanan_CHB, INPUT_PULLUP);
  pinMode(encKiri_CHA, INPUT_PULLUP);
  pinMode(encKiri_CHB, INPUT_PULLUP);
  pinMode(encX_CHA, INPUT_PULLUP);
  pinMode(encX_CHB, INPUT_PULLUP);
  pinMode(encY_CHA, INPUT_PULLUP);
  pinMode(encY_CHB, INPUT_PULLUP);
  pinMode(BE_Rpwm_PIN, OUTPUT);
  pinMode(BE_Lpwm_PIN, OUTPUT);
  pinMode(KA_Rpwm_PIN, OUTPUT);
  pinMode(KA_Lpwm_PIN, OUTPUT);
  pinMode(KI_Rpwm_PIN, OUTPUT);
  pinMode(KI_Lpwm_PIN, OUTPUT);
  pinMode(LimitSwitch, INPUT_PULLUP);
  pinMode(Prox, INPUT);


  attachInterrupt(digitalPinToInterrupt(encDepan_CHA), encDepan_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encDepan_CHB), encDepan_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encBelakang_CHA), encBelakang_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encBelakang_CHB), encBelakang_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKanan_CHA), encKanan_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKanan_CHB), encKanan_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKiri_CHA), encKiri_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKiri_CHB), encKiri_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encX_CHA), encX_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encX_CHB), encX_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encY_CHA), encY_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encY_CHB), encY_INTT_B, CHANGE);
  analogWrite(DE_Lpwm_PIN, LOW);
  analogWrite(DE_Rpwm_PIN, LOW);
  analogWrite(BE_Lpwm_PIN, LOW);
  analogWrite(BE_Rpwm_PIN, LOW);
  analogWrite(KA_Lpwm_PIN, LOW);
  analogWrite(KA_Rpwm_PIN, LOW);
  analogWrite(KI_Lpwm_PIN, LOW);
  analogWrite(KI_Rpwm_PIN, LOW);
  DE.Time = millis();
  BE.Time = millis();
  KA.Time = millis();
  KI.Time = millis();
  ltime1 = millis();
  ltime2 = millis();
}

void setup()
{
  Serial.begin(9600);
  serial.begin(9600);
  Gamepad.begin(9600);
  SlaveSerial.begin(9600);
  PinConfig();
  xTaskCreate(Main, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  xTaskCreate(Odometry, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  vTaskStartScheduler();
}

void loop()
{
  // Dead Loop
}

void LogData()
{
  if (Gamepad.available())
  {
    Joystik.Data = Gamepad.read();
    // serial.println(Joystik.Data);
  }
  if (SlaveSerial.available())
  {
    Slave.Data = SlaveSerial.read();
    // serial.println(Slave.Data);
  }
}

void Odometry(void *pvParameters)
{
  for (;;)
  {
    EncCompute();
  }
}

void Main(void *pvParameters)
{
  for (;;)
  {
    LogData();
    ManualMode();
    CtrlAuto();

    /** Trial **/
    // trialMotor(30);
    // trialSensor();
    // trialEncoder();
    // trialRPS(-10);
  }
}

void CtrlAuto()
{
  if (Joystik.Data == 'P')
  {
    Ctrl = true;
  }

  if (Ctrl) {
    ZoneLempar2();
    Joystik.Data = 'Z';
    Ctrl = false;
  }
}


void ZoneLempar2()
{
  Auto1 = true;
  Zone1 = true;

  /** Local **/
  bool Gorack = false;
  bool AmbilPanah = false;
  bool Switch = false;

  while (Zone1) {
    vrps1 = 30;
    vrps2 = 30;
    vrps3 = 13;
    vrps4 = 13;

    while (Auto1) {
      if (abs(Pos.Y) < 80 && !Gorack && digitalRead(LimitSwitch) == HIGH)
      {
        RPSKanan(-20, 0.01, 0.05);
        RPSKiri(-20, 0.01, 0.05);
      } else {
        SlaveSerial.print('L');
        KA.RPS = KA.pwm = KI.RPS = KI.pwm = 0;
        delay(20);
        Gorack = true;
      }

      while (Gorack) {
        if (abs(Pos.X) < 600 && digitalRead(Prox) == HIGH)
        {
          RPSDepan(-(vrps1), 0.01, 0.05);
          RPSBelakang(-(vrps2), 0.01, 0.05);
          RPSKanan(vrps3, 0.01, 0.05);
          RPSKiri(vrps4, 0.01, 0.05);
          SoftWheel(50, 200, 10, 10, abs(Pos.X), 5);  // Vrps 1 & 2
          SoftWheel1(50, 200, 10, 10, abs(Pos.X), 5); // Vrps 3 & 4
        } if (digitalRead(Prox) == LOW) {
          delay(100);
          Stop();
          vrps1 = 6;
          vrps2 = 6;
          vrps3 = 7;
          vrps4 = 7;
          AmbilPanah = true;
          Reset();
        }
        while (AmbilPanah) {
          RPSDepan(-(vrps1), 0.009, 0.05);
          RPSBelakang(-(vrps2), 0.009, 0.05);
          RPSKanan(-(vrps3), 0.009, 0.05);
          RPSKiri(-(vrps4), 0.009, 0.05);
          SoftWheel(10, 80, 10, 10, abs(Pos.Y), 1);  // Vrps 1 & 2
          SoftWheel1(10, 80, 10, 10, abs(Pos.Y), 1); // Vrps 3 & 4

          if (digitalRead(LimitSwitch) == LOW) {
            Stop();
            delay(100);
            analogWrite(DE_Lpwm_PIN, 70);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 70);
            analogWrite(BE_Rpwm_PIN, LOW);
            delay(300);
            analogWrite(DE_Lpwm_PIN, 30);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 30);
            analogWrite(BE_Rpwm_PIN, LOW);
            delay(700);
            SlaveSerial.print('M');
            AmbilPanah = false;
            Gorack = false;
            Auto1 = false;

            /** GoAuto 2 **/
            Auto2 = true;
            GoPos = true;
            break;
          }
        }
      }
    }

    while (Auto2) {
      while (GoPos)
      {
        LogData();
        if (Slave.Data == 'L')
        {
          Reset();
          vrps1 = 15;
          vrps2 = 15;
          vrps3 = 8;
          vrps4 = 8;
          Go = true;
        }
        while (Go && !GoLmpr)
        {
          if (!GoLmpr && Pos.Y < 50)
          {
            RPSDepan(-(vrps3), 0.01, 0.05);
            RPSBelakang(-(vrps4), 0.01, 0.05);
            RPSKanan(vrps1, 0.01, 0.05);
            RPSKiri(vrps2, 0.01, 0.05);
            SoftWheel(20, 60, 10, 10, Pos.Y, 3);  // Vrps 1 & 2
            SoftWheel1(20, 60, 10, 10, Pos.Y, 1); // Vrps 3 & 4
            if (Pos.Y > 20)
            {
              SlaveSerial.print('K');
            }
          }
          while (Pos.Y > 47 && !GoLmpr)
          {
            DE.pwm = BE.pwm = KA.pwm = KI.pwm = 0;
            analogWrite(KA_Rpwm_PIN, LOW);
            analogWrite(KA_Lpwm_PIN, LOW);
            analogWrite(KI_Rpwm_PIN, LOW);
            analogWrite(KI_Lpwm_PIN, LOW);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(DE_Lpwm_PIN, 70);
            analogWrite(BE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 70);
            delay(100);
            Stop();
            delay(600);
            GoLmpr = true;
            Go = false;
            Slave.Data = 'A';
          }
        }
        while (GoLmpr)
        {
          ThrowZone();
        }
      }
    }



  }
}

void ThrowZone()
{
  LogData();
  param = Slave.Data;

  switch (param)
  {

    /** Zone 2 **/

    case 'A':
      analogWrite(KA_Rpwm_PIN, 43);
      analogWrite(KA_Lpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, 43);
      analogWrite(KI_Lpwm_PIN, LOW);
      analogWrite(DE_Lpwm_PIN, 43);
      analogWrite(DE_Rpwm_PIN, LOW);
      analogWrite(BE_Lpwm_PIN, 43);
      analogWrite(BE_Rpwm_PIN, LOW);
      delay(1000);
      analogWrite(KA_Rpwm_PIN, 5);
      analogWrite(KA_Lpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, 5);
      analogWrite(KI_Lpwm_PIN, LOW);
      analogWrite(DE_Lpwm_PIN, 5);
      analogWrite(DE_Rpwm_PIN, LOW);
      analogWrite(BE_Lpwm_PIN, 5);
      analogWrite(BE_Rpwm_PIN, LOW);
      SlaveSerial.print('A');
      Reset();
      Slave.Data = 'Z';
      break;
    case 'B':
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('B');
      MoveRigh(7.7);
      Slave.Data = 'Z';
      break;
    case 'C':
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('C');
      MoveRigh(15.7);
      Slave.Data = 'Z';
      break;
    case 'D':
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('D');
      MoveRigh(25.7);
      Slave.Data = 'E';
      break;
    case 'E':
      Auto1 = false;
      Auto2 = false;
      Zone1 = false;
      GoLmpr = false;
      GoPos = false;
      Stop();
      Reset();
      Slave.Data = 'Z';
      break;
    default:
      break;
  }
}

void MoveRigh(float pos)
{
  while (Geser)
  {
    if (Move)
    {
      RPSDepan(4, 0.01, 0.05);
      RPSBelakang(4, 0.01, 0.05);
      analogWrite(KA_Rpwm_PIN, LOW);
      analogWrite(KA_Lpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, LOW);
      analogWrite(KI_Lpwm_PIN, LOW);
    }
    if (Pos.X > pos - 1.0)    {
      Move = false;
      MoveStop();
      Geser = false;
    }
  }
}

void MoveLeft(float pos)
{
  while (Geser)
  {
    if (Move)
    {
      RPSDepan(-4, 0.1, 0.05);
      RPSBelakang(-4, 0.1, 0.05);
      analogWrite(KA_Rpwm_PIN, LOW);
      analogWrite(KA_Lpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, LOW);
      analogWrite(KI_Lpwm_PIN, LOW);
    }

    if (abs(Pos.X) > abs(pos) - 1.0)    {
      Move = false;
      MoveStop();
      Geser = false;
    }
  }
}



void MoveStop()
{
  DE.pwm = BE.pwm = KA.pwm = KI.pwm = 0;
  analogWrite(DE_Lpwm_PIN, LOW);
  analogWrite(DE_Rpwm_PIN, LOW);
  analogWrite(BE_Lpwm_PIN, LOW);
  analogWrite(BE_Rpwm_PIN, LOW);
  analogWrite(KA_Rpwm_PIN, 100);
  analogWrite(KA_Lpwm_PIN, LOW);
  analogWrite(KI_Rpwm_PIN, 100);
  analogWrite(KI_Lpwm_PIN, LOW);
  delay(700);
  analogWrite(DE_Lpwm_PIN, LOW);
  analogWrite(DE_Rpwm_PIN, LOW);
  analogWrite(BE_Lpwm_PIN, LOW);
  analogWrite(BE_Rpwm_PIN, LOW);
  analogWrite(KA_Rpwm_PIN, 5);
  analogWrite(KA_Lpwm_PIN, LOW);
  analogWrite(KI_Rpwm_PIN, 5);
  analogWrite(KI_Lpwm_PIN, LOW);
}

void Stop()
{
  analogWrite(DE_Lpwm_PIN, LOW);
  analogWrite(DE_Rpwm_PIN, LOW);
  analogWrite(BE_Lpwm_PIN, LOW);
  analogWrite(BE_Rpwm_PIN, LOW);
  analogWrite(KA_Lpwm_PIN, LOW);
  analogWrite(KA_Rpwm_PIN, LOW);
  analogWrite(KI_Lpwm_PIN, LOW);
  analogWrite(KI_Rpwm_PIN, LOW);
  DE.pwm = BE.pwm = KA.pwm = KI.pwm = 0;
}

/*  MANUAL **/

void ManualMode()
{

  /* NAVIGASI */
  char IR64 = Joystik.Data;

  if (IR64 == 'A')
  { // FORWARD
    RPSKanan(30, 0.1, 0.05);
    RPSKiri(30, 0.1, 0.05);
    analogWrite(DE_Lpwm_PIN, LOW);
    analogWrite(DE_Rpwm_PIN, LOW);
    analogWrite(BE_Lpwm_PIN, LOW);
    analogWrite(BE_Rpwm_PIN, LOW);
  }
  if (IR64 == 'B')
  { // BACKWARD
    RPSKanan(-30, 0.1, 0.05);
    RPSKiri(-30, 0.1, 0.05);
    analogWrite(DE_Lpwm_PIN, LOW);
    analogWrite(DE_Rpwm_PIN, LOW);
    analogWrite(BE_Lpwm_PIN, LOW);
    analogWrite(BE_Rpwm_PIN, LOW);
  }
  if (IR64 == 'C')
  { // RIGH
    RPSDepan(30, 0.1, 0.05);
    RPSBelakang(30, 0.1, 0.05);
    analogWrite(KA_Lpwm_PIN, LOW);
    analogWrite(KA_Rpwm_PIN, LOW);
    analogWrite(KI_Lpwm_PIN, LOW);
    analogWrite(KI_Rpwm_PIN, LOW);
  }
  if (IR64 == 'D')
  { // LEFT
    RPSDepan(-30, 0.1, 0.05);
    RPSBelakang(-30, 0.1, 0.05);
    analogWrite(KA_Lpwm_PIN, LOW);
    analogWrite(KA_Rpwm_PIN, LOW);
    analogWrite(KI_Lpwm_PIN, LOW);
    analogWrite(KI_Rpwm_PIN, LOW);
  }
  if (IR64 == 'E')
  { // SERONG KANAN MAJU {R1}
    RPSDepan(30, 0.1, 0.05);
    RPSBelakang(30, 0.1, 0.05);
    RPSKanan(30, 0.1, 0.05);
    RPSKiri(30, 0.1, 0.05);
  }
  if (IR64 == 'F')
  { // SERONG KIRI MAJU {L1}
    RPSDepan(-30, 0.1, 0.05);
    RPSBelakang(-30, 0.1, 0.05);
    RPSKanan(30, 0.1, 0.05);
    RPSKiri(30, 0.1, 0.05);
  }
  if (IR64 == 'G')
  { // PUTAR KANAN {R2}
    RPSDepan(15, 0.1, 0.05);
    RPSBelakang(-15, 0.1, 0.05);
    RPSKanan(-15, 0.1, 0.05);
    RPSKiri(15, 0.1, 0.05);
  }
  if (IR64 == 'H')
  { // PUTAR KIRI  {L2}
    RPSDepan(-15, 0.1, 0.05);
    RPSBelakang(15, 0.1, 0.05);
    RPSKanan(15, 0.1, 0.05);
    RPSKiri(-15, 0.1, 0.05);
  }
  if (IR64 == '*')
  {
    Reset();
    GoPos = false;
    Go = false;
    GoLmpr = false;
    Ambil = false;
    Ctrl = false;
    Ctrl1 = false;
    flag = false;
    Run = false;
    Auto = false;
    param = 'Z';
    Geser = false;
    Move = false;
  }

  if (IR64 == 'Z')
  {
    analogWrite(DE_Lpwm_PIN, LOW);
    analogWrite(DE_Rpwm_PIN, LOW);
    analogWrite(BE_Lpwm_PIN, LOW);
    analogWrite(BE_Rpwm_PIN, LOW);
    analogWrite(KA_Lpwm_PIN, LOW);
    analogWrite(KA_Rpwm_PIN, LOW);
    analogWrite(KI_Lpwm_PIN, LOW);
    analogWrite(KI_Rpwm_PIN, LOW);
    DE.pwm = BE.pwm = KA.pwm = KI.pwm = 0;
  }
}

void Reset()
{
  EncY.pulseCnt = 0;
  EncX.pulseCnt = 0;
  Pos.X = 0;
  Pos.X = 0;
}

void EncCompute()
{
  Pos.X = (float(EncX.pulseCnt) * KLL) / ENC_RESOLUTION;
  Pos.Y = (float(EncY.pulseCnt) * KLL) / ENC_RESOLUTION;

  //  Serial.print("X: ");
  //  Serial.print(Pos.X);
  //  Serial.print("\t");
  //  Serial.print("Y: ");
  //  Serial.println(Pos.Y);
}

/*********** Soft ***********/

void SoftWheel(float Ta, float Tb, float a, float b, int dist, int Speed)
{
  if ((millis() - ltime1) > 20 && dist > Ta && dist < Tb)
  {
    ltime1 = millis();
    vrps1 -= a;
    vrps2 -= b;
    if (vrps1 < Speed)
    {
      vrps1 = Speed;
    }
    if (vrps2 < Speed)
    {
      vrps2 = Speed;
    }
  }
}

void SoftWheel1(float Ta, float Tb, float a, float b, int dist, int Speed)
{
  if ((millis() - ltime1) > 20 && dist > Ta && dist < Tb)
  {
    ltime1 = millis();
    vrps3 -= a;
    vrps4 -= b;
    if (vrps3 < Speed)
    {
      vrps3 = Speed;
    }
    if (vrps4 < Speed)
    {
      vrps4 = Speed;
    }
  }
}

/*********** Trial ***********/

void trialEncoder()
{

  Pos.X = (float(EncX.pulseCnt) * KLL) / ENC_RESOLUTION;
  Pos.Y = (float(EncY.pulseCnt) * KLL) / ENC_RESOLUTION;

  serial.print("ENC_DE: ");
  serial.print(EncDepan.pulseCnt);
  serial.print("\t");
  serial.print("ENC_BE: ");
  serial.print(EncBelakang.pulseCnt);
  serial.print("\t");
  serial.print("ENC_KA: ");
  serial.print(EncKanan.pulseCnt);
  serial.print("\t");
  serial.print("ENC_KI: ");
  serial.print(EncKiri.pulseCnt);
  serial.print("\t");
  serial.print("ENC_X: ");
  serial.print(EncX.pulseCnt);
  serial.print("\t");
  serial.print("ENC_Y: ");
  serial.print(EncY.pulseCnt);
  serial.print("\t");
  serial.print("XPOS: ");
  serial.print(Pos.X);
  serial.print("\t");
  serial.print("YPOS: ");
  serial.println(Pos.Y);
}

void trialSensor()
{
  serial.print("Prox: ");
  serial.print(digitalRead(Prox));
  serial.print("\t");
  serial.print("Switch: ");
  serial.println(digitalRead(LimitSwitch));
}

void trialMotor(int pwm)
{
  digitalWrite(DE_Lpwm_PIN, LOW);
  analogWrite(DE_Rpwm_PIN, pwm);
  digitalWrite(BE_Lpwm_PIN, LOW);
  analogWrite(BE_Rpwm_PIN, pwm);
  digitalWrite(KA_Lpwm_PIN, LOW);
  analogWrite(KA_Rpwm_PIN, pwm);
  digitalWrite(KI_Lpwm_PIN, LOW);
  analogWrite(KI_Rpwm_PIN, pwm);
  //  delay(1000);
  //  digitalWrite(DE_Rpwm_PIN, LOW);
  //  analogWrite(DE_Lpwm_PIN, pwm);
  //  digitalWrite(BE_Rpwm_PIN, LOW);
  //  analogWrite(BE_Lpwm_PIN, pwm);
  //  digitalWrite(KA_Rpwm_PIN, LOW);
  //  analogWrite(KA_Lpwm_PIN, pwm);
  //  digitalWrite(KI_Rpwm_PIN, LOW);
  //  analogWrite(KI_Lpwm_PIN, pwm);
  //  delay(1000);
}

void trialRPS(int Speed)
{
  RPSDepan(Speed, 0.01, 0.05);
  RPSBelakang(Speed, 0.01, 0.05);
  RPSKanan(Speed, 0.01, 0.05);
  RPSKiri(Speed, 0.01, 0.05);
}

/*********** RPS MOTOR ***********/

void RPSDepan(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;

  float P, I, D, error, lError = 0;
  int cRPS;
  DE.RPS = RPS;

  if ((millis() - DE.Time) > 10)
  {
    cRPS = abs(EncDepan.pulseCnt) - abs(EncDepan.lastpulse);
    if (DE.RPS > 0)
    {
      MotorDepan(maju);
    }
    else if (DE.RPS < 0)
    {
      MotorDepan(mundur);
    }
    else
    {
      MotorDepan(berhenti);
      DE.pwm = 0;
    }

    error = abs(DE.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    DE.PID = P + D;
    DE.pwm = DE.pwm + DE.PID;

    DE.Time = millis();
    lError = error;
    EncDepan.lastpulse = EncDepan.pulseCnt;

    if (DE.pwm > 255)
      DE.pwm = 255;
    else if (DE.pwm < 0)
      DE.pwm = 0;
  }
}

void MotorDepan(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(DE_Rpwm_PIN, DE.pwm);
      analogWrite(DE_Lpwm_PIN, LOW);
      break;
    case mundur:
      analogWrite(DE_Lpwm_PIN, DE.pwm);
      analogWrite(DE_Rpwm_PIN, LOW);
      break;
    case berhenti:
      analogWrite(DE_Rpwm_PIN, LOW);
      analogWrite(DE_Rpwm_PIN, LOW);
      break;
  }
}

void RPSBelakang(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;

  float P, I, D, error, lError = 0;
  int cRPS;
  BE.RPS = RPS;

  if ((millis() - BE.Time) > 10)
  {
    cRPS = abs(EncBelakang.pulseCnt) - abs(EncBelakang.lastpulse);
    if (BE.RPS > 0)
    {
      MotorBelakang(maju);
    }
    else if (BE.RPS < 0)
    {
      MotorBelakang(mundur);
    }
    else
    {
      MotorBelakang(berhenti);
      BE.pwm = 0;
    }

    error = abs(BE.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    BE.PID = P + D;
    BE.pwm = BE.pwm + BE.PID;

    BE.Time = millis();
    lError = error;
    EncBelakang.lastpulse = EncBelakang.pulseCnt;

    if (BE.pwm > 255)
      BE.pwm = 255;
    else if (BE.pwm < 0)
      BE.pwm = 0;
  }
}

void MotorBelakang(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(BE_Rpwm_PIN, BE.pwm);
      analogWrite(BE_Lpwm_PIN, LOW);
      break;
    case mundur:
      analogWrite(BE_Lpwm_PIN, BE.pwm);
      analogWrite(BE_Rpwm_PIN, LOW);
      break;
    case berhenti:
      analogWrite(BE_Rpwm_PIN, LOW);
      analogWrite(BE_Rpwm_PIN, LOW);
      break;
  }
}

void RPSKanan(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;

  float P, I, D, error, lError = 0;
  int cRPS;
  KA.RPS = RPS;

  if ((millis() - KA.Time) > 10)
  {
    cRPS = abs(EncKanan.pulseCnt) - abs(EncKanan.lastpulse);
    if (KA.RPS > 0)
    {
      MotorKanan(maju);
    }
    else if (KA.RPS < 0)
    {
      MotorKanan(mundur);
    }
    else
    {
      MotorKanan(berhenti);
      KA.pwm = 0;
    }

    error = abs(KA.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    KA.PID = P + D;
    KA.pwm = KA.pwm + KA.PID;

    KA.Time = millis();
    lError = error;
    EncKanan.lastpulse = EncKanan.pulseCnt;

    if (KA.pwm > 255)
      KA.pwm = 255;
    else if (KA.pwm < 0)
      KA.pwm = 0;
  }
}

void MotorKanan(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(KA_Rpwm_PIN, KA.pwm);
      analogWrite(KA_Lpwm_PIN, LOW);
      break;
    case mundur:
      analogWrite(KA_Lpwm_PIN, KA.pwm);
      analogWrite(KA_Rpwm_PIN, LOW);
      break;
    case berhenti:
      analogWrite(KA_Rpwm_PIN, LOW);
      analogWrite(KA_Rpwm_PIN, LOW);
      break;
  }
}

void RPSKiri(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;

  float P, I, D, error, lError = 0;
  int cRPS;
  KI.RPS = RPS;

  if ((millis() - KI.Time) > 10)
  {
    cRPS = abs(EncKiri.pulseCnt) - abs(EncKiri.lastpulse);
    if (KI.RPS > 0)
    {
      MotorKiri(maju);
    }
    else if (KI.RPS < 0)
    {
      MotorKiri(mundur);
    }
    else
    {
      MotorKiri(berhenti);
      KI.pwm = 0;
    }

    error = abs(KI.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    KI.PID = P + D;
    KI.pwm = KI.pwm + KI.PID;

    KI.Time = millis();
    lError = error;
    EncKiri.lastpulse = EncKiri.pulseCnt;

    if (KI.pwm > 255)
      KI.pwm = 255;
    else if (KI.pwm < 0)
      KI.pwm = 0;
  }
}

void MotorKiri(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(KI_Rpwm_PIN, KI.pwm);
      analogWrite(KI_Lpwm_PIN, LOW);
      break;
    case mundur:
      analogWrite(KI_Lpwm_PIN, KI.pwm);
      analogWrite(KI_Rpwm_PIN, LOW);
      break;
    case berhenti:
      analogWrite(KI_Rpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, LOW);
      break;
  }
}

/*********** ENC COUNT ***********/

void encDepan_INTT_A()
{
  if (digitalRead(encDepan_CHA) == HIGH)
  {
    if (digitalRead(encDepan_CHB) == LOW)
    {
      EncDepan.pulseCnt++;
    }
    else
    {
      EncDepan.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encDepan_CHB) == HIGH)
    {
      EncDepan.pulseCnt++;
    }
    else
    {
      EncDepan.pulseCnt--;
    }
  }
}

void encDepan_INTT_B()
{
  if (digitalRead(encDepan_CHA) == HIGH)
  {
    if (digitalRead(encDepan_CHB) == HIGH)
    {
      EncDepan.pulseCnt++;
    }
    else
    {
      EncDepan.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encDepan_CHB) == LOW)
    {
      EncDepan.pulseCnt++;
    }
    else
    {
      EncDepan.pulseCnt--;
    }
  }
}

void encBelakang_INTT_A()
{
  if (digitalRead(encBelakang_CHA) == HIGH)
  {
    if (digitalRead(encBelakang_CHB) == LOW)
    {
      EncBelakang.pulseCnt++;
    }
    else
    {
      EncBelakang.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encBelakang_CHB) == HIGH)
    {
      EncBelakang.pulseCnt++;
    }
    else
    {
      EncBelakang.pulseCnt--;
    }
  }
}

void encBelakang_INTT_B()
{
  if (digitalRead(encBelakang_CHA) == HIGH)
  {
    if (digitalRead(encBelakang_CHB) == HIGH)
    {
      EncBelakang.pulseCnt++;
    }
    else
    {
      EncBelakang.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encBelakang_CHB) == LOW)
    {
      EncBelakang.pulseCnt++;
    }
    else
    {
      EncBelakang.pulseCnt--;
    }
  }
}

void encKanan_INTT_A()
{
  if (digitalRead(encKanan_CHA) == HIGH)
  {
    if (digitalRead(encKanan_CHB) == LOW)
    {
      EncKanan.pulseCnt++;
    }
    else
    {
      EncKanan.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encKanan_CHB) == HIGH)
    {
      EncKanan.pulseCnt++;
    }
    else
    {
      EncKanan.pulseCnt--;
    }
  }
}

void encKanan_INTT_B()
{
  if (digitalRead(encKanan_CHA) == HIGH)
  {
    if (digitalRead(encKanan_CHB) == HIGH)
    {
      EncKanan.pulseCnt++;
    }
    else
    {
      EncKanan.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encKanan_CHB) == LOW)
    {
      EncKanan.pulseCnt++;
    }
    else
    {
      EncKanan.pulseCnt--;
    }
  }
}

void encKiri_INTT_A()
{
  if (digitalRead(encKiri_CHA) == HIGH)
  {
    if (digitalRead(encKiri_CHB) == LOW)
    {
      EncKiri.pulseCnt++;
    }
    else
    {
      EncKiri.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encKiri_CHB) == HIGH)
    {
      EncKiri.pulseCnt++;
    }
    else
    {
      EncKiri.pulseCnt--;
    }
  }
}

void encKiri_INTT_B()
{
  if (digitalRead(encKiri_CHA) == HIGH)
  {
    if (digitalRead(encKiri_CHB) == HIGH)
    {
      EncKiri.pulseCnt++;
    }
    else
    {
      EncKiri.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encKiri_CHB) == LOW)
    {
      EncKiri.pulseCnt++;
    }
    else
    {
      EncKiri.pulseCnt--;
    }
  }
}

void encX_INTT_A()
{
  if (digitalRead(encX_CHA) == HIGH)
  {
    if (digitalRead(encX_CHB) == LOW)
    {
      EncX.pulseCnt++;
    }
    else
    {
      EncX.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encX_CHB) == HIGH)
    {
      EncX.pulseCnt++;
    }
    else
    {
      EncX.pulseCnt--;
    }
  }
}

void encX_INTT_B()
{
  if (digitalRead(encX_CHA) == HIGH)
  {
    if (digitalRead(encX_CHB) == HIGH)
    {
      EncX.pulseCnt++;
    }
    else
    {
      EncX.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encX_CHB) == LOW)
    {
      EncX.pulseCnt++;
    }
    else
    {
      EncX.pulseCnt--;
    }
  }
}

void encY_INTT_A()
{
  if (digitalRead(encY_CHA) == HIGH)
  {
    if (digitalRead(encY_CHB) == LOW)
    {
      EncY.pulseCnt++;
    }
    else
    {
      EncY.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encY_CHB) == HIGH)
    {
      EncY.pulseCnt++;
    }
    else
    {
      EncY.pulseCnt--;
    }
  }
}

void encY_INTT_B()
{
  if (digitalRead(encY_CHA) == HIGH)
  {
    if (digitalRead(encY_CHB) == HIGH)
    {
      EncY.pulseCnt++;
    }
    else
    {
      EncY.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(encY_CHB) == LOW)
    {
      EncY.pulseCnt++;
    }
    else
    {
      EncY.pulseCnt--;
    }
  }
}
