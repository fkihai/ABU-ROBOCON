/* FUZZY MEMBER */
#define AVLOW_SPEED   10.0
#define ALOW_SPEED    20.0
#define AMEDIUM_SPEED 30.0
#define AFAST_SPEED   50.0
#define AVFAST_SPEED  75.0

#define LVLOW_SPEED   20.0
#define LLOW_SPEED    40.0
#define LMEDIUM_SPEED 50.0
#define LFAST_SPEED   60.0
#define LVFAST_SPEED  90.0

float Array[5];

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Linerar: ");
  Serial.print(FuzzyLinear(76));
  Serial.print('\t');
  Serial.print("Angular: ");
  Serial.println(FuzzyOmega(98));
  }
float FuzzyOmega(float wSpeed)
{
  float Omega[6] = {0, 40, 110, 180, 250, 320};
  float Speed[5];
  float output = 0;

  // Very Near
  if (wSpeed > Omega[0] && wSpeed <= Omega[1])
  {
    Speed[0] = 1;
  }
  else if (wSpeed > Omega[1] && wSpeed <= Omega[2])
  {
    Speed[0] = (Omega[2] - wSpeed) / (Omega[2] - Omega[1]);
  }
  else
  {
    Speed[0] = 0;
  }
  // Near
  if (wSpeed <= Omega[1])
  {
    Speed[1] = 0;
  }
  else if (wSpeed > Omega[1] && wSpeed <= Omega[2])
  {
    Speed[1] = (wSpeed - Omega[1]) / (Omega[2] - Omega[1]);
  }
  else if (wSpeed > Omega[2] && wSpeed <= Omega[3])
  {
    Speed[1] = (Omega[3] - wSpeed) / (Omega[3] - Omega[2]);
  }
  else
  {
    Speed[1] = 0;
  }
  // Midle
  if (wSpeed <= Omega[2])
  {
    Speed[2] = 0;
  }
  else if (wSpeed > Omega[2] && wSpeed <= Omega[3])
  {
    Speed[2] = (wSpeed - Omega[2]) / (Omega[3] - Omega[2]);
  }
  else if (wSpeed > Omega[3] && wSpeed <= Omega[4])
  {
    Speed[2] = (Omega[4] - wSpeed) / (Omega[4] - Omega[3]);
  }
  else
  {
    Speed[2] = 0;
  }
  // Far
  if (wSpeed <= Omega[3])
  {
    Speed[3] = 0;
  }
  else if (wSpeed > Omega[3] && wSpeed <= Omega[4])
  {
    Speed[3] = (wSpeed - Omega[3]) / (Omega[4] - Omega[3]);
  }
  else if (wSpeed > Omega[4] && wSpeed <= Omega[5])
  {
    Speed[3] = (Omega[5] - wSpeed) / (Omega[5] - Omega[4]);
  }
  else
  {
    Speed[3] = 0;
  }
  // Very Far
  if (wSpeed <= Omega[4])
  {
    Speed[4] = 0;
  }
  else if (wSpeed > Omega[4] && wSpeed <= Omega[5])
  {
    Speed[4] = (wSpeed - Omega[4]) / (Omega[5] - Omega[4]);
  }
  else
  {
    Speed[4] = 1;
  }
  output = ((Speed[0] * AVLOW_SPEED) + (Speed[1] * ALOW_SPEED) + (Speed[2] * AMEDIUM_SPEED) + (Speed[3] * AFAST_SPEED) + (Speed[4] * AVFAST_SPEED)) /
           (Speed[0] + Speed[1] + Speed[2] + Speed[3] + Speed[4]);

  return output;
}

float FuzzyLinear(float vSpeed)
{
  float output = 0;
  float fSpeed[6] = {0, 35, 80, 130, 180, 230};
  float Speed[5];

  // Very Near
  if (vSpeed > fSpeed[0] && vSpeed <= fSpeed[1])
  {
    Speed[0] = 1;
  }
  else if (vSpeed > fSpeed[1] && vSpeed <= fSpeed[2])
  {
    Speed[0] = (fSpeed[2] - vSpeed) / (fSpeed[2] - fSpeed[1]);
  }
  else
  {
    Speed[0] = 0;
  }
  // Near
  if (vSpeed <= fSpeed[1])
  {
    Speed[1] = 0;
  }
  else if (vSpeed > fSpeed[1] && vSpeed <= fSpeed[2])
  {
    Speed[1] = (vSpeed - fSpeed[1]) / (fSpeed[2] - fSpeed[1]);
  }
  else if (vSpeed > fSpeed[2] && vSpeed <= fSpeed[3])
  {
    Speed[1] = (fSpeed[3] - vSpeed) / (fSpeed[3] - fSpeed[2]);
  }
  else
  {
    Speed[1] = 0;
  }
  // Midle
  if (vSpeed <= fSpeed[2])
  {
    Speed[2] = 0;
  }
  else if (vSpeed > fSpeed[2] && vSpeed <= fSpeed[3])
  {
    Speed[2] = (vSpeed - fSpeed[2]) / (fSpeed[3] - fSpeed[2]);
  }
  else if (vSpeed > fSpeed[3] && vSpeed <= fSpeed[4])
  {
    Speed[2] = (fSpeed[4] - vSpeed) / (fSpeed[4] - fSpeed[3]);
  }
  else
  {
    Speed[2] = 0;
  }
  // Far
  if (vSpeed <= fSpeed[3])
  {
    Speed[3] = 0;
  }
  else if (vSpeed > fSpeed[3] && vSpeed <= fSpeed[4])
  {
    Speed[3] = (vSpeed - fSpeed[3]) / (fSpeed[4] - fSpeed[3]);
  }
  else if (vSpeed > fSpeed[4] && vSpeed <= fSpeed[5])
  {
    Speed[3] = (fSpeed[5] - vSpeed) / (fSpeed[5] - fSpeed[4]);
  }
  else
  {
    Speed[3] = 0;
  }
  // Very Far
  if (vSpeed <= fSpeed[4])
  {
    Speed[4] = 0;
  }
  else if (vSpeed > fSpeed[4] && vSpeed <= fSpeed[5])
  {
    Speed[4] = (vSpeed - fSpeed[4]) / (fSpeed[5] - fSpeed[4]);
  }
  else
  {
    Speed[4] = 1;
  }
  output = ((Speed[0] * LVLOW_SPEED) + (Speed[1] * LLOW_SPEED) + (Speed[2] * LMEDIUM_SPEED) + (Speed[3] * LFAST_SPEED) + (Speed[4] * LVFAST_SPEED)) /
           (Speed[0] + Speed[1] + Speed[2] + Speed[3] + Speed[4]);

  return output;
}
