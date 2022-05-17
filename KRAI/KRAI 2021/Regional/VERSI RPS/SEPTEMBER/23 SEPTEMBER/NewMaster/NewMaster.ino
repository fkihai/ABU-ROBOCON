
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
#define ProxSmpng 11
#define ProxDpn   22

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
bool Berhenti = true;
bool Mulai = false;

/* Fungsi AmbilPanah () */
bool Auto = false;
long ltime1 = 0;
long ltime2 = 0;

/* Zone Full Auto */
bool Auto1 = false;
bool Auto2 = false;
bool Auto3 = false;
bool Auto4 = false;
bool Auto5 = false;
bool Auto6 = false;

bool GoTransfer = false;
bool GoToRack = false;
bool GoRun = false;
bool Putar = false;
bool CtrlRun = false;
bool Persiapan = false;


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
  pinMode(ProxSmpng, INPUT);
  pinMode(ProxDpn, INPUT);


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
  }
  if (SlaveSerial.available())
  {
    Slave.Data = SlaveSerial.read();
  }
}

void Odometry(void *pvParameters)
{
  for (;;)
  {
    EncCompute();
    //    Serial.print("X: ");
    //    Serial.print(Pos.X);
    //    Serial.print("\t");
    //    Serial.print("Y: ");
    //    Serial.println(Pos.Y);
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
    Bismillah();
    Joystik.Data = 'Z';
    Ctrl = false;
  }
}


