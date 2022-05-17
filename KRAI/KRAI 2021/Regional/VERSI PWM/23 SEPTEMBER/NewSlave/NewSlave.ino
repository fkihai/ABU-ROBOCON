/*
   TONG KIRI DEPAN
   a,50,105,0.63,0.05

   TONG KIRI BELAKANG
   a,55,110,0.63,0.05

   TONG TENGAH
   a,29,100,0.3,0.05
*/

#include <Servo.h>
Servo myservo;

#define CHA_Pelontar 23
#define CHB_Pelontar 25

#define CHA_SliderPutar 33
#define CHB_SliderPutar 31
#define CHA_SliderGeser 27
#define CHB_SliderGeser 29

/* PELONTAR */
#define RADIUS_PL 2.9
#define KLL_PL 2 * 3.14 * RADIUS_PL
#define PPR_PL 1000

/* SLIDER PUTAR */
#define RADIUS_SP 11.25
#define KLL_SP 2 * 3.14 * RADIUS_SP
#define PPR_SP 33100

/* SLIDER GESER */
#define RADIUS_SG 2.9
#define KLL_SG 2 * 3.14 * RADIUS_SG
#define PPR_SG 500

/* MOTOR*/
#define PL_RPWM_PIN 11
#define PL_LPWM_PIN 12

#define SG_RPWM_PIN 9
#define SG_LPWM_PIN 10
#define SP_RPWM_PIN 4
#define SP_LPWM_PIN 8

#define PW_RPWM_PIN 3
#define PW_LPWM_PIN 2

#define SliderCptA 45
#define SliderCptB 41

#define SliderA 36
#define SliderB 34

#define Capit5A 38
#define Capit5B 40

#define Capit4A 42
#define Capit4B 44

#define Capit1A 48
#define Capit1B 47

#define Capit2A 53
#define Capit2B 50

#define Capit3A 51
#define Capit3B 49

#define SwtchPutar 2
#define SwtchReset A1
#define SwtchGeser A2
#define SwtchPW    A7

#define Brake A8
#define doorlock2 A9
#define doorlock1 A10

#define maju 1
#define mundur 2
#define berhenti 3

struct ENC
{
  volatile long int pulseCnt;
  long int lastpulse;
  float Angle;
} EncPelontar, EncSliderPutar, EncSliderGeser, EncSlider;

struct Motor
{
  float PID, Pwm, RPS;
  long Time;
} PL, SP, SG;

struct serial
{
  char Data = 'V';
} Master, Joystik;

/* Control */
bool Move = false;
bool Geser = false;
bool Persiapan = false;
bool Throw = false;
bool Down = false;
bool Sensor = false;
bool Revers = false;
bool Putar = false;
bool ModeManual = false;

/* TRIAL */
bool Uncal = false;
bool Uncal1 = false;
bool flag = false;
bool trial = false;

/* Trial Throwing */
bool Coba = false;
char Ctrlthrow = 'Z';

/* Var PWM */
int pwm = 0;
float Distance = 0;
long ltime1 = millis();

/* Tunning */
float Angle;
String SerialData;
String Data[10];
String SplitData;
int StringData;

void setup()
{
  Serial.begin(9600);
  Serial3.begin(9600); // Master
  Serial2.begin(9600); // JoyStik
  digitalWrite(Brake, LOW);
  PinConfig();
  myservo.attach(7);
  ResetEnc();
  ModeManual = true;
}

void loop()
{
  /* Main */
  ManualMode();
  Bismillah();

  /* Trial */
  // LogJoystik();
  // manual();
  // trialThrowing();
  // trialButton();
  // trialLempar();
  // trialRelay();
  // trialServo();
  // trialENC();
  // TuningLempar();
}

void ManualMode() {
  while (ModeManual) {
    LogJoystik();
    manual();
    if (Joystik.Data == 'X') {
      Serial.end();
      Serial2.end();
      digitalWrite(Brake, HIGH);
      ModeManual = false;
      break;
    }
  }
}

void LogData()
{
  if (Serial3.available())
  {
    Master.Data = Serial3.read();
  }
}

void LogJoystik() {
  if (Serial2.available())
  {
    Joystik.Data = Serial2.read();
  }
}

