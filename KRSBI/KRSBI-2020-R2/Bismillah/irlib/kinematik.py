
'''
update : 21 februari 2019

'''

from math import *
import vision as vs
#import engine as en

pwmMax = 100
##sudut masing" sumbu roda
pi = 3.141592654
sudutNorm = 90
sudutNormRadian = sudutNorm * pi / 180
sudutAkselerasiMaju = 67 #akan belok sekaligus maju

def speed_out(rx,ry):
    PWM_Out = [0]*3
    '''
    Doc Number python : https://belajarpython.com/tutorial/number-python
    '''
    # Kembali atan (y / x), di radian (KONVERSI DARI DERAJAT KE RADIAN)
    jarakPixel = 0
    jarakPixel = ((ry*ry)+(rx*rx))**0.5
    jarakMeter = FittingJarakKeMeter(jarakPixel)
    c = 0;
    b = 0;
    tetha = atan2(ry,rx)
    #0 = belakang, 1 = kanan, 2 = kiri
    tethaNormalized = tetha - sudutNormRadian;
    tDeg = tetha * 180.0 / pi
    tDegNorm = tethaNormalized * 180.0 / pi
    if (tDegNorm<-180.0):
		v = tDegNorm
		tDegNorm = (360.0+v)
    #reconvert back again
    tethaNormalized = tDegNorm * pi / 180.0
    #print " tetha = ", tDeg, ", tethaNorm = ", tDegNorm
    print "jarakPixel = ", jarakPixel, ", jarakMeter = ", jarakMeter
    
    factorRotasiSpeedKiri = 0.0
    factorRotasiSpeedKanan = 0.0
    factorRotasiSpeedBelakang = 0.0
    
    factorAkselerasiMaju = 0.0
    
    if jarakPixel>0.0:
		if ((abs(tDegNorm)>=sudutAkselerasiMaju) or ((jarakMeter<0.1) and ((abs(tDegNorm)>=2.0)))):
			#berputar saja
			factorRotasiSpeedKiri = tDegNorm / 180
			factorRotasiSpeedKanan = tDegNorm / 180
			factorRotasiSpeedBelakang = (factorRotasiSpeedKanan+factorRotasiSpeedKiri)/2.0
			factorRotasiSpeedKanan = HalfNorm(factorRotasiSpeedKanan)
			factorRotasiSpeedBelakang = HalfNorm(factorRotasiSpeedBelakang)
			factorKecepatanMaju = 0
		else:
			#berputar sambil maju
			factorAkselerasiMaju = 0.05 + (jarakMeter / 4)
			if factorAkselerasiMaju > 1.0:
				factorAkselerasiMaju = 1.0
			#factorRotasiSpeedKiri = HalfNorm(-(ThrustPowerFactorLeftTire(tDegNorm, jarakMeter)) * factorAkselerasiMaju)
			#factorRotasiSpeedKanan = HalfNorm((ThrustPowerFactorRightTire(tDegNorm, jarakMeter)) * factorAkselerasiMaju)
			#factorRotasiSpeedKiri = factorAkselerasiMaju * rKiri
			#factorRotasiSpeedKanan = factorAkselerasiMaju * rKanan
			if (tethaNormalized<=0):
				rKiri = 1.0
				#rKanan = cos(tethaNormalized)
				rKanan = 1.0 - (abs(tDegNorm) / 90.0)
			else:
				rKanan = 1.0
				#rKiri = cos(tethaNormalized)
				rKiri = 1.0 - (abs(tDegNorm) / 90.0)
			
			#print "rKiri = ", rKiri, ", rKanan = ", rKanan
			
			factorRotasiSpeedKiri = factorAkselerasiMaju * -rKiri
			factorRotasiSpeedKanan = factorAkselerasiMaju * rKanan
			#factorRotasiSpeedBelakang = factorAkselerasiMaju * ((rKanan+-rKiri) / 2.0)
			factorRotasiSpeedBelakang =  (tDegNorm / 90.0) * 3.0
    
    PWM_Out[0]= int( factorRotasiSpeedBelakang * pwmMax ) # belakang
    PWM_Out[1]= int( factorRotasiSpeedKanan * pwmMax ) # kanan
    PWM_Out[2]= int( factorRotasiSpeedKiri * pwmMax ) #kiri
    
    #PWM_Out[0]=((int(-rx[0])) *-1 )  # belakang
    #PWM_Out[1]=((int((float(0.5) * rx[1]) - (float(0.8660) * ry[1]))) *-1 )  # kanan
    #PWM_Out[2]=((int((float(0.5) * rx[2]) + (float(0.8660) * ry[2]))) * -1 ) #kiri
    #print (PWM_Out[0],PWM_Out[1],PWM_Out[2])
    return (PWM_Out[0],PWM_Out[1],PWM_Out[2])
    
def degree(x,y):
	#pi = 3.141592654
	rad = (atan2(x,y))
	deg = int(rad *360)/(2*pi)
	return deg

def NearestNorm(x):
	if x<0.0:
		return -(abs(x)**0.5)
	else:
		return x**0.5

def HalfNorm(x):
	if (abs(x)>0.75):
		return x;
	else:
		return x / 2.0

def SigmoidNorm(x):
	limit = 0.015
	if (x<limit):
		return x;
	f = 1.5
	if x<0.0:
		v = -(abs(x)**f)
		if (v<(-limit)):
			v = -limit;
	else:
		v = x**f
		if (v<(limit)):
			v = limit;
	return v

def FittingJarakKeMeter(x):
	xNol = x-69.0
	a = 6.052582159287442E+00
	b = -4.711195260682217E-02
	c = 9.212329742386121E-05
	return (xNol/ (a+(b*xNol)+(c*(xNol**2.0))))

def ThrustPowerFactorLeftTire(degree, r):
	if (degree<=0.0) : return 1.0
	if (degree>90.0) : return 0
	return ((90.0-degree)/90.0) ** 0.85

def ThrustPowerFactorRightTire(degree, r):
	if (degree>=0.0) : return 1.0
	if (degree<-90.0) : return 0
	return ((90.0-abs(degree)) / 90.0) ** 0.85
