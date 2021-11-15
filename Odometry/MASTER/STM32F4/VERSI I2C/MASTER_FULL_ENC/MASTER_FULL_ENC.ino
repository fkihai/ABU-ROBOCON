#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27, 16, 2);

SoftwareSerial cmpSerial    (PD6, PD5);
#define serial SerialUSB
#define ADDR    64

/*COMMAND*/
#define AT_RESET    (byte(0x00))
#define AT_RPS      (byte(0x01))

/* Direction Slave */
#define CW          (byte(0x06))
#define CCW         (byte(0x04))

/* ENCODER */
#define encBelakang_CHA PA2
#define encBelakang_CHB PA0
#define encKanan_CHA    PA4
#define encKanan_CHB    PA6
#define encKiri_CHA     PB14
#define encKiri_CHB     PD8

#define Button1 PE10
#define Button2 PE12
#define Button3 PE14

/* ODOMETRY */
#define MATH_PI           3.14286
#define MATH_ROOT_OF_3    1.73205
#define WHEEL_RADIUS      2.9
#define WHEEL_BASE        9.40
#define ENC_RESOLUTION    2400.0
#define MIN_POS_RES       3.0
#define MIN_DEG_RES       5.0

/* Expansi Macro */
#define POS_CONV_FACTOR   ((2 * MATH_PI) / ENC_RESOLUTION)
#define MATH_SIN(x)       sin(radians(x))
#define MATH_COS(x)       cos(radians(x))

/* Fuzzy */
// omega
#define AVLOW_SPEED      10
#define ALOW_SPEED       20
#define AMEDIUM_SPEED    30
#define AFAST_SPEED      50
#define AVFAST_SPEED     75

// Linear
#define LVLOW_SPEED      20
#define LLOW_SPEED       40
#define LMEDIUM_SPEED    50
#define LFAST_SPEED      60
#define LVFAST_SPEED     90

/* MOVE GOAL TO GOAL */
int Count   = 0;
bool Move = false;
/* MOVE GOAL TO GOAL */

/* KOORDINAT X, Y, DAN HEADING */
struct POS {
  float X, Y, T;
} currentPOS, lastPOS;

/* PENYIMPANAN PULSA ENCODER */
struct ENC {
  volatile long int pulseCnt;
  long int lastpulse;
} EncBelakang, EncKanan, EncKiri;

/*COMPAS GY 25*/
struct cmps {
  char  buffer[50];
  int   counter;
  float heading;
} cmps;

/* Fungsi Compas */
void resetCMPS() {
  // kalibrasi Heading
  cmpSerial.write(0xA5);
  cmpSerial.write(0x55);
  delay(100);
  cmpSerial.write(0xA5);
  cmpSerial.write(0x53);
}

void initCMPS() {
  // Kalibrasi tilt
  cmpSerial.write(0xA5);
  cmpSerial.write(0x54);
  delay(1000);
  resetCMPS();
}

void updateCMPS() {
  char tmp;
  while (cmpSerial.available()) {
    tmp = cmpSerial.read();
    cmps.buffer[cmps.counter++] = tmp;
    if (tmp == '\n') {
      cmps.buffer[cmps.counter] = 0;
      cmps.heading = atof(strtok(cmps.buffer + 5, ","));
      cmps.counter = 0;
    }
  }
}

void setup() {
  lcd.begin();
  Wire.begin();
  cmpSerial.begin(115200);
  serial.begin(9600);
  pinMode(Button1 , INPUT_PULLUP);
  pinMode(Button2 , INPUT_PULLUP);
  pinMode(Button3 , INPUT_PULLUP);
  pinMode(encBelakang_CHA , INPUT_PULLUP);
  pinMode(encBelakang_CHB , INPUT_PULLUP);
  pinMode(encKanan_CHA    , INPUT_PULLUP);
  pinMode(encKanan_CHB    , INPUT_PULLUP);
  pinMode(encKiri_CHA     , INPUT_PULLUP);
  pinMode(encKiri_CHB     , INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encBelakang_CHA), encBelakang_INTT_A,  CHANGE);
  attachInterrupt(digitalPinToInterrupt(encBelakang_CHB), encBelakang_INTT_B,  CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKanan_CHA),    encKanan_INTT_A,     CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKanan_CHB),    encKanan_INTT_B,     CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKiri_CHA),     encKiri_INTT_A,      CHANGE);
  attachInterrupt(digitalPinToInterrupt(encKiri_CHB),     encKiri_INTT_B,      CHANGE);
  delay(1000);

  /* RESET */
  fullResetENC();
  initCMPS();
  initOdometry();

  /* Fungsi Utama */
  controlCompute();

  /* Fungsi Trial */
  // trialEncMaster();
  // TrialButton();

}