void Bismillah()
{
  LogData();
  switch (Master.Data) {

    /*** KIRI DEPAN GELOMBANG SATU ***/
    case 'A':
      LmprCptType1A(55, 220, 6.0);
      Master.Data = 'a';
      break;
    case 'a':
      Serial3.print('B');
      Master.Data = 'Z';
      break;

    case 'B':
      LmprCptType2A(55, 220, 6.0);
      Master.Data = 'b';
      break;
    case 'b':
      Serial3.print('C');
      Master.Data = 'Z';
      break;
    case 'C':
      LmprCptType3A(55, 220, 6.0);
      Master.Data = 'c';
      break;
    case 'c':
      Serial3.print('D');
      Master.Data = 'Z';
      break;

    /*** KIRI BELAKANG GELOMBANG SATU ***/
    case 'D':
      LmprCptType4A(65, 235, 6.0);
      Master.Data = 'd';
      break;
    case 'd':
      Serial3.print('E');
      Master.Data = 'Z';
      break;

    case 'E':
      LmprCptType5A(60, 238, 6.0);
      Master.Data = 'e';
      break;
    case 'e':
      Serial3.print('N');
      Master.Data = 'Z';
      break;


    /*** KIRI BELAKANG GEL SATU ***/
    case 'F':
      LmprCptType5B(60, 238, 6.0);
      Master.Data = 'f';
      break;
    case 'f':
      Serial3.print('M');
      Master.Data = 'Z';
      break;


    /** Type Lempar Gelombang 3 **/
    case 'G':
      /*
         a,42,180,6.0
      */
      LmprCptType5C(40, 170, 6.0);
      Master.Data = 'H';
      break;

    case 'H':
      LmprCptType4C(40, 170, 6.0);
      Master.Data = 'h';
    case 'h':
      Serial3.print('I');
      Master.Data = 'Z';
      break;

    case 'I':
      /*
         a,43,185,6.0
      */
      LmprCptType3C(40, 170, 6.0);
      Master.Data = 'i';
    case 'i':
      Serial3.print('J');
      Master.Data = 'Z';
      break;

    /*** Balik Kanan 1 ***/
    case 'K':
      delay(100);
      while (true) {
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, 60);
        delay(300);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, 20);
        if (digitalRead(SwtchPW) == LOW) {
          digitalWrite(PW_LPWM_PIN, LOW);
          analogWrite(PW_RPWM_PIN, LOW);
          break;
        }
      }
      Master.Data = 'Z';
      break;

    /*** Balik Kanan 2 ***/
    case 'Q':
      delay(100);
      while (true) {
        ServoMundur();
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, 60);
        delay(300);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, 20);
        if (digitalRead(SwtchPW) == LOW) {
          digitalWrite(PW_LPWM_PIN, LOW);
          analogWrite(PW_RPWM_PIN, LOW);
          break;
        }
      }
      Master.Data = 'Z';
      break;

    case 'L':
      CapitBalikKiri();
      delay(100);
      Master.Data = 'Z';
      break;
    case 'M':
      ambilPanah(5);
      delay(500);
      Serial3.print('L');
      Master.Data = 'Z';
      break;
    case 'N':
      ambilPanah(1);
      delay(500);
      Serial3.print('L');
      Master.Data = 'Z';
      break;
    case 'O':
      ambilPanah(3);
      delay(500);
      Serial3.print('L');
      Master.Data = 'Z';
      break;
    default :
      break;
  }
}

void ambilPanah(int Jumlah)
{
  delay(200);
  PneumaticTutup(Jumlah);
  delay(200);
  digitalWrite(PW_LPWM_PIN, LOW);
  analogWrite(PW_RPWM_PIN, LOW);
}

void CapitBalikKanan(int Speed)
{
  digitalWrite(PW_LPWM_PIN, LOW);
  analogWrite(PW_RPWM_PIN, Speed);
}

void CapitBalikKiri()
{
  digitalWrite(PW_RPWM_PIN, LOW);
  analogWrite(PW_LPWM_PIN, 90);
  delay(600);
  PneumaticBuka();
  delay(100);
}

void CapitStop()
{
  digitalWrite(PW_LPWM_PIN, LOW);
  analogWrite(PW_RPWM_PIN, LOW);
}

void PelontarOn() {
  digitalWrite(PL_RPWM_PIN, LOW);
  analogWrite(PL_LPWM_PIN, 30);
  delay(100);
  digitalWrite(PL_RPWM_PIN, LOW);
  analogWrite(PL_LPWM_PIN, 10);
}

void PelontarOff() {
  digitalWrite(PL_RPWM_PIN, LOW);
  analogWrite(PL_LPWM_PIN, LOW);
}

void ServoMaju() {
  const int angleIncrement = 1;
  const int incrementDelay = 10;
  for (int angle = 100; angle > 0; angle -= angleIncrement) {
    myservo.write(angle);
    delay(incrementDelay);
  }
}

void ServoMundur() {
  myservo.write(100);
  delay(15);
}

void Lontar(float SudutLntr, int Speed, float Pembagi) {
  while (Throw)
  {
    PlCompute();
    float redam =  SudutLntr / Pembagi;
    if (EncPelontar.Angle < redam) {
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, 40);
    } else if (EncPelontar.Angle < SudutLntr) {
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, Speed);
    }
    else {
      digitalWrite(Brake, LOW);
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, 0);
      delay(600);
      Throw = false;
      Down = true;
      break;

    }
  }
}

void Transfer(int capitA, int capitB, int Delay)
{
  analogWrite(SG_RPWM_PIN, LOW);
  digitalWrite(SG_LPWM_PIN, LOW);
  delay(100);
  digitalWrite(SliderA, HIGH);
  delay(1000);
  digitalWrite(SliderA, LOW);
  digitalWrite(capitA, HIGH);
  delay(1000);
  digitalWrite(capitA, LOW);
  digitalWrite(SliderB, HIGH);
  digitalWrite(PW_RPWM_PIN, LOW);
  analogWrite(PW_LPWM_PIN, 73);
  delay(Delay);
  digitalWrite(SliderB, LOW);
  digitalWrite(PW_RPWM_PIN, LOW);
  analogWrite(PW_LPWM_PIN, LOW);
  delay(200);
}


