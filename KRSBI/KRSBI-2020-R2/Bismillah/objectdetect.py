
import cv2
import numpy as np
from math import *

#global variabel
#orange
#LOW_OBJECT = np.array([0,94,183])
#UP_OBJECT = np.array([54,255,255])
LOW_ORANGE = np.array(([0,100,181]),np.uint8)
UP_ORANGE = np.array(([10,255,255]), np.uint8)

#black
LOW_BLACK = np.array(([0,31,33]),np.uint8)
UP_BLACK = np.array(([255,164,45]),np.uint8)
#LOW_OBJECT = np.array([0,140,85])
#UP_OBJECT = np.array([31,255,255])

KERNEL = np.ones((3,3),np.uint8)
CONV = np.ones((10,10),np.float32)/25
SPEED = 90
CenterBall = (0,0)
CenterObstacle = (0,0)

#kalman filter
kf = cv2.KalmanFilter(4,2)
kf.measurementMatrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0]], np.float32)
kf.transitionMatrix = np.array([[1, 0, 1, 0], [0, 1, 0, 1], [0, 0, 1, 0], [0, 0, 0, 1]], np.float32)

def Estimate(coordX, coordY):
    measured = np.array([[np.float32(coordX)], [np.float32(coordY)]])
    kf.correct(measured)
    predicted = kf.predict()
    return predicted


def ColorDetect(im):
		#blue,green,red = cv2.split(im)
		#frame = cv2.circle(frame,(323, 228), 370, (200,200,200), 170)
		#frame = cv2.circle(frame,(323, 228), 35, (200,200,200), 70)

		hsv = cv2.cvtColor(im,cv2.COLOR_BGR2HSV)
		#h,s,_ = cv2.split(hsv)
		#y = np.array(((0.21*red) +(0.72*green) + (0.07*blue)),np.uint8)
		#hsy = cv2.merge((h,s,y))
		#threshold
		mask = cv2.inRange(hsv,LOW_ORANGE ,UP_ORANGE)
		mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, KERNEL)
		mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, KERNEL)		

		(_,contours,hierarchy)=cv2.findContours(mask.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		cv2.imshow('threshold',mask)
		CenterObstacle = (0,0)
		if len(contours) >0:
			contour = max(contours, key=cv2.contourArea)
			M = cv2.moments(contour)
			x = M["m10"]/M["m00"]
			y = M["m01"]/M["m00"]
			nx = 320 - x
			ny = 240 - y

			CenterObstacle = (int(x),int(y))
			
			kfObj = cv2.KalmanFilter()
			predictedCoords = np.zeros((2, 1), np.float32)
			predictedCoords = Estimate(x, y)
			# if predictedCoords[0] > 0:
    		# 		predictedCoords[0] += 2
			# elif predictedCoords[1] < 0:
    		# 		predictedCoords[1] -= 2
			# if predictedCoords[1] >0:
    		# 		predictedCoords += 2
			# elif predictedCoords[1] < 0:
    		# 		predictedCoords -= 2

			#real detected
			print "detect : ",CenterObstacle
			print "predict : ", predictedCoords[0], predictedCoords[1]
			value = '('+str(int(nx))+','+str(int(ny))+')'
			cv2.circle(frame,(int(x),int(y)),20,(255,0,0),2)
			cv2.line(frame, (int(x), int(y) + 20), (int(x) + 50, int(y) + 20), [0, 255, 0], 2, 8)
			#cv2.line(frame,(int(x),int(y) + 20),(320,240),(0,100,255),2)
			cv2.putText(frame,value,(int(x),int(y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255))

			cv2.circle(frame, (predictedCoords[0], predictedCoords[1]), 20, [0, 0, 255], 2, 8)
			cv2.putText(frame, "PREDICTION", (predictedCoords[0] + 50, predictedCoords[1] - 30), cv2.FONT_HERSHEY_SIMPLEX, 0.5, [50, 200,250])

			
		# mask1 = cv2.inRange(hsv, LOW_ORANGE, UP_ORANGE)
		# mask1 = cv2.morphologyEx(mask1, cv2.MORPH_OPEN, KERNEL)
		# mask1 = cv2.morphologyEx(mask1, cv2.MORPH_CLOSE, KERNEL)
		# #mask = cv2.filter2D(mask,-1,CONV)
		# (_,contours,hierarchy)=cv2.findContours(mask1.copy(),cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
		# cv2.imshow('threshold',mask1)
		# CenterBall = (0,0)
		# if len(contours) >0:
		# 	contour = max(contours, key=cv2.contourArea)
		# 	M = cv2.moments(contour)
		# 	x = M["m10"]/M["m00"]
		# 	y = M["m01"]/M["m00"]
		# 	nx = 320 - x
		# 	ny = 240 - y
		# 	#cm_to_pixel1 = 11.3/640.0
		# 	#cm_to_pixel2 = 11.3/480.0
		# 	#triGonometry(nx,ny)
			
		# 	CenterBall = (int(nx),int(ny))
		# 	print "bola : ",CenterBall
		# 	value = '('+str(int(nx))+','+str(int(ny))+')'
		# 	cv2.circle(frame,(int(x),int(y)),5,(255,0,0),2)
		# 	cv2.line(frame,(int(x),int(y)),(320,240),(0,100,255),2)
		# 	cv2.putText(frame,value,(int(x),int(y)),cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,255))

if __name__ == '__main__':
	cap = cv2.VideoCapture(-1)
	cap.set(3,640)
	cap.set(4,480)

	while True:
		_,frame = cap.read()
		frame = cv2.circle(frame,(323, 228), 370, (200,200,200), 170)
		frame = cv2.circle(frame,(323, 228), 30, (200,200,200), 60)
		ColorDetect(frame)
		cv2.line(frame,(320,0),(320,640),(0,0,0),1)
		cv2.line(frame,(0,240),(640,240),(0,0,0),1)
		cv2.imshow('frame',frame)
		if cv2.waitKey(1) & 0xFF == ord('q'):
			cv2.waitKey(0)
			cv2.destroyAllWindows()
			break
