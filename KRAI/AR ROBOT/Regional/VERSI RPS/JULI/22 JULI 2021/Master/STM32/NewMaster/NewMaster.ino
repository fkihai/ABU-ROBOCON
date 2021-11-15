#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <STM32FreeRTOS.h>

/* SERIAL */
HardwareSerial Gamepad(PD2, PC12);    // RX, TX
SoftwareSerial SlaveSerial(PC7, PC6); // RX, TX
SoftwareSerial serial(PC11, PC10);    // RX, TX

/* ODOMETRY */
#define MATH_PI 3.14286
#define WHEEL_DIAMETER 5.8
#define ENC_RESOLUTION 2400.0
#define KLL WHEEL_DIAMETER *MATH_PI

/* ENCODER */
#define encDepan_CHA PD11
#define encDepan_CHB PD9
#define encBelakang_CHA PC4
#define encBelakang_CHB PA6

#define encKanan_CHA PE8
#define encKanan_CHB PE10

#define encKiri_CHA PC2
#define encKiri_CHB PA0

#define encX_CHA PE14
#define encX_CHB PE12
#define encY_CHA PD1
#define encY_CHB PD3

#define Button1 PH0
#define Button2 PC14
#define LimitSwitch PD10
#define Prox PD8

/* MOTOR*/
#define DE_RPWM_PIN PA3
#define DE_LPWM_PIN PA1

#define KI_RPWM_PIN PA7
#define KI_LPWM_PIN PB1

#define BE_RPWM_PIN PB13
#define BE_LPWM_PIN PB15
#define KA_RPWM_PIN PB11
#define KA_LPWM_PIN PE13

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
  float PID, PWM, RPS;
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
  pinMode(BE_RPWM_PIN, OUTPUT);
  pinMode(BE_LPWM_PIN, OUTPUT);
  pinMode(KA_RPWM_PIN, OUTPUT);
  pinMode(KA_LPWM_PIN, OUTPUT);
  pinMode(KI_RPWM_PIN, OUTPUT);
  pinMode(KI_LPWM_PIN, OUTPUT);
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(LimitSwitch, INPUT_PULLUP);
  pinMode(Prox, INPUT);

  pinMode(PD12, OUTPUT);
  pinMode(PD13, OUTPUT);
  pinMode(PD14, OUTPUT);
  pinMode(PD15, OUTPUT);

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
  analogWrite(DE_LPWM_PIN, LOW);
  analogWrite(DE_RPWM_PIN, LOW);
  analogWrite(BE_LPWM_PIN, LOW);
  analogWrite(BE_RPWM_PIN, LOW);
  analogWrite(KA_LPWM_PIN, LOW);
  analogWrite(KA_RPWM_PIN, LOW);
  analogWrite(KI_LPWM_PIN, LOW);
  analogWrite(KI_RPWM_PIN, LOW);
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
    //    serial.print("XPOS: ");
    //    serial.print(Pos.X);
    //    serial.print('\t');
    //    serial.print("YPOS: ");
    //    serial.println(Pos.Y);
  }
}