/******* Type A **************/
void LmprCptType1A(float SudutLntr, int Speed, float Pembagi)
{
  Geser = true;
  Move = true;
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Geser)
    {
      EncCompute();
      analogWrite(SG_LPWM_PIN, 70);
      digitalWrite(SG_RPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, 200);
      digitalWrite(SP_RPWM_PIN, LOW);
      if (EncSliderPutar.Angle > 10)
      {
        analogWrite(SG_LPWM_PIN, 40);
        digitalWrite(SG_RPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 60);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(300);
        PelontarOff();
        analogWrite(SP_LPWM_PIN, 27);
        digitalWrite(SP_RPWM_PIN, LOW);
        Transfer(Capit1A, Capit1B, 650);
        delay(200);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Geser = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);

    while (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      analogWrite(SG_LPWM_PIN, LOW);
      digitalWrite(SG_RPWM_PIN, LOW);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        ServoMundur();
        delay(450);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void LmprCptType2A(float SudutLntr, int Speed, float Pembagi)
{
  Geser = true;
  Move = true;
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Geser)
    {
      EncCompute();
      CapitBalikKanan(50);
      analogWrite(SG_RPWM_PIN, 60);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW && Distance > 15)
      {
        PelontarOff();
        Sensor = true;
      }
      if (Sensor)
      {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        Transfer(Capit2A, Capit2B, 650);
        delay(200);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Geser = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);

    while (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        ServoMundur();
        delay(300);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void LmprCptType3A(float SudutLntr, int Speed, float Pembagi)
{
  pwm = 60;
  Geser = true;
  Move = true;
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Geser)
    {
      EncCompute();
      CapitBalikKanan(50);
      analogWrite(SG_RPWM_PIN, pwm);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW && Distance > 13)
      {
        PelontarOff();
        Sensor = true;
      }
      if (Sensor)
      {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        Transfer(Capit3A, Capit3B, 600);
        delay(200);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Geser = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);

    while (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        ServoMundur();
        delay(300);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void LmprCptType4A(float SudutLntr, int Speed, float Pembagi)
{
  pwm = 50;
  Geser = true;
  Move = true;
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Geser)
    {
      EncCompute();
      CapitBalikKanan(50);
      analogWrite(SG_RPWM_PIN, pwm);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW && Distance > 13)
      {
        PelontarOff();
        Sensor = true;
      }
      if (Sensor)
      {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        Transfer(Capit4A, Capit4B, 600);
        delay(200);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Throw = true;
        Geser = false;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);


    while (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        ServoMundur();
        delay(300);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void LmprCptType5A(float SudutLntr, int Speed, float Pembagi)
{
  Move = true;
  Putar = true;
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Putar) {
      digitalWrite(doorlock1, HIGH);
      delay(100);
      ResetEnc();
      EncCompute();
      CapitBalikKanan(50);
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      delay(300);
      Geser = true;
      Putar = false;
      break;
    }

    while (Geser)
    {
      EncCompute();
      PelontarOff();
      analogWrite(SP_LPWM_PIN, 100);
      digitalWrite(SP_RPWM_PIN, LOW);
      analogWrite(SG_RPWM_PIN, 60);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (EncSliderPutar.Angle > 21.0)
      {
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        digitalWrite(doorlock1, LOW);
        Transfer(Capit5A, Capit5B, 520);
        delay(100);
        ResetEnc();
        Persiapan = true;
        Geser = false;
        break;
      }
    }

    while (Persiapan)
    {
      EncCompute();
      digitalWrite(doorlock1, HIGH);
      analogWrite(SP_RPWM_PIN, 100);
      digitalWrite(SP_LPWM_PIN, LOW);
      if (EncSliderPutar.Angle < -36)
      {
        digitalWrite(doorlock1, LOW);
        analogWrite(SP_LPWM_PIN, 150);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(700);
        analogWrite(SP_LPWM_PIN, 40);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(200);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Persiapan = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);


    while (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(100);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Move = false;
        Down = false;
        break;
      }
    }
  }
}

/******* Type B **************/
void LmprCptType5B(float SudutLntr, int Speed, float Pembagi) {
  Move = true;
  Putar = true;
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Putar) {
      digitalWrite(doorlock1, HIGH);
      delay(100);
      ResetEnc();
      EncCompute();
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      delay(300);
      Geser = true;
      Putar = false;
      break;
    }

    while (Geser)
    {
      EncCompute();
      PelontarOff();
      analogWrite(SP_LPWM_PIN, 100);
      digitalWrite(SP_RPWM_PIN, LOW);
      analogWrite(SG_RPWM_PIN, 60);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (EncSliderPutar.Angle > 21.0)
      {
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        analogWrite(SG_RPWM_PIN, 30);
        digitalWrite(SG_LPWM_PIN, LOW);
        digitalWrite(doorlock1, LOW);
        Transfer(Capit5A, Capit5B, 650);
        PelontarOn();
        delay(100);
        PelontarOff();
        ResetEnc();
        Persiapan = true;
        Geser = false;
        break;
      }
    }

    while (Persiapan)
    {
      EncCompute();
      digitalWrite(doorlock1, HIGH);
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      if (EncSliderPutar.Angle < -36)
      {
        digitalWrite(doorlock1, LOW);
        analogWrite(SP_LPWM_PIN, 150);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(700);
        analogWrite(SP_LPWM_PIN, 40);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(200);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Persiapan = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);

    while (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(100);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Move = false;
        Down = false;
        break;
      }
    }
  }
}


/******* Type C **************/
void LmprCptType5C(float SudutLntr, int Speed, float Pembagi) {
  Move = true;
  Putar = true;
  PelontarOn();
  ResetEnc();
  while (Move) {
    while (Putar) {
      digitalWrite(doorlock1, HIGH);
      delay(100);
      ResetEnc();
      EncCompute();
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      delay(300);
      Geser = true;
      Putar = false;
      break;
    }
    while (Geser) {
      EncCompute();
      PelontarOff();
      analogWrite(SG_RPWM_PIN, 80);
      digitalWrite(SG_LPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, 100);
      digitalWrite(SP_RPWM_PIN, LOW);
      if (EncSliderPutar.Angle > 21.0)
      {
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(doorlock1, LOW);
        Transfer(Capit5A, Capit5B, 600);
        delay(100);
        ResetEnc();
        Persiapan = true;
        Geser = false;
        break;
      }
    }
    while (Persiapan) {
      EncCompute();
      analogWrite(SG_LPWM_PIN, 50);
      digitalWrite(SG_RPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, 30);
      digitalWrite(SP_RPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW) {
        Sensor = true;
      } if (Sensor) {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        delay(500);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Persiapan = false;
        Throw = true;
        break;
      }
    }
    Lontar(SudutLntr, Speed, Pembagi);
    while (Down) {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        digitalWrite(SG_RPWM_PIN, LOW);
        analogWrite(SG_LPWM_PIN, LOW);
        ServoMundur();
        delay(400);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void LmprCptType4C(float SudutLntr, int Speed, float Pembagi) {
  Geser = true;
  Move = true;
  pwm = 70;
  digitalWrite(doorlock1, HIGH);
  PelontarOn();
  ResetEnc();
  while (Move)
  {
    while (Geser)
    {
      EncCompute();
      CapitBalikKanan(60);
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      analogWrite(SG_RPWM_PIN, 70);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (abs(EncSliderPutar.Angle) >= 36) {
        PelontarOff();
        digitalWrite(SP_LPWM_PIN, LOW);
        analogWrite(SG_RPWM_PIN, 20);
        digitalWrite(doorlock1, LOW);
        analogWrite(SP_RPWM_PIN, 150);
        digitalWrite(SP_LPWM_PIN, LOW);
        delay(500);
        analogWrite(SP_RPWM_PIN, 30);
        digitalWrite(SP_LPWM_PIN, LOW);
        delay(100);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        Transfer(Capit4A, Capit4B, 650);
        delay(100);
        ResetEnc();
        Sensor = false;
        Persiapan = true;
        Geser = false;
        break;
      }
    }

    while (Persiapan) {
      EncCompute();
      digitalWrite(doorlock1, HIGH);
      analogWrite(SG_LPWM_PIN, pwm);
      digitalWrite(SG_RPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, 200);
      digitalWrite(SP_RPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW && Distance < -10) {
        pwm = 0;
        Sensor = true;
      } if (Sensor) {
        pwm = 0;
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
      }
      if (EncSliderPutar.Angle > 35) {
        digitalWrite(doorlock1, LOW);
        analogWrite(SP_LPWM_PIN, 110);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
      }

      if (Sensor && EncSliderPutar.Angle > 40) {
        delay(500);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Persiapan = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);

    while (Down) {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        ServoMundur();
        delay(400);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void LmprCptType3C(float SudutLntr, int Speed, float Pembagi) {

  Putar = true;
  Move = true;
  pwm = 50;
  PelontarOn();
  ResetEnc();

  while (Move)
  {
    while (Putar) {
      EncCompute();
      digitalWrite(doorlock1, HIGH);
      analogWrite(SP_RPWM_PIN, 255);
      digitalWrite(SP_LPWM_PIN, LOW);
      if (abs(EncSliderPutar.Angle) >= 37) {
        PelontarOff();
        digitalWrite(doorlock1, LOW);
        analogWrite(SP_LPWM_PIN, 60);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        analogWrite(SP_LPWM_PIN, 20);
        digitalWrite(SP_RPWM_PIN, LOW);
        ResetEnc();
        Putar = false;
        Geser = true;
      }
    }

    while (Geser)
    {
      EncCompute();
      CapitBalikKanan(60);
      analogWrite(SG_LPWM_PIN, 60);
      digitalWrite(SG_RPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW && Distance < -10) {
        Sensor = true;
      } if (Sensor) {
        delay(30);
        digitalWrite(PW_LPWM_PIN, LOW);
        analogWrite(PW_RPWM_PIN, LOW);
        analogWrite(SG_LPWM_PIN, LOW);
        digitalWrite(SG_RPWM_PIN, LOW);
        delay(200);
        Transfer(Capit3A, Capit3B, 650);
        ResetEnc();
        Geser = false;
        Sensor = false;
        Revers = true;
      }
    }

    while (Revers) {
      digitalWrite(doorlock1, HIGH);
      delay(100);
      ResetEnc();
      EncCompute();
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      delay(100);
      Persiapan = true;
      Revers = false;
      break;
    }

    while (Persiapan) {
      EncCompute();
      analogWrite(SP_LPWM_PIN, 200);
      digitalWrite(SP_RPWM_PIN, LOW);
      if (EncSliderPutar.Angle >= 25) {
        digitalWrite(doorlock1, LOW);
        analogWrite(SP_LPWM_PIN, 70);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        ServoMaju();
        delay(1650);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Persiapan = false;
        Throw = true;
        break;
      }
    }

    Lontar(SudutLntr, Speed, Pembagi);

    while (Down) {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 80);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        ServoMundur();
        delay(100);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Down = false;
        Move = false;
        break;
      }
    }
  }
}

void trialThrowing()
{
  LogJoystik();
  manual();
  if (Joystik.Data == 'X')
  {
    digitalWrite(Brake, HIGH);
    Coba = true;
  }
  else if (Coba)
  {
    LmprCptType5C(56, 236, 6.0);
    Ctrlthrow = 'A';
    Coba = false;
  }
  switch (Ctrlthrow)
  {
    case 'A':
      LmprCptType4C(56, 236, 6.0);
      Ctrlthrow = 'B';
      break;
    case 'B':
      LmprCptType3C(56, 236, 6.0);
      Ctrlthrow = 'Z';
      break;
    case 'C':
      LmprCptType4A(56, 236, 6.0);
      Ctrlthrow = 'D';
      break;
    case 'D':
      LmprCptType5A(56, 236, 6.0);
      Ctrlthrow = 'Z';
    default:
      break;
  }
}


void PneumaticBuka()
{
  digitalWrite(Capit1A, HIGH);
  digitalWrite(Capit2A, HIGH);
  digitalWrite(Capit3A, HIGH);
  digitalWrite(Capit4A, HIGH);
  digitalWrite(Capit5A, HIGH);
  delay(100);
  digitalWrite(Capit1A, LOW);
  digitalWrite(Capit2A, LOW);
  digitalWrite(Capit3A, LOW);
  digitalWrite(Capit4A, LOW);
  digitalWrite(Capit5A, LOW);
}

void PneumaticTutup(int Jumlah)
{
  if (Jumlah == 5) {
    digitalWrite(Capit1B, HIGH);
    digitalWrite(Capit2B, HIGH);
    digitalWrite(Capit3B, HIGH);
    digitalWrite(Capit4B, HIGH);
    digitalWrite(Capit5B, HIGH);
    delay(100);
    digitalWrite(Capit1B, LOW);
    digitalWrite(Capit2B, LOW);
    digitalWrite(Capit3B, LOW);
    digitalWrite(Capit4B, LOW);
    digitalWrite(Capit5B, LOW);
  } else if (Jumlah == 1) {
    digitalWrite(Capit5B, HIGH);
    delay(100);
    digitalWrite(Capit5B, LOW);
  } else if (Jumlah == 3) {
    digitalWrite(Capit3B, HIGH);
    digitalWrite(Capit4B, HIGH);
    digitalWrite(Capit5B, HIGH);
    delay(100);
    digitalWrite(Capit3B, LOW);
    digitalWrite(Capit4B, LOW);
    digitalWrite(Capit5B, LOW);
  }
}

void EncCompute()
{
  Distance = KLL_SG * (float(EncSliderGeser.pulseCnt) / float(PPR_SG));
  EncSliderPutar.Angle = (float(EncSliderPutar.pulseCnt) / PPR_SP) * 360;
}

void PlCompute()
{
  EncPelontar.Angle = (float(EncPelontar.pulseCnt) / PPR_PL) * 360;
}

void ResetEnc()
{
  EncSliderGeser.pulseCnt = 0;
  EncSliderPutar.pulseCnt = 0;
  EncSlider.pulseCnt = 0;

  EncSliderPutar.Angle = 0.0;
  EncSlider.Angle = 0.0;
  Distance = 0.0;
}

/***********   ***********/

void manual()
{
  /* SLIDER CAPIT  */
  if (Joystik.Data == 'H')
  {
    // MAJU (CROSS)
    // digitalWrite(SliderA, HIGH);
    // digitalWrite(SliderB, LOW);
    digitalWrite(Brake, HIGH);
    digitalWrite(doorlock1, HIGH);
  }
  if (Joystik.Data == 'I')
  { // MUNDUR (CROSS)
    // digitalWrite(SliderA, LOW);
    // digitalWrite(SliderB, HIGH);
    digitalWrite(Brake, LOW);
    digitalWrite(doorlock1, LOW);
  }

  /* SLIDER PUTAR */
  if (Joystik.Data == 'Q')
  { // Q
    analogWrite(SP_RPWM_PIN, 165); // PUTAR KANAN
    digitalWrite(SP_LPWM_PIN, LOW);
  }
  if (Joystik.Data == 'P')
  { // P
    digitalWrite(SP_RPWM_PIN, LOW); // PUTAR KIRI
    analogWrite(SP_LPWM_PIN, 165);
  }

  /* SLIDER GESER */
  if (Joystik.Data == 'T')
  {
    analogWrite(SG_RPWM_PIN, 80); // GESER KANAN
    digitalWrite(SG_LPWM_PIN, LOW);
  }
  if (Joystik.Data == 'U')
  {
    digitalWrite(SG_RPWM_PIN, LOW); // GESER KIRI
    analogWrite(SG_LPWM_PIN, 80);
  }
  /* POWER WINDOW */
  if (Joystik.Data == 'R')
  {
    analogWrite(PW_RPWM_PIN, 60); // PUTAR MAJU
    digitalWrite(PW_LPWM_PIN, LOW);
  }
  if (Joystik.Data == 'S')
  {
    digitalWrite(PW_RPWM_PIN, LOW); // PUTAR MUNDUR
    analogWrite(PW_LPWM_PIN, 60);
  }

  /* ALL IN ONE */
  //  if (Joystik.Data == 'F')
  //  {
  //    PneumaticBuka(); // Circle)
  //  }
  //  if (Joystik.Data == 'G')
  //  {
  //    PneumaticTutup(5); // (Circle)
  //  }

  if (Joystik.Data == 'Z')
  {
    digitalWrite(Capit1A, LOW);
    digitalWrite(Capit1B, LOW);
    digitalWrite(Capit2A, LOW);
    digitalWrite(Capit2B, LOW);
    digitalWrite(Capit3A, LOW);
    digitalWrite(Capit3B, LOW);
    digitalWrite(Capit4A, LOW);
    digitalWrite(Capit4B, LOW);
    digitalWrite(Capit5A, LOW);
    digitalWrite(Capit5B, LOW);
    digitalWrite(SliderA, LOW);
    digitalWrite(SliderB, LOW);
    digitalWrite(PL_RPWM_PIN, LOW);
    analogWrite(PL_LPWM_PIN, LOW);
    digitalWrite(SG_RPWM_PIN, LOW);
    analogWrite(SG_LPWM_PIN, LOW);
    digitalWrite(SP_RPWM_PIN, LOW);
    analogWrite(SP_LPWM_PIN, LOW);
    digitalWrite(PW_RPWM_PIN, LOW);
    analogWrite(PW_LPWM_PIN, LOW);
  }

  if (Joystik.Data == '*') // Start (Reset)
  {
    ResetEnc();
    EncPelontar.pulseCnt = 0;
    EncPelontar.Angle = 0;
    Move = false;
    Geser = false;
    Persiapan = false;
    Throw = false;
    Down = false;
    Sensor = false;
    Putar = false;
  }
}


/*** Fungsi Trial ***/
void trialRelay() {
  String IR64;
  if (Serial.available()) {
    IR64 = Serial.readStringUntil('\n');
  }
  if (IR64 == "a") {
    digitalWrite(Brake, HIGH);
  } else if (IR64 == "b") {
    digitalWrite(Brake, LOW);
  } else if (IR64 == "c") {
    digitalWrite(doorlock1, HIGH);
  } else if (IR64 == "d") {
    digitalWrite(doorlock1, LOW);
  } else if (IR64 == "e") {
    digitalWrite(doorlock2, HIGH);
  } else if (IR64 == "f") {
    digitalWrite(doorlock2, LOW);
  } else if (IR64 == "g") {
    digitalWrite(SliderCptA, HIGH);
    delay(100);
    digitalWrite(SliderCptA, LOW);
  } else if (IR64 == "h") {
    digitalWrite(SliderCptB, HIGH);
    delay(100);
    digitalWrite(SliderCptB, LOW);
  }
}

void trialServo() {
  String serial;
  if (Serial.available()) {
    serial = Serial.readStringUntil('\n');
  }
  if (serial == "a") {
    ServoMaju();
  } else if (serial == "b") {
    ServoMundur();
  } else if (serial == "c") {
    myservo.write(180);
    delay(15);
  }
}

void TuningLempar() {
  digitalWrite(Brake, HIGH);
  delay(300);
  trial = true;
  while (trial) {
    Angle = (float(EncPelontar.pulseCnt) / PPR_PL) * 360;
    if (Serial.available()) {
      SerialData = Serial.readString();
      for (int a = 0; a < SerialData.length(); a++) {
        Data[a] = GetData(SerialData, ',', a);
        //        if (Data[a] != NULL) {
        //          Serial.print("Data ke ");
        //          Serial.print(a);
        //          Serial.print(" = ");
        //          Serial.println(Data[a]);
        //        }
      }
    }

    if (Data[0] == "a") {
      EncPelontar.pulseCnt = 0;
      EncPelontar.Angle = 0.0;
      Throw = true;
      Data[0] = "c";
    } else if (Data[0] == "b") {
      Down = true;
      Data[0] = "c";
    }

    while (Throw)
    {
      PlCompute();
      float redam =  Data[1].toFloat() / Data[3].toFloat();
      if (EncPelontar.Angle < redam) {
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 40);
      } else if (EncPelontar.Angle < Data[1].toFloat()) {
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, Data[2].toInt());
      }
      else {
        digitalWrite(Brake, LOW);
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 0);
        delay(600);
        Throw = false;
        Down = true;
        break;

      }
    }

    while (Down) {
      PlCompute();
      digitalWrite(Brake, HIGH);
      delay(100);
      analogWrite(PL_LPWM_PIN, 50);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        delay(150);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        RPSpelontar(0, 0, 0);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Data[0] = "D";
        Down = false;
        break;
      }
    }
  }
}

String GetData(String data, char Spliter, int Number) {
  StringData = 0;
  SplitData = "";

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == Spliter) {
      StringData++;
    }
    else if (StringData == Number) {
      SplitData.concat(data[i]);
    }
    else if (StringData > Number) {
      return SplitData;
      break;
    }
  }
  return SplitData;
}