void Bismillah()
{
  Auto1 = true;
  Zone1 = true;

  /** Local **/
  bool Gorack = false;
  bool AmbilPanah = false;
  bool Switch = false;

  while (Zone1) {
    vrps1 = 28;
    vrps2 = 28;
    vrps3 = 10;
    vrps4 = 10;

    /* AMBIL KE RACK GELOMBANG PERTAMA */
    while (Auto1) {
      if (abs(Pos.Y) < 60)
      {
        RPSKanan(-34, 0.01, 0.05);
        RPSKiri(-30, 0.01, 0.05);
      } else {
        SlaveSerial.print('L');
        Stop();
        delay(20);
        Gorack = true;
      }

      while (Gorack) {
        if (abs(Pos.X) < 180)
        {
          RPSDepan(-(vrps1), 0.01, 0.05);
          RPSBelakang(-(vrps2), 0.01, 0.05);
          RPSKanan(-(vrps3), 0.01, 0.05);
          RPSKiri(-(vrps4), 0.01, 0.05);
          SoftWheel(50, 175, 10, 10, abs(Pos.X), 5);  // Vrps 1 & 2
          SoftWheel1(50, 175, 10, 10, abs(Pos.X), 5); // Vrps 3 & 4
        }
        if (abs(Pos.X) > 180 || digitalRead(ProxSmpng) == LOW) {
          delay(50);
          Stop();
          vrps1 = 7;
          vrps2 = 7;
          vrps3 = 2;
          vrps4 = 2;
          AmbilPanah = true;
          Reset();
        }
        while (AmbilPanah) {
          RPSDepan(-(vrps1), 0.01, 0.05);
          RPSBelakang(-(vrps2), 0.01, 0.05);
          RPSKanan(-(vrps3), 0.07, 0.05);
          RPSKiri(-(vrps4), 0.07, 0.05);
          if (digitalRead(LimitSwitch) == LOW) {
            analogWrite(KA_Lpwm_PIN, 30 );
            analogWrite(KA_Rpwm_PIN, LOW);
            analogWrite(KI_Lpwm_PIN, 0);
            analogWrite(KI_Rpwm_PIN, LOW);
            analogWrite(DE_Lpwm_PIN, 70);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 90);
            analogWrite(BE_Rpwm_PIN, LOW);
            delay(500);
            SlaveSerial.print('M');
            AmbilPanah = false;
            Gorack = false;
            Auto1 = false;

            /** GoAuto 2 **/
            Auto2 = true;
            break;
          }
        }
      }
    }

    /* LEMPARAN GELOMBANG PERTAMA */
    while (Auto2) {
      LogData();
      if (Slave.Data == 'L')
      {
        Stop();
        delay(100);
        Reset();
        vrps1 = 15;
        vrps2 = 15;
        vrps3 = 8;
        vrps4 = 8;
        Go = true;
        Slave.Data = 'Z';
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
        }
        while (Pos.Y > 42 && !GoLmpr)
        {
          SlaveSerial.print('K');
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
          GoLmpr = true;
          Go = false;
          Slave.Data = 'A';
        }
      }
      while (GoLmpr)
      {
        LogData();
        if (Joystik.Data == 'P') {
          Mulai = true;
        }
        while (Mulai) {
          ThrowZone();
        }
      }
    }

    /* AMBIL KE RACK GELOMABANG KEDUA */
    while (Auto3) {
      LogData();
      if (Joystik.Data == 'P') {
        vrps1 = 8;
        vrps2 = 8;
        vrps3 = 10;
        vrps4 = 10;
        SlaveSerial.print('L');
        Joystik.Data = 'Z';
        Gorack = true;
      } while (Gorack) {
        RPSDepan(-(vrps1), 0.01, 0.05);
        RPSBelakang(-(vrps2), 0.01, 0.05);
        RPSKanan(-(vrps3), 0.01, 0.05);
        RPSKiri(-(vrps4), 0.01, 0.05);
        if (digitalRead(ProxSmpng) == LOW) {
          delay(150);
          Stop();
          vrps1 = 8;
          vrps2 = 8;
          vrps3 = 3;
          vrps4 = 3;
          AmbilPanah = true;
        } while (AmbilPanah) {
          RPSDepan(-(vrps1), 0.01, 0.05);
          RPSBelakang(-(vrps2), 0.01, 0.05);
          RPSKanan(-(vrps3), 0.09, 0.05);
          RPSKiri(-(vrps4), 0.09, 0.05);
          if (digitalRead(LimitSwitch) == LOW) {
            analogWrite(KA_Lpwm_PIN, 0);
            analogWrite(KA_Rpwm_PIN, LOW);
            analogWrite(KI_Lpwm_PIN, 50);
            analogWrite(KI_Rpwm_PIN, LOW);
            analogWrite(DE_Lpwm_PIN, 70);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 90);
            analogWrite(BE_Rpwm_PIN, LOW);
            delay(400);
            SlaveSerial.print('N');
            Reset();
            AmbilPanah = false;
            Gorack = false;
            Auto3 = false;
            Auto4 = true;
            break;
          }
        }
      }
    }

    /* LEMPARAN GELOMBANG KE DUA */
    while (Auto4) {
      LogData();
      if (Slave.Data == 'L')
      {
        vrps1 = 25;
        vrps2 = 25;
        vrps3 = 5;
        vrps4 = 5;
        Go = true;
        Slave.Data = 'Z';
      }
      while (Go) {
        if (Pos.Y < 35) {
          RPSKanan(vrps1, 0.01, 0.05);
          RPSKiri(vrps2, 0.01, 0.05);
          RPSDepan(-(vrps3), 0.01, 0.05);
          RPSBelakang(-(vrps4), 0.01, 0.05);
          SoftWheel(20, 60, 10, 10, Pos.Y, 4);  // Vrps 1 & 2
          SoftWheel1(20, 60, 10, 10, Pos.Y, 2); // Vrps 3 & 4
        } else if (Pos.Y > 35) {
          SlaveSerial.print('Q');
          Stop();
          delay(20);
          vrps1 = 6;
          vrps2 = 6;
          vrps3 = 6;
          vrps4 = 6;
          GoPos = true;
        } while (GoPos) {
          RPSDepan(vrps1, 0.01, 0.05);
          RPSBelakang(vrps2, 0.01, 0.05);
          RPSKanan(vrps3, 0.01, 0.05);
          RPSKiri(vrps4, 0.01, 0.05);
          SoftWheel(15, 60, 10, 10, Pos.X, 4); // Vrps 1 & 2
          SoftWheel1(15, 60, 10, 10, Pos.X, 6); // Vrps 3 & 4
          if (Pos.X > 26 && !GoLmpr ) {
            MoveStop();
            GoPos = false;
            Go = false;
            GoLmpr = true;
            Stop();
            Reset();
            Slave.Data = 'F';
            break;
          }
        }
      }
      while (GoLmpr)
      {
        LogData();
        if (Joystik.Data == 'P') {
          Mulai = true;
        }
        while (Mulai) {
          ThrowZone();
        }
      }
    }

    /* AMBIL KE RACK GELOMBANG KETIGA */
    while (Auto5) {
      LogData();
      if (Joystik.Data == 'P')
      {
        vrps1 = 8;
        vrps2 = 8;
        vrps3 = 10;
        vrps4 = 10;
        SlaveSerial.print('L');
        Joystik.Data = 'Z';
        Gorack = true;
      } while (Gorack) {
        RPSDepan(-(vrps1), 0.01, 0.05);
        RPSBelakang(-(vrps2), 0.01, 0.05);
        RPSKanan(-(vrps3), 0.01, 0.05);
        RPSKiri(-(vrps4), 0.01, 0.05);
        if (digitalRead(ProxSmpng) == LOW) {
          delay(50);
          Stop();
          vrps1 = 11;
          vrps2 = 10;
          vrps3 = 3;
          vrps4 = 3;
          AmbilPanah = true;
        } while (AmbilPanah) {
          RPSDepan(-(vrps1), 0.01, 0.05);
          RPSBelakang(-(vrps2), 0.01, 0.05);
          RPSKanan(-(vrps3), 0.09, 0.05);
          RPSKiri(-(vrps4), 0.09, 0.05);
          if (digitalRead(LimitSwitch) == LOW) {
            analogWrite(KA_Lpwm_PIN, 30 );
            analogWrite(KA_Rpwm_PIN, LOW);
            analogWrite(KI_Lpwm_PIN, 0);
            analogWrite(KI_Rpwm_PIN, LOW);
            analogWrite(DE_Lpwm_PIN, 70);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 90);
            analogWrite(BE_Rpwm_PIN, LOW);
            delay(700);
            SlaveSerial.print('O');
            Reset();
            AmbilPanah = false;
            Gorack = false;
            Auto5 = false;
            Auto6 = true;
            break;
          }
        }
      }
    }
    /** LEMPARAN GELOMBANG KETIGA **/
    while (Auto6) {
      LogData();
      if (Slave.Data == 'L') {
        vrps1 = 15;
        vrps2 = 15;
        vrps3 = 8;
        vrps4 = 8;
        delay(300);
        Go = true;
        Slave.Data = 'Z';
      } while (Go) {
        if (Pos.Y < 34) {
          RPSKanan(vrps1, 0.01, 0.05);
          RPSKiri(vrps2, 0.01, 0.05);
          RPSDepan(-(vrps3), 0.01, 0.05);
          RPSBelakang(-(vrps4), 0.01, 0.05);
          SoftWheel(20, 60, 10, 10, Pos.Y, 4);  // Vrps 1 & 2
          SoftWheel1(20, 60, 10, 10, Pos.Y, 2); // Vrps 3 & 4
        } else if (Pos.Y > 35) {
          SlaveSerial.print('Q');
          Stop();
          delay(20);
          vrps1 = 6;
          vrps2 = 6;
          vrps3 = 6;
          vrps4 = 6;
          while (true) {
            LogData();
            if (Joystik.Data == 'P') {
              GoPos = true;
              break;
            }
          }
          GoPos = true;
        } while (GoPos) {
          RPSDepan(vrps1, 0.01, 0.05);
          RPSBelakang(vrps2, 0.01, 0.05);
          RPSKanan(vrps3, 0.01, 0.05);
          RPSKiri(vrps4, 0.01, 0.05);
          SoftWheel(130, 190, 10, 10, Pos.X, 2);  // Vrps 1 & 2
          SoftWheel1(130, 150, 10, 10, Pos.X, 1); // Vrps 3 & 4
          if (digitalRead(ProxDpn) == HIGH && Pos.X > 140 ) { //178
            SlaveSerial.print('G');
            analogWrite(DE_Lpwm_PIN, LOW);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 20);
            analogWrite(BE_Rpwm_PIN, LOW);
            analogWrite(KA_Rpwm_PIN, 70);
            analogWrite(KA_Lpwm_PIN, LOW);
            analogWrite(KI_Rpwm_PIN, 100);
            analogWrite(KI_Lpwm_PIN, LOW);
            delay(700);
            analogWrite(DE_Lpwm_PIN, LOW);
            analogWrite(DE_Rpwm_PIN, LOW);
            analogWrite(BE_Lpwm_PIN, 15);
            analogWrite(BE_Rpwm_PIN, LOW);
            analogWrite(KA_Rpwm_PIN, 15);
            analogWrite(KA_Lpwm_PIN, LOW);
            analogWrite(KI_Rpwm_PIN, 30);
            analogWrite(KI_Lpwm_PIN, LOW);
            GoPos = false;
            Go = false;
            GoLmpr = true;
            Reset();
            Slave.Data = 'Z';
            break;
          }
        }
      }
      while (GoLmpr) {
        ThrowZone();
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

    /** Lemparan Gelombang 1 **/
    case 'A':
      analogWrite(KA_Rpwm_PIN, 25);
      analogWrite(KA_Lpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, 25);
      analogWrite(KI_Lpwm_PIN, LOW);
      analogWrite(DE_Lpwm_PIN, 25);
      analogWrite(DE_Rpwm_PIN, LOW);
      analogWrite(BE_Lpwm_PIN, 25);
      analogWrite(BE_Rpwm_PIN, LOW);
      delay(300);
      SlaveSerial.print('A');
      Reset();
      Slave.Data = 'Z';
      break;
    case 'B':
      Stop();
      Geser = true;
      Move = true;
      delay(30);
      SlaveSerial.print('B');
      MoveRigh(8, 2, 2, 2);
      Slave.Data = 'Z';
      break;
    case 'C':
      Stop();
      Geser = true;
      Move = true;
      delay(30);
      SlaveSerial.print('C');
      MoveRigh(18, 2, 2, 10);
      Slave.Data = 'Z';
      break;
    case 'D':
      Stop();
      Geser = true;
      Move = true;
      delay(30);
      SlaveSerial.print('D');
      MoveRigh(22, 4, 4, 16);
      Slave.Data = 'Z';
      break;
    case 'E':
      Stop(); 
      Geser = true;
      Move = true;
      delay(30);
      SlaveSerial.print('E');
      MoveRigh(27, 2, 2, 20);
      Slave.Data = 'Z';
      break;

    /* Control Untuk Auto Ke 3 */
    case 'N':
      Stop();
      Auto2 = false;
      GoLmpr = false;
      Mulai = false;
      Auto3 = true;
      Slave.Data = 'Z';
      break;
      break;
    /* Control Untuk Auto ke 5 */
    case 'M':
      Stop();
      Auto4 = false;
      GoLmpr = false;
      Mulai = false;
      Auto5 = true;
      Slave.Data = 'Z';
      break;

    /** Lemparan Gelombang 2 **/
    case 'F':
      analogWrite(KA_Rpwm_PIN, 15);
      analogWrite(KA_Lpwm_PIN, LOW);
      analogWrite(KI_Rpwm_PIN, 15);
      analogWrite(KI_Lpwm_PIN, LOW);
      delay(300);
      SlaveSerial.print('F');
      Reset();
      Slave.Data = 'Z';
      break;

    /** Lemparan Gelombang 3 **/
    case 'G':
      Reset();
      Slave.Data = 'Z';
      break;
    case 'H':
      Stop();
      Geser = true;
      Move = true;
      Reset();
      SlaveSerial.print('H');
      Slave.Data = 'Z';
      break;
    case 'I':
      Stop();
      Geser = true;
      Move = true;
      SlaveSerial.print('I');
      MoveLeft(13, 5, 5, 5);
      Slave.Data = 'Z';
      break;
    case 'J':
      Stop();
      Reset();
      Slave.Data = 'Z';
      break;
    default:
      break;
  }
}

