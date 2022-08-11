
import cv2
import numpy as np
import csv
from time import time,localtime,sleep

#Global Variabel
CNT = 1
font = cv2.FONT_HERSHEY_SIMPLEX
KERNEL = np.ones((8,8), np.uint8)
#fungsi pass
def nothing(x):
	pass

#capture kamera webcam
cap = cv2.VideoCapture(0)
cap.set(3,640)
cap.set(4,480)
#membuat windows dan trackbar
cv2.namedWindow('framing and mask')
cv2.createTrackbar('h_low','framing and mask',0,255,nothing)
cv2.createTrackbar('h_high','framing and mask',0,255,nothing)
cv2.createTrackbar('s_low','framing and mask',0,255,nothing)
cv2.createTrackbar('s_high','framing and mask',0,255,nothing)
cv2.createTrackbar('v_low','framing and mask',0,255,nothing)
cv2.createTrackbar('v_high','framing and mask',0,255,nothing)

while True:
	_,frame = cap.read()
	#mengambil nilai BGR(blue,green,red)
	#frame =cv2.flip(frame,0)
	#frame = cv2.GaussianBlur(frame, (5,5),0)
	#konversi BGR ke HSV(hue, saturation, value)
	img = cv2.cvtColor (frame , cv2.COLOR_BGR2HSV)
	#split data( memisah sub pixel (hue, saturation, value)
	h,s,v = cv2.split(frame)
	#mengambil nilai Trackbar
	H_LOW = cv2.getTrackbarPos('h_low','framing and mask')
	H_HIGH = cv2.getTrackbarPos('h_high','framing and mask')
	S_LOW = cv2.getTrackbarPos('s_low','framing and mask')
	S_HIGH = cv2.getTrackbarPos('s_high','framing and mask')
	V_LOW = cv2.getTrackbarPos('v_low','framing and mask')
	V_HIGH = cv2.getTrackbarPos('v_high','framing and mask')

	L_Color = np.array([H_LOW,S_LOW,V_LOW])

	H_Color = np.array([H_HIGH, S_HIGH, V_HIGH])
	#thresholding hue
	mask_hue = cv2.inRange (h, H_LOW, H_HIGH)
	#thresholding saturation
	mask_Sat = cv2.inRange (s, S_LOW, S_HIGH)
	#thresholding value
	mask_Va = cv2.inRange (v, V_LOW, V_HIGH)
	#thresholding warna yang akan di deteksi
	mask_Color=cv2.inRange (img,L_Color, H_Color)
	cnts= cv2.findContours(mask_Color.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
	if len(cnts)>0:
		c = max(cnts, key=cv2.contourArea)
		x,y,w,h = cv2.boundingRect(c)
		cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),2)
	cv2.imshow('thresholding Hue, Saturation, Value', np.hstack([mask_hue, mask_Sat, mask_Va]))
	mask_color =cv2.cvtColor(mask_Color, cv2.COLOR_GRAY2BGR)
	mask_color =cv2.morphologyEx(mask_Color, cv2.MORPH_OPEN, KERNEL)
	mask_color =cv2.morphologyEx(mask_Color, cv2.MORPH_CLOSE, KERNEL)
	#Timer
	Time = localtime(time())
	Time = str(Time[3])+':'+str(Time[4]) + ':' + str(Time[5])
	cv2.putText(frame,Time,(10,20), font, 0.5,(255,255,255),1)
	cv2.imshow('framing ',frame)
	cv2.imshow('mask', mask_color)
	if cv2.waitKey(1) & 0xFF == ord('q'):
		break
cap.release()
cv2.destroyAllWindows()