void trialLempar()
{
  /** LEMPAR TIPE A **/
  if (Joystik.Data == 'J') // square
  {
    digitalWrite(Brake, HIGH);
    delay(100);
    EncPelontar.Angle = 0;
    EncPelontar.pulseCnt = 0;
    Throw = true;
  }

  while (Throw)
  {
    PlCompute();
    if (EncPelontar.Angle < 48) {
      digitalWrite(Brake, HIGH);
      RPSpelontar(90, 0.32, 0.03);
    }
    else {
      digitalWrite(Brake, LOW);
      RPSpelontar(0, 0, 0);
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, 0);
      delay(300);
      Down = true;
      Throw = false;
      break;
    }
  }

  while (Down) {
    PlCompute();
    digitalWrite(Brake, HIGH);
    delay(100);
    digitalWrite(PL_RPWM_PIN, LOW);
    analogWrite(PL_LPWM_PIN, 50);
    if (EncPelontar.Angle < 5)
    {
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, LOW);
      digitalWrite(Brake, LOW);
      ResetEnc();
      Down = false;
      break;
    }
  }
}

void trialENC()
{

  EncPelontar.Angle = (float(EncPelontar.pulseCnt) / PPR_PL) * 360;
  Distance = KLL_SG * (float(EncSliderGeser.pulseCnt) / float(PPR_SG));
  EncSliderPutar.Angle = (float(EncSliderPutar.pulseCnt) / PPR_SP) * 360;
  EncSlider.Angle = KLL_SG * (float(EncSlider.pulseCnt) / float(PPR_SG));

  /*
    Serial.print("ENC_PL: ");
    Serial.print(EncPelontar.pulseCnt);
    Serial.print('\t');
    Serial.print("ENC_SP: ");
    Serial.print(EncSliderPutar.pulseCnt);
    Serial.print('\t');
    Serial.print("ENC_SG: ");
    Serial.println(EncSliderGeser.pulseCnt);
  */

  Serial.print("ANGLE_SP: ");
  Serial.print(EncSliderPutar.Angle);
  Serial.print('\t');
  Serial.print("DISTANCE: ");
  Serial.print(Distance);
  Serial.print('\t');
  Serial.print("Angle.PL: ");
  Serial.print(EncPelontar.Angle);
  Serial.print('\t');
  Serial.print("SwtchPutarG ");
  Serial.println(digitalRead(SwtchGeser));
}