void MoveLeft(float pos, int RPS1, int RPS2, float param) {
  vrps1 = RPS1;
  vrps2 = RPS1;
  vrps3 = RPS2;
  vrps4 = RPS2;
  while (Geser) {
    if (Move) {
      RPSDepan(-(vrps1), 0.1, 0.05);
      RPSBelakang(-(vrps2), 0.1, 0.05);
      RPSKanan(vrps3, 0.1, 0.05);
      RPSKiri(vrps4, 0.1, 0.05);
      SoftWheel(param , pos, 10, 10, abs(Pos.X), 2); // Vrps1 && vrps2
      SoftWheel1(param, pos, 10, 10, abs(Pos.X), 1); // vrps3 && vrps4
    }  if (abs(Pos.X) > pos - 1.0)    {
      Move = false;
      MoveStop();
      Geser = false;
    }
  }
}


void MoveRigh(float pos, int RPS1, int RPS2, float param) {
  vrps1 = RPS1;
  vrps2 = RPS1;
  vrps3 = RPS2;
  vrps4 = RPS2;
  while (Geser) {
    if (Move) {
      RPSDepan(vrps1, 0.1, 0.05);
      RPSBelakang(vrps2, 0.1, 0.05);
      RPSKanan(vrps3, 0.1, 0.05);
      RPSKiri(vrps4, 0.1, 0.05);
      SoftWheel(param , pos, 10, 10, abs(Pos.X), 2); // Vrps1 && vrps2
      SoftWheel1(param, pos, 10, 10, abs(Pos.X), 1); // vrps3 && vrps4
    }  if (abs(Pos.X) > pos - 1.0)    {
      Move = false;
      MoveStop();
      Geser = false;
    }
  }
}



void MoveStop()
{
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
  analogWrite(KA_Rpwm_PIN, 15);
  analogWrite(KA_Lpwm_PIN, LOW);
  analogWrite(KI_Rpwm_PIN, 15);
  analogWrite(KI_Lpwm_PIN, LOW);
}

void Stop()
{
  RPSKanan(0, 0, 0);
  RPSKiri(0, 0, 0);
  RPSDepan(0, 0, 0);
  RPSBelakang(0, 0, 0);
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
  Pos.Y = 0;
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
  serial.print("ProxSmpng: ");
  serial.print(digitalRead(ProxSmpng));
  serial.print("\t");
  serial.print("ProxDpn: ");
  serial.print(digitalRead(ProxDpn));
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