void loop() {

}

void trialEncMaster() {
  while (1) {
    updateOdometry();
    serial.print("BE: ");
    serial.print(EncBelakang.pulseCnt);
    serial.print("\tKA: ");
    serial.print(EncKanan.pulseCnt);
    serial.print("\tKI: ");
    serial.print(EncKiri.pulseCnt);
    serial.print("\tX: ");
    serial.print(currentPOS.X);
    serial.print("\tY: ");
    serial.print(currentPOS.Y);
    serial.print("\tT: ");
    serial.println(currentPOS.T);
    delay(100);
  }
}

void TrialButton() {
  while (1) {
    serial.print("B1: ");
    serial.print(digitalRead(Button1));
    serial.print("\t");
    serial.print("B2: ");
    serial.print(digitalRead(Button2));
    serial.print("\t");
    serial.print("B3: ");
    serial.println(digitalRead(Button3));
  }
}

/* Odometry */
void initOdometry(void) {
  currentPOS.X       = lastPOS.X = currentPOS.Y = lastPOS.Y = currentPOS.T = 0;
  EncKanan.pulseCnt  = EncBelakang.pulseCnt = EncKiri.pulseCnt    = 0;
  EncKanan.lastpulse = EncBelakang.lastpulse = EncKiri.lastpulse = 0;
}

void fullResetENC(void) {
  initOdometry();
  Wire.beginTransmission(ADDR);
  Wire.write(AT_RESET);
  Wire.endTransmission();
}

void updateOdometry() {
  updateCMPS();
  float _x, _y, _t;
  // kecepatan rotasi posisi dan heading
  float rvPos1, rvPos2, rvPos3;

  long int
  // delta pulse
  dKiriENC, dKananENC, dBelakangENC,
            // pulsa sekarang
            currentBePC, currentKaPC, currentKiPC;

  currentBePC = EncBelakang.pulseCnt;
  currentKaPC = EncKanan.pulseCnt;
  currentKiPC = EncKiri.pulseCnt;

  dBelakangENC = currentBePC - EncBelakang.lastpulse;
  dKananENC    = currentKaPC - EncKanan.lastpulse;
  dKiriENC     = currentKiPC - EncKiri.lastpulse;

  /* POS ENC X AND Y */
  rvPos1 = dBelakangENC * POS_CONV_FACTOR;
  rvPos2 = dKananENC    * POS_CONV_FACTOR;
  rvPos3 = dKiriENC     * POS_CONV_FACTOR;

  _x = WHEEL_RADIUS * ((2 / 3.0 * rvPos1) - (1 / 3.0 * rvPos2) - (1 / 3.0 * rvPos3));
  _y = WHEEL_RADIUS * ((1 / MATH_ROOT_OF_3 * rvPos2) - (1 / MATH_ROOT_OF_3 * rvPos3));
  _t = cmps.heading ;

  currentPOS.X = lastPOS.X + ((MATH_COS(_t) * _x ) - (MATH_SIN(_t) * _y));
  currentPOS.Y = lastPOS.Y + ((MATH_SIN(_t) * _x ) + (MATH_COS(_t) * _y));
  currentPOS.T = _t;

  EncBelakang.lastpulse  = currentBePC;
  EncKanan.lastpulse     = currentKaPC;
  EncKiri.lastpulse      = currentKiPC;

  lastPOS.X = currentPOS.X;
  lastPOS.Y = currentPOS.Y;
}

