/* SERIAL */
#define Gamepad      Serial3
#define serial       Serial
/* ENCODER */
#define encDepan_CHA 25
#define encDepan_CHB 23
#define encKiri_CHA 29
#define encKiri_CHB 27
#define encBelakang_CHA 31
#define encBelakang_CHB 33
#define encKanan_CHA 35
#define encKanan_CHB 37
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

struct COM
{
  char Data = 'Z';
} Slave, Joystik;

int vrps1 = 0, vrps2 = 0,
    vrps3 = 0, vrps4 = 0;


void pinConfig() {
  pinMode(encDepan_CHA, INPUT_PULLUP);
  pinMode(encDepan_CHB, INPUT_PULLUP);
  pinMode(encBelakang_CHA, INPUT_PULLUP);
  pinMode(encBelakang_CHB, INPUT_PULLUP);
  pinMode(encKanan_CHA, INPUT_PULLUP);
  pinMode(encKanan_CHB, INPUT_PULLUP);
  pinMode(encKiri_CHA, INPUT_PULLUP);
  pinMode(encKiri_CHB, INPUT_PULLUP);
  pinMode(BE_Rpwm_PIN, OUTPUT);
  pinMode(BE_Lpwm_PIN, OUTPUT);
  pinMode(KA_Rpwm_PIN, OUTPUT);
  pinMode(KA_Lpwm_PIN, OUTPUT);
  pinMode(KI_Rpwm_PIN, OUTPUT);
  pinMode(KI_Lpwm_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encDepan_CHA), encDepan_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encDepan_CHB), encDepan_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encBelakang_CHA), encBelakang_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encBelakang_CHB), encBelakang_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKanan_CHA), encKanan_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKanan_CHB), encKanan_INTT_B, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKiri_CHA), encKiri_INTT_A, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKiri_CHB), encKiri_INTT_B, CHANGE);
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
}
void LogData()
{
  if (Gamepad.available())
  {
    Joystik.Data = Gamepad.read();
  }
}

void CtrlManual() {
  LogData();
  if (Joystik.Data == 'A') {

  } if (Joystik.Data == 'B') {

  } if (Joystik.Data == 'C') {

  } if (Joystik.Data == 'D') {

  } if (Joystik.Data == 'E') {

  } if (Joystik.Data == 'F') {

  } if (Joystik.Data == 'G') {

  } if (Joystik.Data == 'H') {

  } if (Joystik.Data == 'I') {

  } if (Joystik.Data == 'Z') {

  }
}

void setup() {
  serial.begin(9600);
  Gamepad.begin(9600);
  pinConfig();
}

void loop() {

}
