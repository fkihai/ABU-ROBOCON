'''
'##:::'##:'########:::'######::'########::'####:
 ##::'##:: ##.... ##:'##... ##: ##.... ##:. ##::
 ##:'##::: ##:::: ##: ##:::..:: ##:::: ##:: ##::
 #####:::: ########::. ######:: ########::: ##::
 ##. ##::: ##.. ##::::..... ##: ##.... ##:: ##::
 ##:. ##:: ##::. ##::'##::: ##: ##:::: ##:: ##::
 ##::. ##: ##:::. ##:. ######:: ########::'####:
..::::..::..:::::..:::......:::........:::....::

  Unit          : Engine Library
  Rev           : 32
  Release Date  : 30 Maret 2018
'''

import serial
from time import sleep
from struct import pack, unpack

# Code Execution
CODE__EXEC = '*'

# AT I2C
AT_STOP = 0x00
AT_RST = 0x01
AT_DR__OFF = 0x02
AT_DR__ON = 0x03
AT_WRPS = 0x04
AT_RMENC = 0x05
AT_RXYT = 0x06
AT_WXYT = 0x07
AT_RSENC = 0x08
AT_KICK = 0x09
AT_RCVOLT = 0x0A
AT_MOD_RPS = 0x0B
AT_GXYT = 0x0C
AT_LVOLT = 0x0D
AT_RTIME = 0x0E
AT_ENOFF = 0x0F
AT_RALL = 0x10
AT_DRIBBLE = 0x11

DRB_ON = 1
DRB_OFF = 0

piSerial = serial.Serial()

def __exit():
    if piSerial.isOpen():
        piSerial.write(pack('cb', CODE__EXEC, AT_RST))
        piSerial.close()

def __init(port):
	global piSerial
	piSerial = serial.Serial(port, 115200, timeout=0.1)

def RESET():
	piSerial.write(pack('cb', CODE__EXEC, AT_RST))

def readAll():
	piSerial.write(pack('cb', CODE__EXEC, AT_RALL))
	dAll = piSerial.read(10)

	if dAll != '':
		dAll = unpack('hhhhh', dAll)
	else:
		dAll = [0,0,0,0,0]
		print "Engine: Error"
	return dAll

def readTime():
	piSerial.write(pack('cb', CODE__EXEC, AT_RTIME))
	etime = unpack('hhh', piSerial.read(6))
	return etime

def engineOff():
	piSerial.write(pack('cb', CODE__EXEC, AT_ENOFF))

def setRPS(back, right, left):
	piSerial.write(pack('cbhhh', CODE__EXEC, AT_WRPS, back, right, left))

def Dribble(right,left):
	piSerial.write(pack('cbhh', CODE__EXEC, AT_DRIBBLE,right, left))

def modRPS(rps):
    piSerial.write(pack('cbh', CODE__EXEC, AT_MOD_RPS, rps))

def setXYT(x, y, t):
	piSerial.write(pack('cbhhh', CODE__EXEC, AT_WXYT, x, y, t))

def goXYT(x, y, t):
	piSerial.write(pack('cbhhh', CODE__EXEC, AT_GXYT, x, y, t))

def setDribble(mode):
	if mode == DRB_ON:
		piSerial.write(pack('cb', CODE__EXEC, AT_DR__ON))
	elif mode == DRB_OFF:
		piSerial.write(pack('cb', CODE__EXEC, AT_DR__OFF))

def setKick(kick):
    if kick < 0 :
        kick = 0
    elif kick > 255:
        kick = 255
    piSerial.write(pack('cbh', CODE__EXEC, AT_KICK, kick))

def setCVolt(volt):
    	piSerial.write(pack('cbh', CODE__EXEC, AT_LVOLT, volt))

def readCVolt():
	piSerial.write(pack('cb', CODE__EXEC, AT_RCVOLT))
	cvolt = piSerial.read(2)
	if cvolt != '':
		cvolt = unpack('h', cvolt)
	else:
		cvolt = [0]
	return cvolt[0]

def STOP():
	piSerial.write(pack('cb', CODE__EXEC, AT_STOP))

def readXYT():
	piSerial.write(pack('cb', CODE__EXEC, AT_RXYT))
	xyt = piSerial.read(6)

	if xyt != '':
		xyt = unpack('hhh', xyt)
	else:
		xyt = [0,0,0]
	return xyt

def readMasterEnc():
	piSerial.write(pack('cb', CODE__EXEC, AT_RMENC))
	enc = piSerial.read(12)

	if enc != '':
		enc = unpack('iii', enc)
	else:
		enc = [0,0,0]
	return enc

def readSlaveEnc():
	piSerial.write(pack('cb', CODE__EXEC, AT_RSENC))
	enc = piSerial.read(12)

	if enc != '':
		enc = unpack('iii', enc)
	else:
		enc = [0,0,0]
	return enc