bool goXYT(int x, int y, int t) {
  updateOdometry();
  float
  s,      // Jarak
  v,      // Linear Velocity
  w,      // angular velocity
  Vx,     // Kecepatan arah X
  Vy;     // Kecepatan arah Y

  float
  alpha,  // heading Error Robot
  TB,     // Target Bearing
  dx,     // Delta x
  dy,     // Delta y
  w1,     // Motor Belakang
  w2,     // Motor Kanan
  w3,     // Motor Kiri
  v1,     // Motor Belakang
  v2,     // Motor Kanan
  v3,     // Motor Kiri
  RPM1,   // Motor Belakang
  RPM2,   // Motor Kanan
  RPM3,   // Motor Kiri
  dw;     // Delta Omega

  v1 = v2 = v3 = 0;
  w1 = w2 = w3 = 0;
  v  = 0;
  w  = 0;

  // Delta Position
  dx = abs(currentPOS.X - x);
  dy = abs(currentPOS.Y - y);

  // Target Bearing
  TB    = degrees(atan2((y - currentPOS.Y), (x - currentPOS.X)));

  // heading Error Robot
  alpha = TB - currentPOS.T;

  s  = sqrt (pow(dx, 2) + pow(dy, 2));
  dw = t - currentPOS.T;

  if (s <= MIN_POS_RES) {
    v = 0;
    if (s <= MIN_POS_RES &&  abs(dw) <= MIN_DEG_RES && Move == true) {
      Count++;
      if (Count == 2) {
        setRPM(0, 0, 0);
      }
    }
    if (s <= MIN_POS_RES &&  abs(dw) <= MIN_DEG_RES && Move == false) {
      Count = 0;
      setRPM(0, 0, 0);
    }
  }

  if (s > MIN_POS_RES) {
    v = FuzzyLinear(s);
  }

  if (abs(dw) > MIN_DEG_RES) {
    w = FuzzyOmega(abs(dw));
    if (dw < 0) {
      w *= -1;
    }
  }

  // Hitung kecepatan arah sumbu X dan Y (Vektor)
  Vx = v * cos(radians(alpha));
  Vy = v * sin(radians(alpha));

  // angular Motor [rad/s]
  w1 = (0.67  * Vx)  + (0     * Vy) + (0.33 * w);
  w2 = (-0.33 * Vx)  + (0.58  * Vy) + (0.33 * w);
  w3 = (-0.33 * Vx)  + (-0.58 * Vy) + (0.33 * w);

  // Linear Motor [m/s];
  v1 = float(0.029 * w1);
  v2 = float(0.029 * w2);
  v3 = float(0.029 * w3);

  // convert To RPM
  RPM1 = (60 * v1) / (2 * MATH_PI * 0.075); // 0.075 is radius motor
  RPM2 = (60 * v2) / (2 * MATH_PI * 0.075);
  RPM3 = (60 * v3) / (2 * MATH_PI * 0.075);

  setRPM(RPM1, RPM2, RPM3);

  return false;
}

void setRPM(int beRPM, int kaRPM, int kiRPM) {
  int beCtrl, kaCtrl, kiCtrl;

  if (beRPM < 0) {
    beCtrl = CCW;
  } else {
    beCtrl = CW;
  } if (kaRPM < 0) {
    kaCtrl = CCW;
  } else {
    kaCtrl = CW;
  } if (kiRPM < 0) {
    kiCtrl = CCW;
  } else {
    kiCtrl = CW;
  }

  Wire.beginTransmission(ADDR);
  Wire.write(AT_RPS);
  Wire.write((byte)map(abs(beRPM), 0, 400, 0, 255));
  Wire.write((byte)map(abs(kaRPM), 0, 400, 0, 255));
  Wire.write((byte)map(abs(kiRPM), 0, 400, 0, 255));
  Wire.write(beCtrl);
  Wire.write(kaCtrl);
  Wire.write(kiCtrl);
  Wire.endTransmission();
}

void encBelakang_INTT_A() {
  if (digitalRead(encBelakang_CHA) == HIGH) {
    if (digitalRead(encBelakang_CHB) == LOW) {
      EncBelakang.pulseCnt++;
    } else {
      EncBelakang.pulseCnt--;
    }
  }
  else {
    if (digitalRead(encBelakang_CHB) == HIGH) {
      EncBelakang.pulseCnt++;
    } else {
      EncBelakang.pulseCnt--;
    }
  }
}
void encBelakang_INTT_B() {
  if (digitalRead(encBelakang_CHA) == HIGH) {
    if (digitalRead(encBelakang_CHB) == HIGH) {
      EncBelakang.pulseCnt++;
    } else {
      EncBelakang.pulseCnt--;
    }
  }
  else {
    if (digitalRead(encBelakang_CHB) == LOW) {
      EncBelakang.pulseCnt++;
    } else {
      EncBelakang.pulseCnt--;
    }
  }
}
void encKanan_INTT_A() {
  if (digitalRead(encKanan_CHA) == HIGH) {
    if (digitalRead(encKanan_CHB) == LOW) {
      EncKanan.pulseCnt++;
    } else {
      EncKanan.pulseCnt--;
    }
  }
  else {
    if (digitalRead(encKanan_CHB) == HIGH) {
      EncKanan.pulseCnt++;
    } else {
      EncKanan.pulseCnt--;
    }
  }
}
void encKanan_INTT_B() {
  if (digitalRead(encKanan_CHA) == HIGH) {
    if (digitalRead(encKanan_CHB) == HIGH) {
      EncKanan.pulseCnt++;
    } else {
      EncKanan.pulseCnt--;
    }
  }
  else {
    if (digitalRead(encKanan_CHB) == LOW) {
      EncKanan.pulseCnt++;
    } else {
      EncKanan.pulseCnt--;
    }
  }
}
void encKiri_INTT_A() {
  if (digitalRead(encKiri_CHA) == HIGH) {
    if (digitalRead(encKiri_CHB) == LOW) {
      EncKiri.pulseCnt++;
    } else {
      EncKiri.pulseCnt--;
    }
  }
  else {
    if (digitalRead(encKiri_CHB) == HIGH) {
      EncKiri.pulseCnt++;
    } else {
      EncKiri.pulseCnt--;
    }
  }
}
void encKiri_INTT_B() {
  if (digitalRead(encKiri_CHA) == HIGH) {
    if (digitalRead(encKiri_CHB) == HIGH) {
      EncKiri.pulseCnt++;
    } else {
      EncKiri.pulseCnt--;
    }
  }
  else {
    if (digitalRead(encKiri_CHB) == LOW) {
      EncKiri.pulseCnt++;
    } else {
      EncKiri.pulseCnt--;
    }
  }
}



