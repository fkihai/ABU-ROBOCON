void trialEncoder()
{
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
  serial.println(EncY.pulseCnt);
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
  delay(1000);
  digitalWrite(DE_Rpwm_PIN, LOW);
  analogWrite(DE_Lpwm_PIN, pwm);
  digitalWrite(BE_Rpwm_PIN, LOW);
  analogWrite(BE_Lpwm_PIN, pwm);
  digitalWrite(KA_Rpwm_PIN, LOW);
  analogWrite(KA_Lpwm_PIN, pwm);
  digitalWrite(KI_Rpwm_PIN, LOW);
  analogWrite(KI_Lpwm_PIN, pwm);
  delay(1000);
}

void trialRPS(int Speed)
{
  RPSDepan(Speed, 0.01, 0.05);
  RPSBelakang(Speed, 0.01, 0.05);
  RPSKanan(Speed, 0.01, 0.05);
  RPSKiri(Speed, 0.01, 0.05);
}