void trialButton()
{
  Serial.print("BtnPW: ");
  Serial.print(digitalRead(SwtchPW));
  Serial.print('\t');
  Serial.print("BtnGeser: ");
  Serial.print(digitalRead(SwtchGeser));
  Serial.print('\t');
  Serial.print("BtnPutar: ");
  Serial.print(digitalRead(SwtchPutar));
  Serial.print('\t');
  Serial.print("BtnReset: ");
  Serial.println(digitalRead(SwtchReset));
}

void PinConfig()
{
  pinMode(CHA_Pelontar, INPUT_PULLUP);
  pinMode(CHB_Pelontar, INPUT_PULLUP);
  pinMode(CHA_SliderPutar, INPUT_PULLUP);
  pinMode(CHB_SliderPutar, INPUT_PULLUP);
  pinMode(CHA_SliderGeser, INPUT_PULLUP);
  pinMode(CHB_SliderGeser, INPUT_PULLUP);
  pinMode(SwtchPutar, INPUT_PULLUP);
  pinMode(SwtchReset, INPUT_PULLUP);
  pinMode(SwtchGeser, INPUT_PULLUP);
  pinMode(SwtchPW, INPUT_PULLUP);

  /* MOTOR */
  pinMode(PL_RPWM_PIN, OUTPUT);
  pinMode(PL_LPWM_PIN, OUTPUT);
  pinMode(SP_RPWM_PIN, OUTPUT);
  pinMode(SP_LPWM_PIN, OUTPUT);
  pinMode(SG_RPWM_PIN, OUTPUT);
  pinMode(SG_LPWM_PIN, OUTPUT);
  pinMode(PW_RPWM_PIN, OUTPUT);
  pinMode(PW_LPWM_PIN, OUTPUT);

  /* SELENOID */
  pinMode(Capit1A, OUTPUT);
  pinMode(Capit1B, OUTPUT);
  pinMode(Capit2A, OUTPUT);
  pinMode(Capit2B, OUTPUT);
  pinMode(Capit3A, OUTPUT);
  pinMode(Capit3B, OUTPUT);
  pinMode(Capit4A, OUTPUT);
  pinMode(Capit4B, OUTPUT);
  pinMode(Capit5A, OUTPUT);
  pinMode(Capit5B, OUTPUT);
  pinMode(SliderA, OUTPUT);
  pinMode(SliderB, OUTPUT);
  pinMode(SliderCptA, OUTPUT);
  pinMode(SliderCptB, OUTPUT);
  pinMode(doorlock1, OUTPUT);
  pinMode(doorlock2, OUTPUT);
  pinMode(Brake, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(CHA_Pelontar), encPelontar_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB_Pelontar), encPelontar_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHA_SliderPutar), encSliderPutar_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB_SliderPutar), encSliderPutar_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHA_SliderGeser), encSliderGeser_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB_SliderGeser), encSliderGeser_INTT_B, CHANGE);

  digitalWrite(Capit1A, LOW);
  digitalWrite(Capit1B, LOW);
  digitalWrite(Capit2A, LOW);
  digitalWrite(Capit2B, LOW);
  digitalWrite(Capit3A, LOW);
  digitalWrite(Capit3B, LOW);
  digitalWrite(Capit4A, LOW);
  digitalWrite(Capit4B, LOW);
  digitalWrite(Capit5A, LOW);
  digitalWrite(Capit5B, LOW);
  digitalWrite(SliderA, LOW);
  digitalWrite(SliderB, LOW);
  digitalWrite(doorlock1, LOW);
  digitalWrite(doorlock2, LOW);
  digitalWrite(SliderB, HIGH);
  delay(100);
  digitalWrite(SliderB, LOW);
  ServoMundur();

  EncPelontar.Angle = EncPelontar.pulseCnt = 0;
  EncSliderGeser.pulseCnt = EncSliderPutar.pulseCnt = 0;
  EncSliderPutar.Angle = EncSliderPutar.pulseCnt = 0;

  PL.Time = millis();

}

