from math import *
import vision as vs
import engine as en


robotId = 2
pwmMax = 4
def speed_out(rx,ry):
    r = int(sqrt((rx**2)+(ry**2)))
    tetha = atan2(rx,ry)
    degree = int(degrees(tetha))
    jarakPixel = 0
    degree_tolerance = 4
    Vx = rx
    Vy = ry 
    jarakPixel = ((ry*ry)+(rx*rx))**0.5
    jarakMeter = FittingJarakKeMeter(jarakPixel)
    print  ('rx', rx, 'ry', ry, 'degreee', degree)
    print "jarakPixel = ", jarakPixel, "jarakMeter = ", jarakMeter
    
    goal_linear_x_velocity = 0.0
    goal_linear_y_velocity = 0.0
    goal_angular_velocity = 0.0
    
    linear_x = 0
    linear_y = 0
    angular_z = 0
    
    WHEEL_RADIUS = 0.029 # in meter
    DISTANCE_CENTER_TO_WHEEL = 0.1064 # in meter 
    
    MAX_LINEAR_VELOCITY = 0.22 #0.22 m/s
    MAX_ANGULAR_VELOCITY = 2.85 #m/s
    VELOCITY_LINEAR_X = 0.2 #m/s
    VELOCITY_LINEAR_Y = 0.02
    VELOCITY_ANGULAR_Z = 0.02

    SCALE_VELOCITY_LINEAR_X = 1
    SCALE_VELOCITY_LINEAR_Y = 1
    SCALE_VELOCITY_ANGULAR_Z = 1
    RPM_CONSTANT_VALUE = 0.229
    
    LIMIT_X_MAX_VALUE = 480

    #set arah tracking
    if abs(degree) > 8:
        angular_z = degree/4  
	linear_x = 0
	linear_y =0
        print "edek"
    elif  rx > 1:
        print "KANAN"
        linear_x += SCALE_VELOCITY_LINEAR_X *  VELOCITY_LINEAR_X * 5
	linear_y = 0
	angular_z = 0 
    elif rx < -1:
        linear_x -= SCALE_VELOCITY_LINEAR_X *  VELOCITY_LINEAR_X * 5
	angular_z = 0
	linear_y =0 
#     
#   if rx == 0:
#         linear_x =  0.03
#    ## elif abs(degree) >= degree_tolerance  :
    #    angular_z = degree/45
#	print("wayahe muter")
 #       if angular_z == 0:
  #     	    angular_z = 1.5
   # 	    linear_y = 1.5
    else:
	angular_z = 0
	linear_x = 0
	linear_y = 0
    #stabilkan kecepatan x,z
    if linear_x > MAX_LINEAR_VELOCITY:
        linear_x = MAX_LINEAR_VELOCITY
    if angular_z > MAX_ANGULAR_VELOCITY:
        angular_z = MAX_ANGULAR_VELOCITY
    if linear_y > MAX_LINEAR_VELOCITY:
        linear_y = MAX_LINEAR_VELOCITY
        


    goal_linear_x_velocity = linear_x
    goal_angular_velocity = angular_z
    goal_linear_y_velocity = linear_y
    v1 = 0
    v2 = 0
    v3 = 0
    PWM_Out = [0]*3
    
    #rx = [ v1 * cos(tetha),v2 * cos(tetha),v3 * cos(tetha)]
    #ry = [ v1 * sin(tetha),v2 * sin(tetha),v3 * sin(tetha)]  
    
    v1 = (goal_linear_y_velocity * 0) + (goal_linear_x_velocity * (1 / WHEEL_RADIUS)) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
    v2 = (goal_linear_y_velocity * (sqrt(3) / (2 * WHEEL_RADIUS))) + (goal_linear_x_velocity * (-1 / (2 * WHEEL_RADIUS))) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
    v3 = (goal_linear_y_velocity * (sqrt(3) / (-2 * WHEEL_RADIUS))) + (goal_linear_x_velocity * (-1 / (2 * WHEEL_RADIUS))) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
   


    PWM_Out[0] = (v1 * pwmMax)
    PWM_Out[1] = (v2 * pwmMax)
    PWM_Out[2] = (v3 * pwmMax)

    if PWM_Out[0] > LIMIT_X_MAX_VALUE:
        PWM_Out[0] = LIMIT_X_MAX_VALUE
    elif PWM_Out[0] < -LIMIT_X_MAX_VALUE:
        PWM_Out[0] = -LIMIT_X_MAX_VALUE
    if PWM_Out[1] > LIMIT_X_MAX_VALUE:
        PWM_Out[1] = LIMIT_X_MAX_VALUE
    elif PWM_Out[1] < -LIMIT_X_MAX_VALUE:
        PWM_Out[1] = -LIMIT_X_MAX_VALUE
    if PWM_Out[2] > LIMIT_X_MAX_VALUE:
       PWM_Out[2] = LIMIT_X_MAX_VALUE
    elif PWM_Out[2] < -LIMIT_X_MAX_VALUE:
        PWM_Out[2] = -LIMIT_X_MAX_VALUE    
    
    
    print (PWM_Out[0],PWM_Out[2],PWM_Out[1])
    return (PWM_Out[0],PWM_Out[2],PWM_Out[1])
    
    
def degree(x,y):
    pi = 22/7
    rad = (atan2(x,y))
    deg = int(rad*360)/(2/pi)
    return deg


def FittingJarakKeMeter(x):
    	#.....
	if (robotId==3): #R3
		xNol = x-73.0
		a = 6.236122109976270E01
		b = 3.114684996083170E-01
		c = -3.43859740306561E-03
		return (xNol/ (a+(b*xNol)+(c*(xNol**2.0))))
	elif (robotId==2): #R2
		#recipocal quadratic - YD
		xNol = x-68
		a =  8.80738541429166E01
		b =  6.616390346903918E-02
		c = -3.693755506247804E-03
		return (xNol/ (a+(b*xNol)+(c*(xNol**2.0))))
