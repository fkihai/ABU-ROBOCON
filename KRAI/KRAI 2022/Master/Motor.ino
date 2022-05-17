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