float FuzzyOmega(float wSpeed) {
  float Omega [6] = {0, 10, 75, 130, 180, 230};
  float Speed[5];
  float output = 0;

  /* MEMBER FUNCTION */
  // Very Near
  if (wSpeed >  Omega[0] && wSpeed <= Omega[1]) {
    // Speed[0] = (Omega[1] - wSpeed) / (Omega[1] - Omega[0]);
    Speed[0] = 1;
  } else if (wSpeed > Omega[1] && wSpeed <= Omega[2]) {
    Speed[0] = (Omega[2] - wSpeed) / (Omega[2] - Omega[1]);
  } else {
    Speed[0] = 0;
  }
  // Near
  if (wSpeed <=  Omega[1]) {
    Speed[1] = 0;
  } else if (wSpeed > Omega[1] && wSpeed <= Omega[2]) {
    Speed[1] = (wSpeed - Omega[1]) / (Omega[2] - Omega[1]);
  } else if (wSpeed > Omega[2] && wSpeed <= Omega[3]) {
    Speed[1] = (Omega[3] - wSpeed) / (Omega[3] - Omega[2]);
  } else {
    Speed[1] = 0;
  }
  // Midle
  if (wSpeed <= Omega[2]) {
    Speed[2] = 0;
  } else if (wSpeed > Omega[2] && wSpeed <= Omega[3]) {
    Speed[2] = (wSpeed - Omega[2]) / (Omega[3] - Omega[2]);
  } else if (wSpeed > Omega[3] && wSpeed <= Omega[4]) {
    Speed[2] = (Omega[4] - wSpeed) / (Omega[4] - Omega[3]);
  } else {
    Speed[2] = 0;
  }
  // Far
  if (wSpeed <= Omega[3]) {
    Speed[3] = 0;
  } else if (wSpeed > Omega[3] && wSpeed <= Omega[4]) {
    Speed[3] = (wSpeed - Omega[3]) / (Omega[4] - Omega[3]);
  } else if (wSpeed > Omega[4] && wSpeed <= Omega[5]) {
    Speed[3] = (Omega[5] - wSpeed) / (Omega[5] - Omega[4]);
  } else {
    Speed[3] = 0;
  }
  // Very Far
  if (wSpeed <= Omega[4]) {
    Speed[4] = 0;
  } else if (wSpeed > Omega[4] && wSpeed <= Omega[5]) {
    Speed[4] = (wSpeed - Omega[4]) / (Omega[5] - Omega[4]);
  } else {
    Speed[4] = 1;
  }

  /* RULE */
  output = ((Speed[0] * AVLOW_SPEED) + (Speed[1] * ALOW_SPEED) + (Speed[2] * AMEDIUM_SPEED) + (Speed[3] * AFAST_SPEED) + (Speed[4] * AVFAST_SPEED)) /
           (Speed[0] + Speed[1] + Speed[2] + Speed[3] + Speed[4] );
  return output;
}

