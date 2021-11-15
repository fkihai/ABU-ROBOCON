#include <PS2X_lib.h>
#include <SoftwareSerial.h>

SoftwareSerial Slave(2, 3);
#define Master Serial

PS2X ps2x;

byte vibrate = 0;
int LY, LX, RY, RX, LYold, LXold, RYold, RXold;

struct
{
  int Ctrl = 0;
  bool Push = false;
} Triangle, Circle, Square, Cross, R3, L2, Start, RL1;

void setup()
{
  Master.begin(9600);
  Slave.begin(9600);
  delay(300);
  ps2x.config_gamepad(13, 11, 10, 12, false, false);
}

void loop()
{
  // Monitor();
  SendData();
}

void SendData()
{
  ps2x.read_gamepad(false, vibrate);

  /* TOMBOL ARAH (MASTER) */
  if (ps2x.ButtonPressed(PSB_PAD_UP))
  {
    Master.print('A');
  }
  else if (ps2x.ButtonReleased(PSB_PAD_UP))
  {
    Master.print('Z');
  }

  if (ps2x.ButtonPressed(PSB_PAD_DOWN))
  {
    Master.print('B');
  }
  else if (ps2x.ButtonReleased(PSB_PAD_DOWN))
  {
    Master.print('Z');
  }
  if (ps2x.ButtonPressed(PSB_PAD_RIGHT))
  {
    Master.print('C');
  }
  else if (ps2x.ButtonReleased(PSB_PAD_RIGHT))
  {
    Master.print('Z');
  }
  if (ps2x.ButtonPressed(PSB_PAD_LEFT))
  {
    Master.print('D');
  }
  else if (ps2x.ButtonReleased(PSB_PAD_LEFT))
  {
    Master.print('Z');
  }

  /** TRIANGLE **/
  if (ps2x.ButtonPressed(PSB_TRIANGLE))
  {
    Master.print('P');
    Slave.print('X');
  }
  else if (ps2x.ButtonReleased(PSB_TRIANGLE))
  {
    Slave.print('Z');
    Master.print('Z');
  }

  /** CIRCLE **/
  if (ps2x.ButtonPressed(PSB_CIRCLE))
  {
    Master.print('V');
    Slave.print('X');
  }
  else if (ps2x.ButtonReleased(PSB_CIRCLE))
  {
    Master.print('Z');
    Master.print('Z');
  }

  /** SQUARE **/
  if (ps2x.ButtonPressed(PSB_SQUARE))
  {
    Master.print('X');
    Slave.print('X');
  }
  else if (ps2x.ButtonReleased(PSB_SQUARE))
  {
    Master.print('Z');
    Master.print('Z');
  }

  /** CROSS **/
  if (ps2x.ButtonPressed(PSB_CROSS))
  {
    Cross.Ctrl++;
    if (Cross.Ctrl == 1)
    {
      Slave.print('H');
    }
    if (Cross.Ctrl == 2)
    {
      Cross.Ctrl = 0;
      Slave.print('I');
    }
  }
  else if (ps2x.ButtonReleased(PSB_CROSS))
  {
    Slave.print('Z');
  }

  /* START AND SELECT */
  if (ps2x.ButtonPressed(PSB_START))
  {
    Triangle.Ctrl = 0;
    Cross.Ctrl = 0;
    Square.Ctrl = 0;
    Circle.Ctrl = 0;
    R3.Ctrl = 0;
    R3.Push = false;
    Slave.print('*');
    Master.print('*');
  }
  else if (ps2x.ButtonReleased(PSB_START))
  {
    Slave.print('Z');
  }

  /* L DAN R (SLAVE) */
  if (ps2x.ButtonPressed(PSB_R1))
  {
    Slave.print('Q');
  }
  else if (ps2x.ButtonReleased(PSB_R1))
  {
    Slave.print('Z');
  }
  if (ps2x.ButtonPressed(PSB_L1))
  {
    Slave.print('P');
  }
  else if (ps2x.ButtonReleased(PSB_L1))
  {
    Slave.print('Z');
  }

  if (ps2x.ButtonPressed(PSB_R2))
  {
    Slave.print('U');
  }
  else if (ps2x.ButtonReleased(PSB_R2))
  {
    Slave.print('Z');
  }

  if (ps2x.ButtonPressed(PSB_L2))
  {
    Slave.print('T');
  }
  else if (ps2x.ButtonReleased(PSB_L2))
  {
    Slave.print('Z');
  }

  LY = (int)ps2x.Analog(PSS_LY);
  LX = (int)ps2x.Analog(PSS_LX);
  RY = (int)ps2x.Analog(PSS_RY);
  RX = (int)ps2x.Analog(PSS_RX);

  if (LX != LXold || LY != LYold)
  {
    if (LY == 0 && LX == 128) // POWER WINDOW MAJU
    {
      Slave.print('R');
    }
    if (LY == 255 && LX == 128) // POWER WINDOWS MUNDUR
    {
      Slave.print('S');
    }
    if (LY == 127 && LX == 128)
    {
      Slave.print('Z');
    }
  }

  LYold = LY;
  LXold = LX;
  RYold = RY;
  RXold = RX;

  delay(20);
}

void Monitor()
{

  ps2x.read_gamepad(false, vibrate);

  LY = (int)ps2x.Analog(PSS_LY);
  LX = (int)ps2x.Analog(PSS_LX);
  RY = (int)ps2x.Analog(PSS_RY);
  RX = (int)ps2x.Analog(PSS_RX);

  Master.print(" L : ");
  Master.print(LY, DEC);
  Master.print(',');
  Master.print(LX, DEC);
  Master.print('\t');
  Master.print("R : ");
  Master.print(RY, DEC);
  Master.print(',');
  Master.println(RX, DEC);

  LYold = LY;
  LXold = LX;
  RYold = RY;
  RXold = RX;
}