/*********** Soft Speed ***********/

void SoftSpeed(float ParamA, float a, int dist, int Speed)
{
  if ((millis() - ltime1) > 20 && dist > ParamA)
  {
    ltime1 = millis();
    pwm -= a;
    if (pwm < Speed)
    {
      pwm = Speed;
    }
  }
}

/*********** RPS MOTOR ***********/

void RPSpelontar(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1;
  float P, I, D, error, lError = 0;
  int cRPS;
  PL.RPS = RPS;

  if ((millis() - PL.Time) > 10)
  {
    cRPS = abs(EncPelontar.pulseCnt) - abs(EncPelontar.lastpulse);

    if (PL.RPS > 0) {
      MotorPelontar(maju);
    }
    else if (PL.RPS < 0) {
      MotorPelontar(mundur);
    }
    else {
      MotorPelontar(berhenti);
      PL.Pwm = 0;
    }

    error = abs(PL.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    PL.PID = P + D;
    PL.Pwm = PL.Pwm + PL.PID;

    PL.Time = millis();
    lError = error;
    EncPelontar.lastpulse = EncPelontar.pulseCnt;

    if (PL.Pwm > 255)
      PL.Pwm = 255;
    else if (PL.Pwm < 0)
      PL.Pwm = 0;
  }
}

void MotorPelontar(int dir)
{
  switch (dir)
  {
    case maju:
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, PL.Pwm);
      break;
    case mundur:
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, PL.Pwm);
      break;
    case berhenti:
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 0);
      break;
  }
}

