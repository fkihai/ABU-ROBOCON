/*
   NB : TONG KIRI DEKAT
    1.a,53,45,0.32,0.03
    2.a,53,45,0.32,0.03
    3.a,53,44,0.32,0.03
*/

/*
   NB : TONG KIRI JAUH
   4  a,53,51,0.32,0.03
   5. a,53,51,0.32,0.03
*/

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
#define doorlock A10
#define Brake A9

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

struct Data
{
  String SerialData;
  String SplitData;
  String Data[100];
  int StringData;
} Slave;

struct serial
{
  char Data = '&';
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
bool Back = false;
bool stopSwitch = false;

/* TRIAL */
bool Uncal = false;
bool Uncal1 = false;
bool flag = false;

/* Auto */
bool Lmpr = false;
bool Ambil = false;
bool Ctrl = true;
bool Running = false;
bool ModeManual = false;
bool ModeAuto = true;

/* Trial Throwing */
bool Coba = false;
char Ctrlthrow = 'Z';

/* Var PWM */
int pwm = 0;
int pwm1 = 0;
int SwitchCtrl = 0;
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
  PinConfig();
  delay(100);
  ResetEnc();
  ModeManual = true;
}

void loop()
{
  ManualMode();
  ThrowZone1();

  /* Trial */
  // TuningLempar();
  // trialThrowing();
  // trialButton();
  // trialLempar();
  // trialRelay();
  // trialENC();
}

