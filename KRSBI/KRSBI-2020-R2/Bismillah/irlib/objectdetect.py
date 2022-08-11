
import cv2
import numpy as np
from math import *

#global variabel
LOW_OBJECT = np.array([0,128,154])
UP_OBJECT = np.array([15,255,255])
#LOW_OBJECT = np.array(([0,89,95]),np.uint8)
#UP_OBJECT = np.array(([7,255,255]),np.uint8)
#LOW_OBJECT = np.array([0,140,85])
#UP_OBJECT = np.array([31,255,255])

KERNEL = np.ones((3,3),np.uint8)
CONV = np.ones((10,10),np.float32)/25
SPEED = 90
CenterBall = (0,0)
def triGonometry(vx,vy):
	r = int(sqrt((vx**2)+(vy**2)))
	radian = atan2(vx,vy)
	degree = int(degrees(radian))
	jarak_meter = FittingJarakKeMeter(vy)
	x_real = jarak_meter * cos(degree)
	y_real = jarak_meter * sin(degree)
	#Vx = SPEED*cos(radian)
	#Vy = SPEED*sin(radian)
	print 'r=',r,'jarak_meter=',jarak_meter,'degree=',degree, 'xreal', x_real, 'y_real', y_real

def FittingJarakKeMeter(x):
	xNol = x - 69
	a = 6.052582159287442E+00
	b = -4.711195260682217E-02
	c = 9.212329742386121E-05
	return (x / (a+(b*xNol)+(c*(xNol**2.0))))

def ColorDetect(im):
		blue,green,red = cv2.split(im)
		hsv = cv2.cvtColor(im,cv2.COLOR_BGR2HSV)
		h,s,_ = cv2.split(hsv)
		y = np.array(((0.21*red) +(0.72*green) + (0.07*blue)),np.uint8)
		hsy = cv2.merge((h,s,y))
		#threshold
		mask = cv2.inRange(hsy,LOW_OBJECT ,UP_OBJECT)
		mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)
		mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, KERNEL)
		mask = cv2.filter2D(mask,-1,CONV)
		cv2.imshow('threshold',mask)
		(_,contours,hierarchy)=cv2.findContours(mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		CenterBall = (0,0)
		if len(contours) >0:
			contour = max(contours, key=cv2.contourArea)
			M = cv2.moments(contour)
			x = M["m10"]/M["m00"]
			y = M["m01"]/M["m00"]
			nx = 320 - x
			ny = 240 - y
			triGonometry(nx,ny)
			CenterBall = (int(nx),int(ny))
			print "bola : ",CenterBall
			value = '('+str(int(nx))+','+str(int(ny))+')'
			#fiting = '('+str(int(jarak_meter)) + ')'
			cv2.circle(frame,(int(x),int(y)),5,(255,0,0),2)
			cv2.line(frame,(int(x),int(y)),(320,240),(0,100,255),2)
			cv2.putText(frame,value,(int(x),int(y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255))
			#cv2.putText(frame,fiting,(int(x),int(y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255))

if __name__ == '__main__':
	cap = cv2.VideoCapture(0)
	cap.set(3,640)
	cap.set(4,480)

	while True:
		_,frame = cap.read()
		ColorDetect(frame)
		cv2.line(frame,(320,0),(320,640),(0,0,0),2)
		cv2.line(frame,(0,240),(640,240),(0,0,0),2)
		cv2.imshow('frame',frame)
		if cv2.waitKey(1) & 0xFF == ord('q'):
			cv2.waitKey(0)
			cv2.destroyAllWindows()
			break