float FuzzyLinear(float vSpeed) {
  float output = 0;
  float fSpeed[6] = {0, 35, 80, 130, 180, 230};
  float Speed[5];

  /* MEMBER FUNCTION */
  // Very Near
  if (vSpeed >  fSpeed[0] && vSpeed <= fSpeed[1]) {
    // Speed[0] = (fSpeed[1] - vSpeed) / (fSpeed[1] - fSpeed[0]);
    Speed[0] = 1;
  } else if (vSpeed > fSpeed[1] && vSpeed <= fSpeed[2]) {
    Speed[0] = (fSpeed[2] - vSpeed) / (fSpeed[2] - fSpeed[1]);
  } else {
    Speed[0] = 0;
  }
  // Near
  if (vSpeed <=  fSpeed[1]) {
    Speed[1] = 0;
  } else if (vSpeed > fSpeed[1] && vSpeed <= fSpeed[2]) {
    Speed[1] = (vSpeed - fSpeed[1]) / (fSpeed[2] - fSpeed[1]);
  } else if (vSpeed > fSpeed[2] && vSpeed <= fSpeed[3]) {
    Speed[1] = (fSpeed[3] - vSpeed) / (fSpeed[3] - fSpeed[2]);
  } else {
    Speed[1] = 0;
  }
  // Midle
  if (vSpeed <= fSpeed[2]) {
    Speed[2] = 0;
  } else if (vSpeed > fSpeed[2] && vSpeed <= fSpeed[3]) {
    Speed[2] = (vSpeed - fSpeed[2]) / (fSpeed[3] - fSpeed[2]);
  } else if (vSpeed > fSpeed[3] && vSpeed <= fSpeed[4]) {
    Speed[2] = (fSpeed[4] - vSpeed) / (fSpeed[4] - fSpeed[3]);
  } else {
    Speed[2] = 0;
  }
  // Far
  if (vSpeed <= fSpeed[3]) {
    Speed[3] = 0;
  } else if (vSpeed > fSpeed[3] && vSpeed <= fSpeed[4]) {
    Speed[3] = (vSpeed - fSpeed[3]) / (fSpeed[4] - fSpeed[3]);
  } else if (vSpeed > fSpeed[4] && vSpeed <= fSpeed[5]) {
    Speed[3] = (fSpeed[5] - vSpeed) / (fSpeed[5] - fSpeed[4]);
  } else {
    Speed[3] = 0;
  }
  // Very Far
  if (vSpeed <= fSpeed[4]) {
    Speed[4] = 0;
  } else if (vSpeed > fSpeed[4] && vSpeed <= fSpeed[5]) {
    Speed[4] = (vSpeed - fSpeed[4]) / (fSpeed[5] - fSpeed[4]);
  } else {
    Speed[4] = 1;
  }
  /* RULE */
  output = ((Speed[0] * LVLOW_SPEED) + (Speed[1] * LLOW_SPEED) + (Speed[2] * LMEDIUM_SPEED) + (Speed[3] * LFAST_SPEED) + (Speed[4] * LVFAST_SPEED)) /
           (Speed[0] + Speed[1] + Speed[2] + Speed[3] + Speed[4] );
  return output;
}


void controlCompute() {
  char IR64;
  long previousmillis = 0;
  bool LCD      = true;
  bool MoveGoal = false;
  bool writeODM = false;

  struct data {
    int16_t
    p1,
    p2,
    p3;
  } targetXYT;

  // Move goal To goal
  int ZoneArr[100][3] {
    { 0, 150, 0},
    { 150, 150, 0},
    { 0, 0, 0},
  };

  while (1) {
    updateOdometry();
    if (writeODM) {
      goXYT(targetXYT.p1, targetXYT.p2, targetXYT.p3);
    }
    if (MoveGoal) {
      switch (Count) {
        case 0:
          goXYT(ZoneArr[0][0], ZoneArr[0][1], ZoneArr[0][2]);
          break;
        case 1:
          goXYT(ZoneArr[1][0], ZoneArr[1][1], ZoneArr[1][2]);
          break;
        case 2:
          goXYT(ZoneArr[2][0], ZoneArr[2][1], ZoneArr[2][2]);
          break;
        default:
          setRPM(0, 0, 0);
          Count = 2; // Last Count
          break;
      }
    }

    if (digitalRead(Button1) == LOW) {
      writeODM = true;
      MoveGoal = false;
      Move = false;
      targetXYT.p1 = 0; 
      targetXYT.p2 = 0;
      targetXYT.p3 = 10;
    }
    if (digitalRead(Button2) == LOW) {
      writeODM = true;
      MoveGoal = false;
      Move = false;
      targetXYT.p1 = 0;
      targetXYT.p2 = 0;
      targetXYT.p3 = 0;
    }
    if (digitalRead(Button3) == LOW) {
      writeODM = false;
      MoveGoal = true;
      Move = true;
    }
    //    if ((millis() - previousmillis) > 100) {
    //      previousmillis = millis();
    //      serial.print( currentPOS.X);
    //      serial.print("\t");
    //      serial.print( currentPOS.Y);
    //      serial.print("\t");
    //      serial.print( currentPOS.T);
    //      serial.print("\t");
    //      serial.println(Count);
    //    }
  }
}