void ManualMode() {
  while (ModeManual) {
    LogJoystik();
    manual();
    if (Joystik.Data == 'X') {
      ModeManual = false;
      ModeAuto = true;
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

void ThrowZone1()
{
  LogData();
  switch (Master.Data) {

    /*** Type Lempar B ***/
    case 'A':
      digitalWrite(PW_LPWM_PIN, LOW);
      analogWrite(PW_RPWM_PIN, 0);
      LmprCptType1B(53, 44, 0.32, 0.03);
      CapitStop();
      Serial3.print('B');
      Master.Data = 'Z';
      break;
    case 'B':
      LmprCptType2B(53, 44, 0.32, 0.03);
      CapitStop();
      Serial3.print('C');
      Master.Data = 'Z';
      break;
    case 'C':
      LmprCptType3B(53, 44, 0.32, 0.03);
      CapitStop();
      Serial3.print('D');
      Master.Data = 'Z';
      break;
    case 'D':
      LmprCptType4B(55, 53, 0.33, 0.03);
      CapitStop();
      Master.Data = 'E';
      break;
    case 'E':
      LmprCptType5B(55, 53, 0.33, 0.03);
      CapitStop();
      Master.Data = 'Z';
      break;

    /*** Ambil Panah ***/
    case 'K':
      digitalWrite(PW_LPWM_PIN, LOW);
      analogWrite(PW_RPWM_PIN, 50);
      delay(300);
      digitalWrite(PW_LPWM_PIN, LOW);
      analogWrite(PW_RPWM_PIN, 30);
      break;
    case 'L':
      CapitBalikKiri();
      delay(100);
      Master.Data = 'Z';
      break;
    case 'M':
      ambilPanah();
      delay(200);
      Serial3.print('L');
      delay(200);
      Master.Data = 'Z';
      break;
    default :
      break;
  }
}

void ambilPanah()
{
  delay(400);
  PneumaticTutup();
  digitalWrite(PW_LPWM_PIN, LOW);
  analogWrite(PW_RPWM_PIN, 65);
  delay(100);
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
  analogWrite(PW_LPWM_PIN, 75);
  delay(1000);
  PneumaticBuka();
  delay(100);
}


void CapitStop()
{
  digitalWrite(PW_LPWM_PIN, LOW);
  analogWrite(PW_RPWM_PIN, LOW);
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
  analogWrite(PW_LPWM_PIN, 60);
  delay(Delay);
  digitalWrite(SliderB, LOW);
  digitalWrite(PW_RPWM_PIN, LOW);
  analogWrite(PW_LPWM_PIN, LOW);
  delay(200);
}

/******* Type Pelempar Zona 2 **************/

void LmprCptType1B(float SudutLntr, int RPS, float KP, float KD)
{
  Geser = true;
  Move = true;
  ResetEnc();
  while (Move)
  {
    if (Geser)
    {
      EncCompute();
      analogWrite(SG_LPWM_PIN, 40);
      digitalWrite(SG_RPWM_PIN, LOW);
      analogWrite(SP_LPWM_PIN, 200);
      digitalWrite(SP_RPWM_PIN, LOW);
      if (EncSliderPutar.Angle > 10)
      {
        analogWrite(SP_LPWM_PIN, 60);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(300);
        analogWrite(SP_LPWM_PIN, 27);
        digitalWrite(SP_RPWM_PIN, LOW);
        Transfer(Capit1A, Capit1B, 500);
        delay(500);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Throw = true;
        Geser = false;
      }
    }

    if (Throw)
    {
      PlCompute();
      if (EncPelontar.Angle < SudutLntr) {
        digitalWrite(Brake, HIGH);
        RPSpelontar(RPS, KP, KD);
      }
      if (EncPelontar.Angle >= SudutLntr)
      {
        digitalWrite(Brake, LOW);
        RPSpelontar(0, 0, 0);
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 0);
        delay(300);
        Down = true;
        Throw = false;
      }
    }

    if (Down)
    {
      EncCompute();
      PlCompute();
      CapitBalikKanan(50);
      delay(100);
      digitalWrite(Brake, HIGH);
      analogWrite(SP_LPWM_PIN, LOW);
      digitalWrite(SP_RPWM_PIN, LOW);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 50);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        digitalWrite(PW_RPWM_PIN, LOW);
        analogWrite(PW_LPWM_PIN, LOW);
        delay(150);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        digitalWrite(Brake, LOW);
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

void LmprCptType2B(float SudutLntr, int RPS, float KP, float KD)
{
  Geser = true;
  Move = true;
  ResetEnc();
  while (Move)
  {
    if (Geser)
    {
      EncCompute();
      analogWrite(SG_RPWM_PIN, 40);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (digitalRead(SwtchGeser) == LOW && Distance > 13)
      {
        Sensor = true;
      }
      if (Sensor)
      {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        Transfer(Capit2A, Capit2B, 500);
        delay(500);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Throw = true;
        Geser = false;
      }
    }

    if (Throw)
    {
      PlCompute();
      if (EncPelontar.Angle < SudutLntr) {
        digitalWrite(Brake, HIGH);
        RPSpelontar(RPS, KP, KD);
      }
      if (EncPelontar.Angle >= SudutLntr)
      {
        digitalWrite(Brake, LOW);
        RPSpelontar(0, 0, 0);
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 0);
        delay(300);
        Down = true;
        Throw = false;
      }
    }


    if (Down)
    {
      EncCompute();
      PlCompute();
      CapitBalikKanan(50);
      delay(100);
      digitalWrite(Brake, HIGH);
      analogWrite(SP_LPWM_PIN, LOW);
      digitalWrite(SP_RPWM_PIN, LOW);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 50);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        digitalWrite(PW_RPWM_PIN, LOW);
        analogWrite(PW_LPWM_PIN, LOW);
        delay(150);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        digitalWrite(Brake, LOW);
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

void LmprCptType3B(float SudutLntr, int RPS, float KP, float KD)
{
  pwm = 50;
  Geser = true;
  Move = true;
  ResetEnc();
  while (Move)
  {
    if (Geser)
    {
      EncCompute();
      analogWrite(SG_RPWM_PIN, pwm);
      digitalWrite(SG_LPWM_PIN, LOW);
      SoftSpeed(28, 20, Distance, 38);
      if (digitalRead(SwtchGeser) == LOW && Distance > 13)
      {
        Sensor = true;
      }
      if (Sensor)
      {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        Transfer(Capit3A, Capit3B, 400);
        delay(500);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Throw = true;
        Geser = false;
      }
    }


    if (Throw)
    {
      PlCompute();
      if (EncPelontar.Angle < SudutLntr) {
        digitalWrite(Brake, HIGH);
        RPSpelontar(RPS, KP, KD);
      }
      if (EncPelontar.Angle >= SudutLntr)
      {
        digitalWrite(Brake, LOW);
        RPSpelontar(0, 0, 0);
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 0);
        delay(300);
        Down = true;
        Throw = false;
      }
    }


    if (Down)
    {
      EncCompute();
      PlCompute();
      CapitBalikKanan(40);
      delay(100);
      digitalWrite(Brake, HIGH);
      analogWrite(SP_LPWM_PIN, LOW);
      digitalWrite(SP_RPWM_PIN, LOW);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 50);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        digitalWrite(PW_RPWM_PIN, LOW);
        analogWrite(PW_LPWM_PIN, LOW);
        delay(150);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        digitalWrite(Brake, LOW);
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

void LmprCptType4B(float SudutLntr, int RPS, float KP, float KD)
{
  pwm = 50;
  Geser = true;
  Move = true;
  Putar = false;
  ResetEnc();
  while (Move)
  {
    if (Geser)
    {
      EncCompute();
      analogWrite(SG_RPWM_PIN, pwm);
      digitalWrite(SG_LPWM_PIN, LOW);
      SoftSpeed(28, 20, Distance, 38);
      if (digitalRead(SwtchGeser) == LOW && Distance > 13)
      {
        Sensor = true;
      }
      if (Sensor)
      {
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        Transfer(Capit4A, Capit4B, 350);
        delay(500);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Sensor = false;
        Throw = true;
        Geser = false;
      }
    }

    if (Throw)
    {
      PlCompute();
      if (EncPelontar.Angle < SudutLntr) {
        digitalWrite(Brake, HIGH);
        RPSpelontar(RPS, KP, KD);
      }
      if (EncPelontar.Angle >= SudutLntr)
      {
        digitalWrite(Brake, LOW);
        RPSpelontar(0, 0, 0);
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 0);
        delay(300);
        Down = true;
        Throw = false;
      }
    }


    if (Down)
    {
      EncCompute();
      PlCompute();
      CapitBalikKanan(40);
      delay(100);
      digitalWrite(Brake, HIGH);
      analogWrite(SP_LPWM_PIN, LOW);
      digitalWrite(SP_RPWM_PIN, LOW);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 50);
      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
        digitalWrite(PW_RPWM_PIN, LOW);
        analogWrite(PW_LPWM_PIN, LOW);
        delay(150);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        digitalWrite(Brake, LOW);
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

void LmprCptType5B(float SudutLntr, int RPS, float KP, float KD)
{
  Move = true;
  Putar = true;
  ResetEnc();
  while (Move)
  {
    if (Putar) {
      digitalWrite(doorlock, HIGH);
      delay(100);
      ResetEnc();
      EncCompute();
      analogWrite(SP_RPWM_PIN, 200);
      digitalWrite(SP_LPWM_PIN, LOW);
      delay(100);
      Geser = true;
      Putar = false;
    }

    if (Geser)
    {
      EncCompute();
      analogWrite(SP_LPWM_PIN, 100);
      digitalWrite(SP_RPWM_PIN, LOW);
      analogWrite(SG_RPWM_PIN, 60);
      digitalWrite(SG_LPWM_PIN, LOW);
      if (EncSliderPutar.Angle > 22.8)
      {
        analogWrite(SP_LPWM_PIN, LOW);
        digitalWrite(SP_RPWM_PIN, LOW);
        analogWrite(SG_RPWM_PIN, LOW);
        digitalWrite(SG_LPWM_PIN, LOW);
        digitalWrite(doorlock, LOW);
        Transfer(Capit5A, Capit5B, 350);
        delay(100);
        ResetEnc();
        Persiapan = true;
        Geser = false;
      }
    }

    if (Persiapan)
    {
      EncCompute();
      digitalWrite(doorlock, HIGH);
      analogWrite(SP_RPWM_PIN, 100);
      digitalWrite(SP_LPWM_PIN, LOW);
      if (EncSliderPutar.Angle < -36)
      {
        digitalWrite(doorlock, LOW);
        analogWrite(SP_LPWM_PIN, 150);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(700);
        analogWrite(SP_LPWM_PIN, 40);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(500);
        analogWrite(SP_LPWM_PIN, 30);
        digitalWrite(SP_RPWM_PIN, LOW);
        delay(100);
        EncPelontar.pulseCnt = 0;
        EncPelontar.Angle = 0;
        Persiapan = false;
        Throw = true;
      }
    }

    if (Throw)
    {
      PlCompute();
      if (EncPelontar.Angle < SudutLntr) {
        digitalWrite(Brake, HIGH);
        RPSpelontar(RPS, KP, KD);
      }
      if (EncPelontar.Angle >= SudutLntr)
      {
        digitalWrite(Brake, LOW);
        RPSpelontar(0, 0, 0);
        digitalWrite(PL_LPWM_PIN, LOW);
        analogWrite(PL_RPWM_PIN, 0);
        delay(300);
        Down = true;
        Throw = false;
      }
    }

    if (Down)
    {
      PlCompute();
      digitalWrite(Brake, HIGH);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 40);
      analogWrite(SG_LPWM_PIN, 100);
      digitalWrite(SG_RPWM_PIN, LOW);

      if (EncPelontar.Angle < 5)
      {
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, 10);
      }
      if (digitalRead(SwtchReset) == LOW)
      {
        delay(100);
        digitalWrite(Brake, LOW);
        analogWrite(SG_RPWM_PIN, 0);
        digitalWrite(SG_LPWM_PIN, LOW);
        digitalWrite(PL_RPWM_PIN, LOW);
        analogWrite(PL_LPWM_PIN, LOW);
        analogWrite(SP_RPWM_PIN, LOW);
        digitalWrite(SP_LPWM_PIN, LOW);
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


void trialThrowing()
{
  if (Joystik.Data == 'F' or Joystik.Data == 'G')
  {
    Coba = true;
  }
  else if (Coba)
  {
    LmprCptType1B(48.0, 48, 0.38, 0.05);
    Ctrlthrow = 'A';
    Coba = false;
  }
  switch (Ctrlthrow)
  {
    case 'A':
      LmprCptType2B(48.0, 64, 0.38, 0.05);
      Ctrlthrow = 'B';
      break;
    case 'B':
      LmprCptType3B(48.0, 64, 0.38, 0.05);
      Ctrlthrow = 'C';
      break;
    case 'C':
      LmprCptType4B(48.0, 36, 0.38, 0.05);
      Ctrlthrow = 'D';
      break;
    case 'D':
      LmprCptType5B(48.0, 36, 0.38, 0.05);
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

void PneumaticTutup()
{
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
  EncPelontar.pulseCnt = 0;
  EncSliderGeser.pulseCnt = 0;
  EncSliderPutar.pulseCnt = 0;
  EncSlider.pulseCnt = 0;

  EncSliderPutar.Angle = 0.0;
  EncPelontar.Angle = 0.0;
  EncSlider.Angle = 0.0;
  Distance = 0.0;
}

/*********** MANUAL ***********/

void manual()
{
  /* SLIDER CAPIT  */
  if (Joystik.Data == 'H')
  {
    // MAJU (CROSS)
    // digitalWrite(SliderA, HIGH);
    // digitalWrite(SliderB, LOW);
    digitalWrite(Brake, HIGH);
    digitalWrite(doorlock, HIGH);
  }
  if (Joystik.Data == 'I')
  { // MUNDUR (CROSS)
    // digitalWrite(SliderA, LOW);
    // digitalWrite(SliderB, HIGH);
    digitalWrite(Brake, LOW);
    digitalWrite(doorlock, LOW);
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
    analogWrite(PW_RPWM_PIN, 45); // PUTAR MAJU
    digitalWrite(PW_LPWM_PIN, LOW);
  }
  if (Joystik.Data == 'S')
  {
    digitalWrite(PW_RPWM_PIN, LOW); // PUTAR MUNDUR
    analogWrite(PW_LPWM_PIN, 45);
  }

  /* ALL IN ONE */
  if (Joystik.Data == 'F')
  {
    PneumaticBuka(); // Circle)
  }
  if (Joystik.Data == 'G')
  {
    PneumaticTutup(); // (Circle)
  }

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
    Move = false;
    Geser = false;
    Persiapan = false;
    Throw = false;
    Down = false;
    Sensor = false;
    Putar = false;
    Back = false;
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
    digitalWrite(doorlock, HIGH);
  } else if (IR64 == "d") {
    digitalWrite(doorlock, LOW);
  }
}

void TuningLempar() {
  Angle = (float(EncPelontar.pulseCnt) / PPR_PL) * 360;
  while (Serial.available()) {
    SerialData = Serial.readString();
    for (int a = 0; a < SerialData.length(); a++) {
      Data[a] = GetData(SerialData, ',', a);
      if (Data[a] != NULL) {
        // Serial.print("Data ke ");
        // Serial.print(a);
        // Serial.print(" = ");
        // Serial.println(Data[a]);
      }
    }
  }

  if (Data[0] == "a") {
    EncPelontar.pulseCnt = 0;
    EncPelontar.Angle = 0.0;
    Throw = true;
    Data[0] = "c";
  }  else if (Data[0] == "b") {
    Down = true;
    Data[0] = "c";
  }

  if (Throw)
  {
    PlCompute();

    if (EncPelontar.Angle < Data[1].toFloat()) {
      digitalWrite(Brake, HIGH);
      RPSpelontar(Data[2].toInt(), Data[3].toFloat(), Data[4].toFloat());
    }
    if (EncPelontar.Angle >= Data[1].toFloat())
    {
      digitalWrite(Brake, LOW);
      RPSpelontar(0, 0, 0);
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, 0);
      delay(300);
      Throw = false;
    }
  }

  if (Down) {
    PlCompute();
    digitalWrite(Brake, HIGH);
    digitalWrite(PL_RPWM_PIN, LOW);
    analogWrite(PL_LPWM_PIN, 50);
    if (EncPelontar.Angle < 5)
    {
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, 10);
      delay(150);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, LOW);
      digitalWrite(Brake, LOW);
      RPSpelontar(0, 0, 0);
      EncPelontar.pulseCnt = 0;
      EncPelontar.Angle = 0;
      Data[0] = "D";
      Down = false;
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
  /* LEMPAR TIPE A*/
  if (Joystik.Data == 'J') // square
  {
    EncPelontar.Angle = 0;
    EncPelontar.pulseCnt = 0;
    Throw = true;
  }

  while (Throw)
  {
    PlCompute();
    if (EncPelontar.Angle < 55) {
      digitalWrite(Brake, HIGH);
      RPSpelontar(50, 0.3, 0.03);
    }
    if (EncPelontar.Angle >= 55)
    {
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

  if (Down) {
    PlCompute();
    digitalWrite(Brake, HIGH);
    digitalWrite(PL_RPWM_PIN, LOW);
    analogWrite(PL_LPWM_PIN, 50);
    if (EncPelontar.Angle < 5)
    {
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, LOW);
      digitalWrite(Brake, LOW);
      ResetEnc();
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
  pinMode(doorlock, OUTPUT);
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
  digitalWrite(doorlock, LOW);
  digitalWrite(Brake, LOW);

  digitalWrite(Capit1A, HIGH);
  digitalWrite(Capit2A, HIGH);
  digitalWrite(Capit3A, HIGH);
  digitalWrite(Capit4A, HIGH);
  digitalWrite(Capit5A, HIGH);
  digitalWrite(SliderB, HIGH);
  delay(100);
  digitalWrite(Capit1A, LOW);
  digitalWrite(Capit2A, LOW);
  digitalWrite(Capit3A, LOW);
  digitalWrite(Capit4A, LOW);
  digitalWrite(Capit5A, LOW);
  digitalWrite(SliderB, LOW);

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

void SoftSpeed1(float ParamA, float a, int dist, int Speed)
{
  if ((millis() - ltime1) > 20 && dist > ParamA)
  {
    ltime1 = millis();
    pwm1 -= a;
    if (pwm1 < Speed)
    {
      pwm1 = Speed;
    }
  }
}

/*********** RPS MOTOR ***********/

void RPSpelontar(int RPS, float kp, float kd)
{
  float
  Kp = kp,
  Kd = kd, TS = 1.0;
  float P, I, D, error, lError = 0;
  int cRPS;
  PL.RPS = RPS;

  if ((millis() - PL.Time) > 10)
  {
    cRPS = abs(EncPelontar.pulseCnt) - abs(EncPelontar.lastpulse);

    if (PL.RPS > 0)
    {
      MotorPelontar(maju);
    }
    else if (PL.RPS < 0)
    {
      MotorPelontar(mundur);
    }
    else
    {
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