void RPSsliderPutar(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;
  float P, I, D, error, lError = 0;
  int cRPS;
  SP.RPS = RPS;

  if ((millis() - SP.Time) > 10)
  {
    cRPS = abs(EncSliderPutar.pulseCnt) - abs(EncSliderPutar.lastpulse);
    if (SP.RPS > 0)
    {
      MotorSliderPutar(maju);
    }
    else if (SP.RPS < 0)
    {
      MotorSliderPutar(mundur);
    }
    else
    {
      MotorSliderPutar(berhenti);
      SP.Pwm = 0;
    }

    error = abs(SP.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    SP.PID = P + D;
    SP.Pwm = SP.Pwm + SP.PID;

    SP.Time = millis();
    lError = error;
    EncSliderPutar.lastpulse = EncSliderPutar.pulseCnt;

    if (SP.Pwm > 255)
      SP.Pwm = 255;
    else if (SP.Pwm < 0)
      SP.Pwm = 0;
  }
}

void MotorSliderPutar(int dir)
{
  switch (dir)
  {
    case maju:
      digitalWrite(SP_RPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, SP.Pwm);
      break;
    case mundur:
      digitalWrite(SP_LPWM_PIN, LOW);
      analogWrite(SP_RPWM_PIN, SP.Pwm);
      break;
    case berhenti:
      digitalWrite(SP_RPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, 0);
      break;
  }
}

void RPSsliderGeser(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;
  float P, I, D, error, lError = 0;
  int cRPS;
  SG.RPS = RPS;

  if ((millis() - SG.Time) > 10)
  {
    cRPS = abs(EncSliderGeser.pulseCnt) - abs(EncSliderGeser.lastpulse);
    if (SG.RPS > 0)
    {
      MotorSliderGeser(maju);
    }
    else if (SG.RPS < 0)
    {
      MotorSliderGeser(mundur);
    }
    else
    {
      MotorSliderGeser(berhenti);
      SG.Pwm = 0;
    }

    error = abs(SG.RPS) - abs(cRPS);
    P = Kp * error;
    D = ((Kd / TS) * (error - lError));
    SG.PID = P + D;
    SG.Pwm = SG.Pwm + SG.PID;

    SG.Time = millis();
    lError = error;
    EncSliderGeser.lastpulse = EncSliderGeser.pulseCnt;

    if (SG.Pwm > 255)
      SG.Pwm = 255;
    else if (SG.Pwm < 0)
      SG.Pwm = 0;
  }
}

void MotorSliderGeser(int dir)
{
  switch (dir)
  {
    case maju:
      digitalWrite(SG_RPWM_PIN, LOW);
      analogWrite(SG_LPWM_PIN, SG.Pwm);
      break;
    case mundur:
      digitalWrite(SG_LPWM_PIN, LOW);
      analogWrite(SG_RPWM_PIN, SG.Pwm);
      break;
    case berhenti:
      digitalWrite(SG_RPWM_PIN, LOW);
      analogWrite(SG_LPWM_PIN, 0);
      break;
  }
}

/*********** ENC COUNT ***********/

void encPelontar_INTT_A()
{
  if (digitalRead(CHA_Pelontar) == HIGH)
  {
    if (digitalRead(CHB_Pelontar) == LOW)
    {
      EncPelontar.pulseCnt++;
    }
    else
    {
      EncPelontar.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(CHB_Pelontar) == HIGH)
    {
      EncPelontar.pulseCnt++;
    }
    else
    {
      EncPelontar.pulseCnt--;
    }
  }
}

void encPelontar_INTT_B()
{
  if (digitalRead(CHA_Pelontar) == HIGH)
  {
    if (digitalRead(CHB_Pelontar) == HIGH)
    {
      EncPelontar.pulseCnt++;
    }
    else
    {
      EncPelontar.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(CHB_Pelontar) == LOW)
    {
      EncPelontar.pulseCnt++;
    }
    else
    {
      EncPelontar.pulseCnt--;
    }
  }
}

void encSliderPutar_INTT_A()
{
  if (digitalRead(CHA_SliderPutar) == HIGH)
  {
    if (digitalRead(CHB_SliderPutar) == LOW)
    {
      EncSliderPutar.pulseCnt++;
    }
    else
    {
      EncSliderPutar.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(CHB_SliderPutar) == HIGH)
    {
      EncSliderPutar.pulseCnt++;
    }
    else
    {
      EncSliderPutar.pulseCnt--;
    }
  }
}

void encSliderPutar_INTT_B()
{
  if (digitalRead(CHA_SliderPutar) == HIGH)
  {
    if (digitalRead(CHB_SliderPutar) == HIGH)
    {
      EncSliderPutar.pulseCnt++;
    }
    else
    {
      EncSliderPutar.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(CHB_SliderPutar) == LOW)
    {
      EncSliderPutar.pulseCnt++;
    }
    else
    {
      EncSliderPutar.pulseCnt--;
    }
  }
}

void encSliderGeser_INTT_A()
{
  if (digitalRead(CHA_SliderGeser) == HIGH)
  {
    if (digitalRead(CHB_SliderGeser) == LOW)
    {
      EncSliderGeser.pulseCnt++;
      EncSlider.pulseCnt++;
    }
    else
    {
      EncSliderGeser.pulseCnt--;
      EncSlider.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(CHB_SliderGeser) == HIGH)
    {
      EncSliderGeser.pulseCnt++;
      EncSlider.pulseCnt++;
    }
    else
    {
      EncSliderGeser.pulseCnt--;
      EncSlider.pulseCnt--;
    }
  }
}

void encSliderGeser_INTT_B()
{
  if (digitalRead(CHA_SliderGeser) == HIGH)
  {
    if (digitalRead(CHB_SliderGeser) == HIGH)
    {
      EncSliderGeser.pulseCnt++;
      EncSlider.pulseCnt++;
    }
    else
    {
      EncSliderGeser.pulseCnt--;
      EncSlider.pulseCnt--;
    }
  }
  else
  {
    if (digitalRead(CHB_SliderGeser) == LOW)
    {
      EncSliderGeser.pulseCnt++;
      EncSlider.pulseCnt++;
    }
    else
    {
      EncSliderGeser.pulseCnt--;
      EncSlider.pulseCnt--;
    }
  }
}