void Main(void *pvParameters)
{
  for (;;)
  {
    LogData();
    ManualMode();
    // CtrlAuto();

    /** Trial **/
    // trialMotor(30);
    // trialSensor();
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
  vrps1 = 40;
  vrps2 = 40;
  vrps3 = 5;
  vrps4 = 5;
  GoPos = true;
  Auto1 = true;
  Zone1 = true;

  while (Zone1) {
    while (Auto1) {
      while (GoPos) {
        if (Pos.Y < 50) {
          RPSDepan(vrps3, 0.01, 0.05);
          RPSBelakang(vrps4, 0.01, 0.05);
          RPSKanan((vrps1), 0.01, 0.05);
          RPSKiri((vrps2), 0.01, 0.05);
          SoftWheel(40, 60, 10, 10, Pos.Y, 5);  // Vrps 1 & 2
          SoftWheel1(40, 60, 10, 10, Pos.Y, 3); // Vrps 3 & 4
        } else if (Pos.Y > 50 && Pos.Y < 90) {
          RPSDepan(vrps3, 1, 0.05);
          RPSBelakang(vrps4, 1, 0.05);
          RPSKanan((vrps1), 1, 0.05);
          RPSKiri((vrps2), 1, 0.05);
          SoftWheel(40, 60, 10, 10, Pos.Y, 5);  // Vrps 1 & 2
          SoftWheel1(40, 60, 10, 10, Pos.Y, 3); // Vrps 3 & 4
        }
        else {
          analogWrite(KA_RPWM_PIN, LOW);
          analogWrite(KA_LPWM_PIN, LOW);
          analogWrite(KI_RPWM_PIN, LOW);
          analogWrite(KI_LPWM_PIN, LOW);
          analogWrite(DE_LPWM_PIN, LOW);
          analogWrite(DE_RPWM_PIN, 70);
          analogWrite(BE_LPWM_PIN, LOW);
          analogWrite(BE_RPWM_PIN, 70);
          delay(200);
          Stop();
          delay(50);
          Slave.Data = 'A';
          GoLmpr = true;
          GoPos = false;
        }
      }
      while (GoLmpr) {
        ThrowZone();
      }
    }
  }

  while (Zone2) {
    vrps1 = 40;
    vrps2 = 40;
    vrps3 = 0;
    vrps4 = 0;
    while (GoTransfer) {
      if (abs(Pos.X) < 600 && digitalRead(Prox) == HIGH)
      {
        RPSDepan(-(vrps1), 0.01, 0.05);
        RPSBelakang(-(vrps2), 0.01, 0.05);
        RPSKanan(-(vrps3), 0.01, 0.05);
        RPSKiri(-(vrps4), 0.01, 0.05);
        SoftWheel(200, 450, 10, 10, abs(Pos.X), 5);  // Vrps 1 & 2
        SoftWheel1(200, 450, 10, 10, abs(Pos.X), 0); // Vrps 3 & 4
      }
      else if (digitalRead(Prox) == LOW)
      {
        DE.PWM = BE.PWM = KA.PWM = KI.PWM = 0;
        analogWrite(KA_RPWM_PIN, LOW);
        analogWrite(KA_LPWM_PIN, LOW);
        analogWrite(KI_RPWM_PIN, LOW);
        analogWrite(KI_LPWM_PIN, LOW);

        analogWrite(DE_LPWM_PIN, 15);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 15);
        analogWrite(BE_RPWM_PIN, LOW);
        delay(300);
        analogWrite(KA_RPWM_PIN, LOW);
        analogWrite(KA_LPWM_PIN, LOW);
        analogWrite(KI_RPWM_PIN, LOW);
        analogWrite(KI_LPWM_PIN, LOW);

        analogWrite(DE_LPWM_PIN, 10);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 10);
        analogWrite(BE_RPWM_PIN, LOW);
        delay(200);
        Stop();
        delay(100);
        GoTransfer = false;
        Zone2 = false;
        flag = true;
        Ambil = true;
        vrps1 = 15;
        vrps2 = 15;
        vrps3 = 7;
        vrps4 = 7;
        Auto = true;
        break;
      }
    }
  }

  while (Ambil) {
    while (Auto) {
      if (digitalRead(LimitSwitch) == HIGH) {

        analogWrite(KA_LPWM_PIN, 40);
        analogWrite(KA_RPWM_PIN, LOW);
        analogWrite(KI_LPWM_PIN, 40);
        analogWrite(KI_RPWM_PIN, LOW);
        analogWrite(DE_LPWM_PIN, 40);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 40);
        analogWrite(BE_RPWM_PIN, LOW);
        delay(500);
        analogWrite(KA_LPWM_PIN, 20);
        analogWrite(KA_RPWM_PIN, LOW);
        analogWrite(KI_LPWM_PIN, 20);
        analogWrite(KI_RPWM_PIN, LOW);
        analogWrite(DE_LPWM_PIN, 20);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 20);
        analogWrite(BE_RPWM_PIN, LOW);
      }

      if (digitalRead(LimitSwitch) == LOW)
      {
        DE.PWM = BE.PWM = KA.PWM = KI.PWM = 0;
        analogWrite(KA_LPWM_PIN, LOW);
        analogWrite(KA_RPWM_PIN, LOW);
        analogWrite(KI_LPWM_PIN, LOW);
        analogWrite(KI_RPWM_PIN, LOW);
        analogWrite(DE_LPWM_PIN, 70);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 70);
        analogWrite(BE_RPWM_PIN, LOW);
        delay(500);
        analogWrite(DE_LPWM_PIN, 20);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 20);
        analogWrite(BE_RPWM_PIN, LOW);
        Auto = false;
        break;
      }
      if (!Auto)
      {
        break;
      }
    }
    SlaveSerial.print('M');
    GoPos = true;
    Ambil = false;
  }

  /* Pergi Ke Zona Lempar */
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
      while (Pos.Y > 40 && !GoLmpr)
      {
        DE.PWM = BE.PWM = KA.PWM = KI.PWM = 0;
        analogWrite(KA_RPWM_PIN, LOW);
        analogWrite(KA_LPWM_PIN, LOW);
        analogWrite(KI_RPWM_PIN, LOW);
        analogWrite(KI_LPWM_PIN, LOW);
        analogWrite(DE_RPWM_PIN, LOW);
        analogWrite(DE_LPWM_PIN, 70);
        analogWrite(BE_RPWM_PIN, LOW);
        analogWrite(BE_LPWM_PIN, 70);
        delay(300);
        Stop();
        delay(2000);
        GoLmpr = true;
        Go = false;
        Slave.Data = 'D';
      }
    }
    while (GoLmpr)
    {
      ThrowZone();
    }
  }
}

