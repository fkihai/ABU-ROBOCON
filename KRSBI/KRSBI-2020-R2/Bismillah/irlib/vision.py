import cv2
import numpy as np
from math import *
import threading as th
from time import sleep
CONV = np.ones((10,10),np.float32)/25
# Konstanta
#range warna orangex
# R3_LOW_OBJECT = np.array([0,99,236])
# R3_UP_OBJECT = np.array([15,255,255])

kf = cv2.KalmanFilter(4,2)
kf.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
kf.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32)


def Estimate(coordX, coordY):
    measured = np.array([[np.float32(coordX)], [np.float32(coordY)]])
    kf.correct(measured)
    predicted = kf.predict()
    return predicted

R3_LOW_OBJECT = np.array([0,81,220])
R3_UP_OBJECT = np.array([22,255,255])

R2_LOW_OBJECT = np.array([0,100,186])
R2_UP_OBJECT = np.array([10,255,255])

KERNEL = np.ones((3,3),np.uint8)

#global variabel
low_object = R2_LOW_OBJECT
up_object = R2_UP_OBJECT
frame = np.zeros((640,480,3),np.uint8)
CenterBall = (0,0)
Obstacle = [0,0,0]
predict = (0,0)
predictedCoords = []
pack = []

def _init_(width,hight,rID,win=False):
	global show_window
	global low_object
	global up_object
	global cap

	imgThread = th.Thread(target = update)
	ImageProcThread = th.Thread(target = ColorDetect)

	imgThread.daemon = True
	ImageProcThread .daemon = True

	show_window = win
	if rID == "r3":
		low_object = R3_LOW_OBJECT
		up_object = R3_UP_OBJECT
	elif rID == "r2":
		low_object = R2_LOW_OBJECT
		up_object = R2_UP_OBJECT

	cap = cv2.VideoCapture(0)
	cap.set(3,width)
	cap.set(4,hight)

	imgThread.start()
	sleep(1)
	ImageProcThread .start()

def update():
	global frame
	global cap
	global show_window

	while True:
		_,frame = cap.read()
		#frame = cv2.flip(frame,0)
		if show_window == True:
			cv2.imshow('frame',frame)

def ColorDetect():
	global frame
	global CenterBall
	global show_window
	global predict
	while 1:
		hsv = cv2.cvtColor(frame,cv2.COLOR_BGR2HSV)
		mask = cv2.inRange(hsv,low_object ,up_object)
		mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)
		mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, KERNEL)
		#(_,contours,hierarchy)=cv2.findContours(mask.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		(_,contours,hierarchy)=cv2.findContours(mask.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
		CenterBall = (0,0)
		if len(contours) >0:
			contour = max(contours, key=cv2.contourArea)
			x,y,w,h = cv2.boundingRect(contour)
			M = cv2.moments(contour)
			x = M["m10"]/M["m00"]
			y = M["m01"]/M["m00"]
			nx = 320 - x
			ny = 240 - y
			CenterBall = (int(nx), int(ny))
			
			kfObj = cv2.KalmanFilter()
			predictedCoords = np.zeros((2, 1), np.float32)
			predictedCoords = Estimate(int(nx), int(ny))
			predict = (int(predictedCoords[0][0]), int(predictedCoords[1][0]))

def BallObstacle():
	return CenterBall
def show():
        if cv2.waitKey(0) & 0xFF == ord('q'):
                cap.release()
                cv2.destroyAllWindows()
