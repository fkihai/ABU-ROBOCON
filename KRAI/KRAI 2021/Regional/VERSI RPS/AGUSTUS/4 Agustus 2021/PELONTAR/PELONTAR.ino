#define RADIUS_PL 2.9
#define KLL_PL 2 * 3.14 * RADIUS_PL
#define PPR_PL 1000

#define CHA_Pelontar 23
#define CHB_Pelontar 25
#define PL_RPWM_PIN 11
#define PL_LPWM_PIN 12
#define Brake A9

#define maju 1
#define mundur 2
#define berhenti 3

float Angle;
String SerialData;
String Data[10];
String SplitData;
int StringData;

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

bool Throw = false;
bool Down = false;

void setup() {
  Serial.begin(9600);
  pinMode(CHA_Pelontar, INPUT_PULLUP);
  pinMode(CHB_Pelontar, INPUT_PULLUP);
  pinMode(PL_RPWM_PIN, OUTPUT);
  pinMode(PL_LPWM_PIN, OUTPUT);
  pinMode(Brake, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(CHA_Pelontar), encPelontar_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB_Pelontar), encPelontar_INTT_B, CHANGE);
}

void loop() {
  trial();
  // TuningLempar();
}

void PlCompute()
{
  EncPelontar.Angle = (float(EncPelontar.pulseCnt) / PPR_PL) * 360;
}


void trial() {

  while (Serial.available()) {
    SerialData = Serial.readString();
    for (int a = 0; a < SerialData.length(); a++) {
      Data[a] = GetData(SerialData, ',', a);
      if (Data[a] != NULL) {
        //        Serial.print("Data ke ");
        //        Serial.print(a);
        //        Serial.print(" = ");
        //        Serial.println(Data[a]);
      }
    }
  }

  if (Data[0] == "a") {
    Throw = true;
  }  else if (Data[0] == "b") {
    Down = true;
  }

  if (Throw)
  {
    PlCompute();
    if (EncPelontar.Angle < Data[1].toInt()) {
      digitalWrite(Brake, HIGH);
      RPSpelontar(Data[2].toInt(), Data[3].toFloat(), Data[4].toFloat());
    }
    if (EncPelontar.Angle >= Data[1].toInt())
    {
      digitalWrite(Brake, LOW);
      RPSpelontar(0, 0, 0);
      digitalWrite(PL_LPWM_PIN, LOW);
      analogWrite(PL_RPWM_PIN, 0);
      delay(300);
      Data[0] = "c";
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
      delay(100);
      digitalWrite(PL_RPWM_PIN, LOW);
      analogWrite(PL_LPWM_PIN, LOW);
      ResetEnc();
      Data[0] = "D";
      Down = false;
    }
  }
}

void TuningLempar() {
  Angle = (float(EncPelontar.pulseCnt) / PPR_PL) * 360;

  while (Serial.available()) {
    SerialData = Serial.readString();
    for (int a = 0; a < SerialData.length(); a++) {
      Data[a] = GetData(SerialData, ',', a);
      if (Data[a] != NULL) {
        //        Serial.print("Data ke ");
        //        Serial.print(a);
        //        Serial.print(" = ");
        //        Serial.println(Data[a]);
      }
    }
  }

  if (Data[0] == "a" && Angle < Data[1].toInt()) {
    digitalWrite(Brake, HIGH);
    RPSpelontar(Data[2].toInt(), Data[3].toFloat(), Data[4].toFloat());
  } else if (Data[0] == "a" && Angle >= Data[1].toInt()) {
    digitalWrite(Brake, LOW);
    RPSpelontar(0, 0.1, 0.05);
    digitalWrite(PL_LPWM_PIN, LOW);
    analogWrite(PL_RPWM_PIN, 0);
  }

  /* Versi Satu */
  if (Data[0] == "b" && Angle > 5) {
    digitalWrite(Brake, HIGH);
    digitalWrite(PL_RPWM_PIN, LOW);
    analogWrite(PL_LPWM_PIN, 50);
  } else if (Data[0] == "b" && Angle < 5) {
    digitalWrite(PL_RPWM_PIN, LOW);
    analogWrite(PL_LPWM_PIN, LOW);
    digitalWrite(Brake, LOW);
    ResetEnc();
  }

  if (Data[0] == "c") {
    digitalWrite(Brake, HIGH);
  } else if (Data[0] == "d") {
    digitalWrite(Brake, LOW);
  }
}


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

void ResetEnc()
{
  EncPelontar.pulseCnt = 0;
  EncPelontar.Angle = 0.0;

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



/*
   NB : TONG KIRI DEKAT
   2. a,55,50,0.3,0.03
   3.
*/

/*
   NB : TONG KIRI JAUH
   4  a,51,51,0.39,0.03
   5.
*/
