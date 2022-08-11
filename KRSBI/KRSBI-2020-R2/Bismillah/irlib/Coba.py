# from time import sleep
# a = 0
# while True:
#     a += 1
#     print(a)
#     sleep(1)
#     if(a == 5):
#         break
    

# print("fikri")

# nama = print("fikri")
# nama



# print (nama[0][0] + 10, nama[0][1]+ 10) 

# for i in range(len(nama)):
#     for j in range(1,3):
#         raw = 0
#         nama[i][j] + 10 
#         if(nama[i] == nama[j]){
#             a += 1
#             raw += 1
#             print(a)
#             continue
#         }


# for pos in range(len(nama)):
#     i = 0
#     k = 0
    
#     if()


    # i = 0
    # k = 1
    #     k += 1
    #     continue

    
    # i += 1
    # j += 1

# from time import sleep, time


# kp = 0.02
# ki = 0.01
# kd = 0.03

# LastError = 0

# degree = 16    
# setpoint = 0
# interval = 0

# while True:
#     if (time() - interval) > 0.05:

        
    
#         Error  = setpoint - (degree)
#         P = kp * Error

#         Integral = Error / 0.05
#         I = ki * Integral

#         Derivatif = (Error - LastError)
#         D = kd * Derivatif
#         LastError = Error
        
#         interval = time()
#         PID = I + D + I
        
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