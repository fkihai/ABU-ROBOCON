

from math import *
import vision as vs
import engine as en
from time import sleep,time
#id = []
pwmMax = 0.4
def speed_out(distance):
    PWM_Out = [0]*3
    (ball_x,ball_y) = vs.BallObstacle()
    rx = ball_x
    ry = ball_y
    s = int(sqrt((rx**2)+(ry**2)))
    distance_tolerance = distance
    angle_tolerance = 2
    print "jarak", s

    goal_linear_x_velocity = 0.0
    goal_linear_y_velocity = 0.0
    goal_angular_velocity = 0.0
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    linear_x = linear_y = angular_z = 0
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    WHEEL_RADIUS = 0.029 # in meter
    DISTANCE_CENTER_TO_WHEEL = 0.1064 # in meter 

    Vx = rx  *  0.07
    Vy = ry  * 0.07

    LIMIT_X_MAX_VALUE = 100
    
    #if ry <= 77 and ry > 50:
#	Vy = 1
		#Vx = rx * 0.2
    # if Vx > 10:
    #     Vx = 10
    # elif Vx > -10:
    #     Vx = -10
 # elif Vy > -10:
    #     Vy = 10
                                                                                                                                                                                                                                                                                                                                
    radian = atan2(rx,ry)
    degree = int(degrees(radian))
    w = Kalman(degree) * 0.9
    print("degree", degree)
                #if 
                #PID = pid(degree)
                                                                                                                                                                                                                                                                                                                                
    if ry <= distance_tolerance  and  abs(degree) <= angle_tolerance  :
        PWM_Out[0] = 0
        PWM_Out[1] = 0
        PWM_Out[2] = 0
        return (PWM_Out[0], PWM_Out[1], PWM_Out[2])
 
    else :
        linear_y = pwmMax * Vy
        linear_x = pwmMax * Vx
        angular_z = w
        #if angular_z > 8:
        #    angular_z = 8
        #if angular_z > -8:
        #    angular_z = -8
        print ("maju")

        goal_linear_x_velocity = linear_x  
        goal_angular_velocity = angular_z/4
        goal_linear_y_velocity = linear_y 
	#if abs(degree) > angle_tolerance and ry < 120:
	 #   goal_angular_velocity = degree /4  
	 #   goal_linear_y_velocity = 0 
	if ry < 90 and ry > 50:
            #print "yok muter yok"
           # if abs(degree) > angle_tolerance:
            goal_angular_velocity = degree / 5
            goal_linear_x_velocity = Vx/3
            goal_linear_y_velocity = Vy  /3

        if ry < 0 and abs(degree) > 30 :
            goal_angular_velocity = angular_z 
            goal_linear_x_velocity = linear_x  
            print "balik kanan"
        #else:
	 #   goal_linear_x_velocity = linear_x
	  #  goal_linear_y_velocity = linear_y
        v1 = (goal_linear_y_velocity * 0) + (goal_linear_x_velocity * (1 / WHEEL_RADIUS)) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
        v2 = (goal_linear_y_velocity * (sqrt(3) / (2 * WHEEL_RADIUS))) + (goal_linear_x_velocity * (-1 / (2 * WHEEL_RADIUS))) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
        v3 = (goal_linear_y_velocity * (sqrt(3) / (-2 * WHEEL_RADIUS))) + (goal_linear_x_velocity * (-1 / (2 * WHEEL_RADIUS))) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
        for i in range(len(PWM_Out)):
            if PWM_Out[i] > LIMIT_X_MAX_VALUE:
                PWM_Out[i] = LIMIT_X_MAX_VALUE
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
        PWM_Out[0] = (v1 * pwmMax) 
        PWM_Out[1] = (v2 * pwmMax) 
        PWM_Out[2] = (v3 * pwmMax) 
 

                #print (PWM_Out[0],PWM_Out[1],PWM_Out[2])
        return (PWM_Out[0] ,PWM_Out[1],PWM_Out[2] )
                
def Kalman(degree):
        i     = 0
        lXval = i
        lPval = 1.0
        Zval  = 0
        Rval  = 0.1
        tmp   = 0
        steps = 0
        Kval  = 0
        Pval  = 0
        Xval  = 0

        Zval = degree
        Pval = lXval
        Pval = lPval + lPval / 10

        Kval = Pval / (Pval + Rval)

        Xval = Xval + Kval * (Zval - Xval)
        Pval = (1-Kval) * Pval

        lXval = Xval
        lPval = Pval

        if Xval != Zval:
                steps += 1
        if tmp != Zval:
                tmp = Zval

        return Xval              