void ThrowZone()
{
  LogData();
  param = Slave.Data;

  switch (param)
  {

    /** Zone 1 **/
    case 'A':
      Reset();
      analogWrite(KA_RPWM_PIN, 30);
      analogWrite(KA_LPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, 30);
      analogWrite(KI_LPWM_PIN, LOW);
      analogWrite(DE_LPWM_PIN, LOW);
      analogWrite(DE_RPWM_PIN, 30);
      analogWrite(BE_LPWM_PIN, LOW);
      analogWrite(BE_RPWM_PIN, 30);
      delay(1000);
      analogWrite(KA_RPWM_PIN, 5);
      analogWrite(KA_LPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, 5);
      analogWrite(KI_LPWM_PIN, LOW);
      analogWrite(DE_LPWM_PIN, LOW);
      analogWrite(DE_RPWM_PIN, 5);
      analogWrite(BE_LPWM_PIN, LOW);
      analogWrite(BE_RPWM_PIN, 5);
      SlaveSerial.print('A');
      Slave.Data = 'Z';
      param = 'Z';
      break;
    case 'B':
      Reset();
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('D');
      MoveLeft(-10);
      Slave.Data = 'Z';
      param = 'Z';
      break;
    case 'C':
      Stop();
      Reset();
      delay(100);
      GoLmpr = false;
      Auto1 = false;
      Zone1 = false;
      // Zone2 = true;
      SlaveSerial.print('L');
      delay(50);
      GoTransfer = true;
      Slave.Data = 'Z';
      param = 'Z';
      break;

    /** Zone 2 **/

    case 'D':
      analogWrite(KA_RPWM_PIN, 30);
      analogWrite(KA_LPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, 30);
      analogWrite(KI_LPWM_PIN, LOW);
      analogWrite(DE_LPWM_PIN, 30);
      analogWrite(DE_RPWM_PIN, LOW);
      analogWrite(BE_LPWM_PIN, 30);
      analogWrite(BE_RPWM_PIN, LOW);
      delay(1000);
      analogWrite(KA_RPWM_PIN, 5);
      analogWrite(KA_LPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, 5);
      analogWrite(KI_LPWM_PIN, LOW);
      analogWrite(DE_LPWM_PIN, 5);
      analogWrite(DE_RPWM_PIN, LOW);
      analogWrite(BE_LPWM_PIN, 5);
      analogWrite(BE_RPWM_PIN, LOW);
      SlaveSerial.print('F');
      Slave.Data = 'Z';
      break;
    case 'E':
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('G');
      MoveRigh(10.6);
      Slave.Data = 'Z';
      break;
    case 'F':
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('H');
      MoveRigh(15.7);
      Slave.Data = 'Z';
      break;
    case 'G':
      Geser = true;
      Move = true;
      delay(50);
      SlaveSerial.print('I');
      MoveRigh(33.7);
      Slave.Data = 'Z';
      break;
    case 'H':
      Reset();
      SlaveSerial.print('J');
      Slave.Data = 'Z';
      break;
    case 'I':
      GoLmpr = false;
      GoPos = false;
      Stop();
      Reset();
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
      RPSDepan(4, 0.1, 0.05);
      RPSBelakang(4, 0.1, 0.05);
      analogWrite(KA_RPWM_PIN, LOW);
      analogWrite(KA_LPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, LOW);
      analogWrite(KI_LPWM_PIN, LOW);
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
      analogWrite(KA_RPWM_PIN, LOW);
      analogWrite(KA_LPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, LOW);
      analogWrite(KI_LPWM_PIN, LOW);
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
  DE.PWM = BE.PWM = KA.PWM = KI.PWM = 0;
  analogWrite(DE_LPWM_PIN, LOW);
  analogWrite(DE_RPWM_PIN, LOW);
  analogWrite(BE_LPWM_PIN, LOW);
  analogWrite(BE_RPWM_PIN, LOW);
  analogWrite(KA_RPWM_PIN, 100);
  analogWrite(KA_LPWM_PIN, LOW);
  analogWrite(KI_RPWM_PIN, 100);
  analogWrite(KI_LPWM_PIN, LOW);
  delay(700);
  analogWrite(DE_LPWM_PIN, LOW);
  analogWrite(DE_RPWM_PIN, LOW);
  analogWrite(BE_LPWM_PIN, LOW);
  analogWrite(BE_RPWM_PIN, LOW);
  analogWrite(KA_RPWM_PIN, 5);
  analogWrite(KA_LPWM_PIN, LOW);
  analogWrite(KI_RPWM_PIN, 5);
  analogWrite(KI_LPWM_PIN, LOW);
}

void Stop()
{
  analogWrite(DE_LPWM_PIN, LOW);
  analogWrite(DE_RPWM_PIN, LOW);
  analogWrite(BE_LPWM_PIN, LOW);
  analogWrite(BE_RPWM_PIN, LOW);
  analogWrite(KA_LPWM_PIN, LOW);
  analogWrite(KA_RPWM_PIN, LOW);
  analogWrite(KI_LPWM_PIN, LOW);
  analogWrite(KI_RPWM_PIN, LOW);
  DE.PWM = BE.PWM = KA.PWM = KI.PWM = 0;
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
    analogWrite(DE_LPWM_PIN, LOW);
    analogWrite(DE_RPWM_PIN, LOW);
    analogWrite(BE_LPWM_PIN, LOW);
    analogWrite(BE_RPWM_PIN, LOW);
  }
  if (IR64 == 'B')
  { // BACKWARD
    RPSKanan(-30, 0.1, 0.05);
    RPSKiri(-30, 0.1, 0.05);
    analogWrite(DE_LPWM_PIN, LOW);
    analogWrite(DE_RPWM_PIN, LOW);
    analogWrite(BE_LPWM_PIN, LOW);
    analogWrite(BE_RPWM_PIN, LOW);
  }
  if (IR64 == 'C')
  { // RIGH
    RPSDepan(30, 0.1, 0.05);
    RPSBelakang(30, 0.1, 0.05);
    analogWrite(KA_LPWM_PIN, LOW);
    analogWrite(KA_RPWM_PIN, LOW);
    analogWrite(KI_LPWM_PIN, LOW);
    analogWrite(KI_RPWM_PIN, LOW);
  }
  if (IR64 == 'D')
  { // LEFT
    RPSDepan(-30, 0.1, 0.05);
    RPSBelakang(-30, 0.1, 0.05);
    analogWrite(KA_LPWM_PIN, LOW);
    analogWrite(KA_RPWM_PIN, LOW);
    analogWrite(KI_LPWM_PIN, LOW);
    analogWrite(KI_RPWM_PIN, LOW);
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
    analogWrite(DE_LPWM_PIN, LOW);
    analogWrite(DE_RPWM_PIN, LOW);
    analogWrite(BE_LPWM_PIN, LOW);
    analogWrite(BE_RPWM_PIN, LOW);
    analogWrite(KA_LPWM_PIN, LOW);
    analogWrite(KA_RPWM_PIN, LOW);
    analogWrite(KI_LPWM_PIN, LOW);
    analogWrite(KI_RPWM_PIN, LOW);
    DE.PWM = BE.PWM = KA.PWM = KI.PWM = 0;
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

void trialMotor(int PWM)
{
  analogWrite(DE_LPWM_PIN, LOW);
  analogWrite(DE_RPWM_PIN, PWM);
  analogWrite(BE_LPWM_PIN, LOW);
  analogWrite(BE_RPWM_PIN, PWM);
  analogWrite(KA_LPWM_PIN, LOW);
  analogWrite(KA_RPWM_PIN, PWM);
  analogWrite(KI_LPWM_PIN, LOW);
  analogWrite(KI_RPWM_PIN, PWM);
  delay(1000);
  analogWrite(DE_RPWM_PIN, LOW);
  analogWrite(DE_LPWM_PIN, PWM);
  analogWrite(BE_RPWM_PIN, LOW);
  analogWrite(BE_LPWM_PIN, PWM);
  analogWrite(KA_RPWM_PIN, LOW);
  analogWrite(KA_LPWM_PIN, PWM);
  analogWrite(KI_RPWM_PIN, LOW);
  analogWrite(KI_LPWM_PIN, PWM);
  delay(1000);
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
      DE.PWM = 0;
    }

    error = abs(DE.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    DE.PID = P + D;
    DE.PWM = DE.PWM + DE.PID;

    DE.Time = millis();
    lError = error;
    EncDepan.lastpulse = EncDepan.pulseCnt;

    if (DE.PWM > 255)
      DE.PWM = 255;
    else if (DE.PWM < 0)
      DE.PWM = 0;
  }
}

void MotorDepan(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(DE_RPWM_PIN, DE.PWM);
      analogWrite(DE_LPWM_PIN, LOW);
      break;
    case mundur:
      analogWrite(DE_LPWM_PIN, DE.PWM);
      analogWrite(DE_RPWM_PIN, LOW);
      break;
    case berhenti:
      analogWrite(DE_RPWM_PIN, LOW);
      analogWrite(DE_RPWM_PIN, LOW);
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
      BE.PWM = 0;
    }

    error = abs(BE.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    BE.PID = P + D;
    BE.PWM = BE.PWM + BE.PID;

    BE.Time = millis();
    lError = error;
    EncBelakang.lastpulse = EncBelakang.pulseCnt;

    if (BE.PWM > 255)
      BE.PWM = 255;
    else if (BE.PWM < 0)
      BE.PWM = 0;
  }
}

