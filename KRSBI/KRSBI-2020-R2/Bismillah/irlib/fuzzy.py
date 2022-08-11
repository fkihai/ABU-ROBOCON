FunctionMember = [-45, -5, 0, 5, 45] 

Fast = 3
Mid  = 2
LOW  = 1

def alpha(input):
    theta = [0,0,0,0,0]

    # Pengujian data kiri
    if input < FunctionMember[0]:
        theta[0] = 1
    elif input > FunctionMember[0] and input <= FunctionMember[1]:
        theta[0] = float(FunctionMember[1]-input)/float(FunctionMember[1]-FunctionMember[0])
    else :
        theta[0] = 0

    # pengujian data Kiri Sedang
    if input <= FunctionMember[1]:
        theta[1] = 0
    elif input > FunctionMember[0] and input <= FunctionMember[1]:
        theta[1] = float(input - FunctionMember[0])/float(FunctionMember[1]-FunctionMember[0])
    elif input > FunctionMember[1] and input <= FunctionMember[2]:
        theta[1] = float(FunctionMember[2] - input)/float(FunctionMember[2]-FunctionMember[1])
    else:
        theta[1] = 0             

    # pengujian data mid 
    if input <= FunctionMember[1]:
        theta[2] = 0
    elif input > FunctionMember[1] and input <= FunctionMember[2]:
        theta[2] = float(input - FunctionMember[1])/float(FunctionMember[2]-FunctionMember[1])
    elif input > FunctionMember[2] and input <= FunctionMember[3]:
        theta[2] = float(FunctionMember[3] - input)/float(FunctionMember[3]-FunctionMember[2])
    else:
        theta[2] = 0       

    # pengujian data Kanan Sedang
    if input <= FunctionMember[2]:
        theta[3] = 0
    elif input > FunctionMember[2] and input <= FunctionMember[3]:
        theta[3] = float(input - FunctionMember[2])/float(FunctionMember[3]-FunctionMember[2])
    elif input > FunctionMember[3] and input <= FunctionMember[4]:
        theta[3] = float(FunctionMember[4] - input)/float(FunctionMember[4]-FunctionMember[3])
    else:
        theta[3] = 0             

    # pengujian data kanan
    if input <= FunctionMember[3]:
        theta[4] = 0
    elif input > FunctionMember[3] and input <= FunctionMember[4]:
        theta[4] = float(input - FunctionMember[3])/float(FunctionMember[4] - FunctionMember[3])
    else:
        theta[4] = 1  

    return theta   


    
def Output(theta):

    Speed   = (theta[0] * Fast + theta[1] * Mid + theta[2] * LOW + theta[3] * Mid + theta[4] * Fast)
    theta   = theta[0] + theta[1] + theta[2] + theta[3] + theta[4]
    
    return Speed / theta
    



    
    