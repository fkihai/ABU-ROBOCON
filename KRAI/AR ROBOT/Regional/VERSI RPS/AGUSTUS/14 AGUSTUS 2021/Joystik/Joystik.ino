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

  /* PRESS KOTAK AND FRIEND (SLAVE)*/
  if (ps2x.ButtonPressed(PSB_TRIANGLE))
  {
    Master.print('P');
    Slave.print('X');
  }
  else if (ps2x.ButtonReleased(PSB_TRIANGLE))
  {
    Slave.print('Z');
  }

  if (ps2x.ButtonPressed(PSB_CIRCLE))
  {
    Circle.Ctrl++;
    if (Circle.Ctrl == 1)
    {
      Slave.print('F');
    }
    else if (Circle.Ctrl == 2)
    {
      Slave.print('G');
      Circle.Ctrl = 0;
    }
  }
  else if (ps2x.ButtonReleased(PSB_CIRCLE))
  {
    Slave.print('Z');
  }

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

  if (ps2x.ButtonPressed(PSB_SQUARE))
  {
    Slave.print('J');
    Master.print('J');
  }
  else if (ps2x.ButtonReleased(PSB_SQUARE))
  {
    Slave.print('Z');
    Master.print('Z');
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

  /* L DAN R (MASTER) */
  if (ps2x.ButtonPressed(PSB_R1))
  {
    Master.print('E');
  }
  else if (ps2x.ButtonReleased(PSB_R1))
  {
    Master.print('Z');
  }
  if (ps2x.ButtonPressed(PSB_L1))
  {
    Master.print('F');
  }
  else if (ps2x.ButtonReleased(PSB_L1))
  {
    Master.print('Z');
  }

  if (ps2x.ButtonPressed(PSB_R2))
  {
    Master.print('G');
  }
  else if (ps2x.ButtonReleased(PSB_R2))
  {
    Master.print('Z');
  }

  if (ps2x.ButtonPressed(PSB_L2))
  {
    Master.print('H');
  }
  else if (ps2x.ButtonReleased(PSB_L2))
  {
    Master.print('Z');
  }

  /* L DAN R (SLAVE) */

  if (ps2x.ButtonPressed(PSB_R3))
  {
    R3.Ctrl++;
    if (R3.Ctrl == 1 && !R3.Push)
    {
      Master.print('I');
      R3.Push = true;
    }
    else if (R3.Ctrl == 2 && R3.Push)
    {
      Master.print('J');
      R3.Push = false;
    }
    else if (R3.Ctrl == 3 && !R3.Push)
    {
      Master.print('K');
      R3.Push = true;
    }

    else if (R3.Ctrl == 4 && R3.Push)
    {
      Master.print('L');
      R3.Push = true;
    }
    else if (R3.Ctrl == 5 && R3.Push)
    {
      Master.print('M');
      R3.Push = true;
    }
    else if (R3.Ctrl == 6 && R3.Push)
    {
      Master.print('N');
      R3.Push = true;
    }

    else if (R3.Ctrl == 7 && R3.Push)
    {
      Master.print('O');
      R3.Push = false;
      R3.Push = 10;
    }
  }
  else if (ps2x.ButtonReleased(PSB_R3))
  {
    Master.print('Z');
  }

  LY = (int)ps2x.Analog(PSS_LY);
  LX = (int)ps2x.Analog(PSS_LX);
  RY = (int)ps2x.Analog(PSS_RY);
  RX = (int)ps2x.Analog(PSS_RX);

  if (RX != RXold || RY != RYold)
  {
    if (RY == 0 && RX == 128) //majupelan
    {
      Slave.print('N');
    }
    if (RY == 255 && RX == 128) //mundurpelan
    {
      Slave.print('O');
    }
    if (RY == 127 && RX == 0) // SLIDER PUTAR KIRI
    {
      Slave.print('P');
    }
    if (RY == 127 && RX == 255) // SLIDER PUTAR KANAN
    {
      Slave.print('Q');
    }
    if (RY == 127 && RX == 128)
    {
      Slave.print('Z');
    }
  }

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
    if (LY == 127 && LX == 0) // SLIDER GESER KIRI
    {
      Slave.print('T');
    }
    if (LY == 127 && LX == 255) // SLIDER GESER KANAN
    {
      Slave.print('U');
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