void MotorBelakang(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(BE_RPWM_PIN, BE.PWM);
      analogWrite(BE_LPWM_PIN, LOW);
      break;
    case mundur:
      analogWrite(BE_LPWM_PIN, BE.PWM);
      analogWrite(BE_RPWM_PIN, LOW);
      break;
    case berhenti:
      analogWrite(BE_RPWM_PIN, LOW);
      analogWrite(BE_RPWM_PIN, LOW);
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
      KA.PWM = 0;
    }

    error = abs(KA.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    KA.PID = P + D;
    KA.PWM = KA.PWM + KA.PID;

    KA.Time = millis();
    lError = error;
    EncKanan.lastpulse = EncKanan.pulseCnt;

    if (KA.PWM > 255)
      KA.PWM = 255;
    else if (KA.PWM < 0)
      KA.PWM = 0;
  }
}

void MotorKanan(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(KA_RPWM_PIN, KA.PWM);
      analogWrite(KA_LPWM_PIN, LOW);
      break;
    case mundur:
      analogWrite(KA_LPWM_PIN, KA.PWM);
      analogWrite(KA_RPWM_PIN, LOW);
      break;
    case berhenti:
      analogWrite(KA_RPWM_PIN, LOW);
      analogWrite(KA_RPWM_PIN, LOW);
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
      KI.PWM = 0;
    }

    error = abs(KI.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    KI.PID = P + D;
    KI.PWM = KI.PWM + KI.PID;

    KI.Time = millis();
    lError = error;
    EncKiri.lastpulse = EncKiri.pulseCnt;

    if (KI.PWM > 255)
      KI.PWM = 255;
    else if (KI.PWM < 0)
      KI.PWM = 0;
  }
}

void MotorKiri(int dir)
{
  switch (dir)
  {
    case maju:
      analogWrite(KI_RPWM_PIN, KI.PWM);
      analogWrite(KI_LPWM_PIN, LOW);
      break;
    case mundur:
      analogWrite(KI_LPWM_PIN, KI.PWM);
      analogWrite(KI_RPWM_PIN, LOW);
      break;
    case berhenti:
      analogWrite(KI_RPWM_PIN, LOW);
      analogWrite(KI_RPWM_PIN, LOW);
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
