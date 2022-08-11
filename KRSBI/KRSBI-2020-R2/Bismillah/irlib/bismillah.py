# library
# LOCAL
'''
##############################################
    Edit : 16 juni 2019

##############################################
'''

import irlib.engine as en
import irlib.netCore as nc
import irlib.vision as vs
import irlib.kinematix as kn
import irlib.kinematixV1 as mv

#SYSTEM
import threading  as th
from time import sleep, time
from math import *
import signal as sg

# Global Konstanta
ENGINE_DEV = '/dev/ttyACM1'
BROKER_ADDR = '10.10.10.3' #ip basestation
ID = 'r3'

#Global variabel
data = {}   # data yang akan dipublish
loop = True # kontrol signal
trackBallBool = False
findBallBool = False
dribbleBool = True
ballRange =200
ballStat = 1
control = 0

useGUI = False

def robotInit():
    en.__init(ENGINE_DEV)
    nc.__init(BROKER_ADDR, ID)
    vs._init_(640,480,ID,useGUI)
    sleep(3)
    en.RESET()
    nc.subscribe('bt')

def pixel(rx,ry):
	jarakPixel = 0
	jarakPixel = ((ry*ry)+(rx*rx))**0.5
	return jarakPixel

def publishData():
	global data
	global ballStat
	dAll = en.readAll()
	data['pos'] = dAll[:3:]
	data['cap'] = dAll[3]
	ballStat = dAll[4]
	(ball_x,ball_y) = vs.BallObstacle()
	(data['ball']) = pixel(ball_x, ball_y)
	data['track'] = ballStat

	# print data
	nc.publish(data)

def mainProccess():
	global data
	global ballRange
	global trackBallBool
	global dribbleBool
	global findBallBool

	interval = time()
	while loop:
		msg = nc.readData('bt')
		if msg != None:
			nc.clearData()
			if ID in msg:
				msg = msg[ID]
				if msg['CMD'] == 'Track':
					trackBallBool = True
					findBallBool = False
					data['status'] = 'Tracking'
					en.modRPS(msg['speed'])
					ballRange = msg['range']
					dribbleBool = msg['drib']
					# nc.clearData()
				elif msg['CMD'] == 'Tracking':
					trackBallBool = False
					findBallBool = True
					data['status'] = 'Tracking'
					en.modRPS(msg['speed'])
					ballRange = msg['range']
					dribbleBool = msg['drib']
					# nc.clearData()
	
				elif msg['CMD'] == 'Goxyt':
					trackBallBool = False
					findBallBool = False
					pos = msg['pos']
					en.modRPS(msg['speed'])
					sleep(0.05)
					en.goXYT(pos[0], pos[1], pos[2])
					data['status'] = 'Goto Position'
					# nc.clearData()
				elif msg['CMD'] == 'Kick':
					en.setKick(msg['pwm'])
					data['status'] = 'Kick'
					# nc.clearData()
				elif msg['CMD'] == 'Stop':
					en.STOP()
					sleep(0.05)
					en.setDribble(0)
					data['status'] = 'Stop'
					trackBallBool = False
					findBallBool = False

					# nc.clearData()
				elif msg['CMD'] == 'Cap':
					en.setCVolt(msg['volt'])
					data['status'] = 'Setting Capasitor'
					# nc.clearData()
				elif msg['CMD'] == 'Setxyt':
					pos = msg['pos']
					en.STOP()
					sleep(0.05)
					en.setXYT(pos[0],pos[1],pos[2])
					data['status'] = 'Setting Position'
					sleep(1.5)
					# nc.clearData()
				elif msg['CMD'] == 'EngineOff':
					en.STOP()
					sleep(0.05)
					en.engineOff()
					data['status'] = "Engine Off"

		if (time() - interval) > 0.05:
			publishData()
			interval = time()

			if trackBallBool:
				(ball_x,ball_y) = vs.BallObstacle()
				xMember = ball_x
				yMember = ball_y	
				print ("x =",  xMember, "y = ", yMember)
				if ballStat == 0 and ballRange == 0:
					trackBallBool = False
					data['status'] = 'Tracking Finish'
					en.STOP()
					en.setDribble(dribbleBool)
				elif (ballStat == 0 and yMember < ballRange) and yMember != 0:
					print "ballstat", ballStat, "BallRange : ", ballRange
					trackBallBool = False
					data['status'] = 'Tracking Finish'
					en.STOP()
					en.setDribble(dribbleBool)
				else:

					if (pixel(ball_x,ball_y)) < 200:
						en.setDribble(1)
					else:
						en.setDribble(1)
					xMember = ball_x
					yMember = ball_y
					pwmBelakang,pwmKanan,pwmKiri= kn.speed_out(ballRange)
					en.setRPS(pwmBelakang,pwmKanan,pwmKiri)
					print "ballRange : ", ballRange
			if findBallBool:
				(ball_x,ball_y) = vs.BallObstacle()
				xMember = ball_x
				yMember = ball_y	
				print ("x =",  xMember, "y = ", yMember)
				if ballStat == 0 and ballRange == 0:
					trackBallBool = False
					data['status'] = 'Tracking Finish'
					en.STOP()
					en.setDribble(dribbleBool)
				elif (ballStat == 0 and yMember < ballRange) and yMember != 0:
					print "ballstat", ballStat, "BallRange : ", ballRange
					trackBallBool = False
					data['status'] = 'Tracking Finish'
					en.STOP()
					en.setDribble(dribbleBool)
				else:

					if (pixel(ball_x,ball_y)) < 200:
						en.setDribble(1)
					else:
						en.setDribble(1)
					xMember = ball_x
					yMember = ball_y
					pwmBelakang,pwmKanan,pwmKiri= mv.speed_out(xMember, yMember)
					en.setRPS(pwmBelakang,pwmKanan,pwmKiri)
					print "ballRange : ", ballRange
		sleep(0.01)

def __signal(signum, fram):
    global loop
    print "Exiting Program"
    loop = False
    sleep(1)
    en.RESET()
    sleep(1)
    en.__exit()

if __name__ == '__main__':
    print "Init..."
    robotInit()
    print "Init Finish"
    #pubThread = th.Thread(target=publishData)
    # trackThread = th.Thread(target=trackBall)
    #pubThread.daemon = True
    # trackThread.daemon = True
    data['status'] = 'standby'
    #pubThread.start()
    # trackThread.start()

    sg.signal(sg.SIGINT, __signal)
    print "\n\n"
    print "+---------------+"
    print "|.::Bismillah::.|"
    print "+---------------+"
    mainProccess()
