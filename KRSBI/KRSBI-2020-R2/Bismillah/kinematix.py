from math import *
#import omni as vs
import engine as en
from time import sleep
#id = []
pwmMax = 1
def speed_out(rx,ry):
    distance_tolerance = 15
    angle_tolerance = 10

    goal_linear_x_velocity = 0.0
    goal_linear_y_velocity = 0.0
    goal_angular_velocity = 0.0
    
    linear_x = linear_y = angular_z = 0
    
    
    WHEEL_RADIUS = 0.029 # in meter
    DISTANCE_CENTER_TO_WHEEL = 0.1064 # in meter 
    
    MAX_LINEAR_VELOCITY = 0.22 #0.22 m/s
    MAX_ANGULAR_VELOCITY = 2.85 #m/s
    VELOCITY_LINEAR_X = 0.02 #m/s
    VELOCITY_LINEAR_Y = 0.02
    VELOCITY_ANGULAR_Z = 0.02

    SCALE_VELOCITY_LINEAR_X = 1
    SCALE_VELOCITY_LINEAR_Y = 1
    SCALE_VELOCITY_ANGULAR_Z = 1
    RPM_CONSTANT_VALUE = 0.229
    
    LIMIT_X_MAX_VALUE = 480
    
    if linear_x > MAX_LINEAR_VELOCITY:
        linear_x = MAX_LINEAR_VELOCITY
    if angular_z > MAX_ANGULAR_VELOCITY:
        angular_z = MAX_ANGULAR_VELOCITY
    if linear_y > MAX_LINEAR_VELOCITY:
        linear_y = MAX_LINEAR_VELOCITY
    
    s = int(sqrt((rx**2)+(ry**2)))
    radian = atan2(rx,ry)
    degree = int(degrees(radian))

    if s <= distance_tolerance and abs(degree) <= angle_tolerance:
        linear_x = 0
        angular_z = 0
        linear_y = 0
        print "stop"
    else :
        linear_y = pwmMax * cos(radians(degree))
        linear_x = pwmMax * sin(radians(degree))

        goal_linear_x_velocity = linear_x
        goal_angular_velocity = angular_z
        goal_linear_y_velocity = linear_y
        
        v1 = v2 = v3 = 0
        vx = vy = w = 0
        PWM_Out = [0]*3

        v1 = (goal_linear_y_velocity * 0) + (goal_linear_x_velocity * (1 / WHEEL_RADIUS)) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
        v2 = (goal_linear_y_velocity * (sqrt(3) / (2 * WHEEL_RADIUS))) + (goal_linear_x_velocity * (-1 / (2 * WHEEL_RADIUS))) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
        v3 = (goal_linear_y_velocity * (sqrt(3) / (-2 * WHEEL_RADIUS))) + (goal_linear_x_velocity * (-1 / (2 * WHEEL_RADIUS))) + (goal_angular_velocity * (-DISTANCE_CENTER_TO_WHEEL/WHEEL_RADIUS))
        

        for i in range(len(PWM_Out)):
            if PWM_Out[i] > LIMIT_X_MAX_VALUE:
                PWM_Out[i] = LIMIT_X_MAX_VALUE
            
        PWM_Out[0] = (v1 * pwmMax)
        PWM_Out[1] = (v2 * pwmMax)
        PWM_Out[2] = (v3 * pwmMax)

        #en.setRPS(PWM_Out[0], PWM_Out[1], PWM_Out[2])
        print (PWM_Out[0],PWM_Out[1],PWM_Out[2])
        return (PWM_Out[0],PWM_Out[1],PWM_Out[2])

def degree(x,y):
    pi = 22/7
    rad = (atan2(x,y))
    deg = int(rad*360)/(2/pi)
    return deg

#output
# [0, -180, 180, 0, 360]
#input [1-360]
# 1-180= 0 
#180 - 270 = 180
#-180 - -270 = 180

#270 - 360 = 360
def degree(input):
    if input > 180 and input <= 270:
        output = 180
    return output

