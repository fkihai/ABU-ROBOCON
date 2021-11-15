'''
Release : 19 Juni 2019

'''
import netCore as nc
from time import sleep
from math import *
import threading as th
from time import time,sleep,localtime
import copy

data = {}
loop = False

CAM_RES = [1280, 720]

LOW_SPEED = 50
MED_SPEED = 80
FAST_SPEED = 100


# STRATEGY

HOME_POS = {
	'r2': [0,250,0],
	'r3': [600,250, 0]
}

DROP_BALL_RANGE = {
	'r2': 600,
	'r3': 600
}

KICK_OFF_RANGE = {
	'r2': 61,
	'r3': 79
}


# IP BaseStation
nc.__init('10.10.10.3', 'bt') 
nc.subscribe('r2')
nc.subscribe('r3')

# def start(rID):
# 	print "Manager: start"
# 	mThread = th.Thread(target=main, args=(rID,))
# 	mThread.daemon = True
# 	mThread.start()

def whileNe(rID, data, rStatus):
	global loop


	loop = True
	nc.clearData()

	sTime = time()
	while True:
		msg = nc.readData(rID)
		if msg != None:
			sTime = time()
			if msg['status'] == rStatus:
				break
		if (time() - sTime) > 3:
			print rID, "Offline"
			break
		sleep(0.01)


def r3Home():
	print "Manager: 3Home"

	global loop

	loop = False
	data = {}
	data['r3'] = {'CMD': 'Setxyt', 'pos': HOME_POS['r3']}
	nc.publish(data)

def r2Home():
	print "Manager: r2Home"

	global loop

	loop = False
	data = {}
	data['r2'] = {'CMD': 'Setxyt', 'pos': HOME_POS['r2']}
	nc.publish(data)

def setHome():

	global loop
	loop = False

	data = {}
	data['r3'] = {'CMD': 'Setxyt', 'pos': HOME_POS['r3']}
	data['r2'] = {'CMD': 'Setxyt', 'pos': HOME_POS['r2']}
	nc.publish(data)
	print "R3 Home : ", HOME_POS['r3']
	print "R2 Home : ", HOME_POS['r2']

def engineOff():
	print "Manger: Engine Off"
	data = {}
	data['r2'] = {'CMD':'EngineOff'}
	data['r3'] = {'CMD':'EngineOff'}

	nc.publish(data)

def backHome():

	# Normal Back Home
	print "Manager: backHome"
	global loop
	loop = False
	data['r2'] = {'CMD': 'Goxyt', 'pos': HOME_POS['r2'], 'speed':FAST_SPEED}
	data['r3'] = {'CMD': 'Goxyt', 'pos': HOME_POS['r3'], 'speed':FAST_SPEED}
	nc.publish(data)

def Stop():
	print "Manager: Stop"

	global loop
	loop = False
	data = {}
	data['r2'] = {'CMD':'Stop'}
	data['r3'] = {'CMD':'Stop'}
	nc.publish(data)


# ------------------function Trial------------------#

def trialKick(rID, pwm):
	data = {}

	print 'manajer : ' + rID + ' kick' + str(pwm)

	if rID == 'both' :
		data['r2'] = {'CMD':'Kick','pwm' : pwm}
		data['r3'] = {'CMD':'Kick','pwm' : pwm}
		nc.publish(data)
		
	else :
		data[rID] = {'CMD':'Kick','pwm' : pwm}
		nc.publish(data)

def trialTracking(rID, ballRange) :
	data = {}

	try :

		print 'manajer : ' + rID + ' tracking' + '(' + ballRange + ')'

		if rID == 'r2' :
			data['r2'] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': ballRange, 'drib':False}	
			nc.publish(data)

		elif rID == 'r3' :
			data['r3'] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': ballRange, 'drib':False}
			nc.publish(data)
	
	except KeyboardInterrupt :
		print 'break'
		pass
	except :
		print 'force close'


def trialDribble(rID) :
	data = {}

	try :

		print 'manajer : ' + rID + ' dribble'

		if rID == 'both' :
			data['r2'] = {'CMD': '', 'drib': True}
			data['r3'] = {'CMD': '', 'drib': True}
			nc.publish(data)
			
		else :
			data[rID] = {'CMD': '', 'drib': True}
			nc.publish(data)
	except KeyboardInterrupt :
		print 'break'
		pass
	except :
		print 'force close'

def trialXYT(rID, posX, posY, posT, speed) :
	data = {}
	currentSpeed = 0

	try :
		if speed == 'Low' :
			currentSpeed = LOW_SPEED
		elif speed == 'Med' :
			currentSpeed = MED_SPEED
		elif speed == 'Fast':
			currentSpeed = FAST_SPEED

		print 'manajer : ' + rID + '(' + str(posX) + ', ' + str(posY) + ', ' + str(posT) + ', ' + str(currentSpeed) + ')'

		if rID == 'r3' :
			data['r3'] = {'CMD': 'Goxyt', 'pos': [posX, posY, posT], 'speed': currentSpeed}
			nc.publish(data)
		elif rID == 'r2':
			data['r2'] = {'CMD': 'Goxyt', 'pos': [posX, posY, posT], 'speed': currentSpeed}
			nc.publish(data)

		else :
			print 'manajer : error'

			
	except KeyboardInterrupt :
		print 'break'
		pass

	except :
		print 'force close'


# --------- STRATEGI SATU ----------- #

def strategi1():

	# Parameter
	KO_POS_START = {
		'r3': [600,471,90],
		'r2': [17,461,-95]
	}

	KO_POS_START1 = {
		'r2': [270,450,-82],
		'r3': [500,461,90]
	}

	KO_POS_START2 = {
		'r3': [400,461,85],
		'r2': [270,350,-45]
	}

	KO_POS_Kiper = [250,0,0]

	try:
		print "Bismillahirrohmanirrohim............."
		print "Mugo - Mugo gak error"

		Rid = 'r3'
		Lid = 'r2'

		global loop
		global lkick
		loop = False

		print Lid + " Ambil bola dan " + Rid +  " putar hadap terima bola"
		data = {}
		data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':MED_SPEED}
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
		nc.publish(data)


		while True:
			# read XYT
			pos  = nc.readData(Lid)['pos']
			Node = KO_POS_START[Lid]	
			if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5 :  
				break

		
		# R2 tracking
		sleep(0.2)
		print Lid + " Tracking"
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START1[Rid], 'speed':MED_SPEED}
		data[Lid] = {'CMD': 'Track', 'speed':FAST_SPEED, 'range': KICK_OFF_RANGE[Lid] , 'drib': True}
		nc.publish(data)
		print 'Masih Tracking'
		whileNe(Lid, data, 'Tracking Finish' )

		# Lid = 'r2' Umpan satu dua kepada r3
		# Rid = 'r3' Terima umpan dua
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					print  Lid + " Belum Kick"
					Node = KO_POS_START1[Lid]

					# r3 Gedek gedek
					data[Rid] = {'CMD': 'Tracking', 'speed':FAST_SPEED, 'range': 500 , 'drib': True}
					nc.publish(data)
					# Lid = 'r2' jika sudah memenuhi pos untuk umpan maka tendang
					# Rid = 'r3' terima umpan dari r2
					if pos[2] >= Node[2] - 30 and pos[2] <= Node[2] + 30 :
						# iki edit
						sleep(1)
						print "Kick"
						data[Lid] = {'CMD':'Kick','pwm' : 80}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						

						# Rid = "r3" Terima Umpan 
						# Lid = "r2" pergi ke posisi untuk terima umpan assis
						print "r3 Terima umpan Satu dua"
						data[Rid] = {'CMD': 'Tracking', 'speed':LOW_SPEED, 'range': KICK_OFF_RANGE[Rid] , 'drib': True}
						data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START2[Lid], 'speed':LOW_SPEED}
						nc.publish(data)
						whileNe(Rid,data,'Tracking Finish')

						# rid = "R3" selesai tracking
						# Lid = "r2" Sudah ke posisi untuk terima umpan assis 
						while True:
							# read XYT
							pos  = nc.readData(Lid)['pos']
							Node = KO_POS_START2[Lid]	
							if pos[1] >= Node[1] - 5 and pos[1] <= Node[1] + 5 :  
								break

						# Rid = "r3" pergi ke posisi umpan assis
						# Lid = "r2" gedek - gedek
						print "menuju pos untuk umpan"
						sleep(1)
						KO_POS_START[Rid][2] = 132
						KO_POS_START[Rid][0] = 500
						pos    = nc.readData(Rid)['pos']
						pos[2] = 132 
						data[Rid] = {'CMD': 'Goxyt', 'pos':pos, 'speed':FAST_SPEED}
						# data[Lid] = {'CMD': 'Tracking', 'speed':MED_SPEED, 'range': 300, 'drib': True}
						nc.publish(data)
						print "break"
						break

					else:
						# Lid = "r2" pergi ke posisi umpan satu dua / r3
						# Rid = "r3" gedek - gedek                  /r2
						print  Lid + " Belum Kick dan " + Rid + " tracking" 
						print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
						# KO_POS_START1[Lid][2] = -100
						pos[2] = -100
						data[Lid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':pos}
						data[Rid] = {'CMD': 'Tracking', 'speed':FAST_SPEED, 'range': 500 , 'drib': True}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Tracking', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)
						
				
		while True:
			# Tedang assist 
			pos  = nc.readData(Rid)['pos']
			Node = KO_POS_START[Rid]
			print "Target: ",KO_POS_START[Rid], "Heading: ", pos
			if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
				data[Lid] = {'CMD': 'Tracking', 'speed':FAST_SPEED, 'range': 300, 'drib': False}
				nc.publish(data)
				sleep(3)
				print Rid + "Tendang Assist"
				data[Rid] = {'CMD':'Kick','pwm' : 20}
				nc.publish(data)
				whileNe(Rid, data, 'Kick')
				break

		# tracking untuk goal
		lkick = KO_POS_Kiper
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					heading = -1 * degrees(atan2(lkick[0] - pos[0], lkick[1] - pos[1]))
					
					if pos[2] >= (heading - 7) and pos[2] <= (heading + 7):
						print "Kick"
						print "Target: ",heading, "Heading: ",pos[2]
						data[Lid] = {'CMD':'Kick','pwm' : 90}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						break
					else:
						print  Lid + "Belum Kick"
						print "Target: ",heading, "Heading: ",pos[2] 					
						pos[2] = heading
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':pos}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Lid], 'drib': True}
					nc.publish(data)

		print "Selesai......."				
	
	except KeyboardInterrupt:
		print "break"
# --------- STRATEGI SATU ----------- #



# --------- STRATEGI DUA ----------- #
def strategi2():

	KO_POS_START = {
		'r3': [400,80,0],
		'r2': [17,461,-90]
	}

	KO_POS_START1 = {
		'r2': [350,305,-177]
	}
	
	KO_POS_START2 = {
		'r3': [400,130,83],
		'r2': [157,37,-81]
	}


	KO_POS_START3 = {
		'r2': [50,100,-90]
	}

	KO_POS_Kiper = [300,0,0]

	try:
		print "Bismillahirrohmanirrohim............."
		print "Mugo - Mugo gak error"

		Rid = 'r3'
		Lid = 'r2'

		global loop
		global lkick
		loop = False

		print Lid + " Ambil bola dan " + Rid +  " putar hadap terima bola"
		data = {}
		# Rid = 'r3' => Pergi ke pos 1 untuk terima umpan bola 
		# Lid = 'r2' => ambil bola pada titik pusat		
		data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':MED_SPEED}
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
		nc.publish(data)


		while True:
			# kondisi r2 berhenti untuk tracking ambil bola
			pos  = nc.readData(Lid)['pos']
			Node = KO_POS_START[Lid]	
			if pos[2] > Node[2] - 2 and pos[2] < Node[2] + 2 :  
				break

		
		# r2 tracking ambil bola
		sleep(0.2)
		print Lid + " Tracking"
		data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Lid] , 'drib': True}
		nc.publish(data)
		whileNe(Lid, data, 'Tracking Finish' )
		
		# lkick = KO_POS_START[Rid]
		# lkick = nc.readData(Rid)['pos']

		# opsi 1 pertama
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			# lkick = nc.readData(Rid)['pos']
			nc.clearData()
			if rKick != None:
				# nc.clearData()
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					print  Lid + " Belum Kick"
					Node = KO_POS_START1[Lid]

					# jika r2 memenuhi untuk posisi umpan satu dua dan tendang
					if pos[2] > Node[2] - 4 and pos[2] < Node[2] + 4:
						sleep(1)
						print "Kick"
						data[Lid] = {'CMD':'Kick','pwm' : 80}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						sleep(0.5)
						# Rid = 'r3' terima bola dari umpan pertama
						# Lid = 'r2' menuju posisi pertama unutk menuju pos 2 untuk terima umpan assist
						print "Menuju Pos pertama Untuk menuju ke pos 2 Terima umpan assist"
						data[Rid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Rid] , 'drib': True}
						data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':LOW_SPEED}
						nc.publish(data)
						whileNe(Rid,data,'Tracking Finish')

						# r3 menuju pos terakir unutk umpan assis
						print "menuju pos untuk umpan"
						sleep(0.3)
						KO_POS_START2[Rid][2] = 93
						data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START2[Rid], 'speed':LOW_SPEED}
						nc.publish(data)
						print "break"
						break

					else:
						print  Lid + " Belum Kick"
						print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
						# r2 menuju pos untuk umpan satu dua
						# r3 gedek gedek
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':KO_POS_START1[Lid]}
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 500 , 'drib': True}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)
						# Siap Umpan Assist
		
		
		while True:
			# jika r2 sudah berhasil pos pertama maka menuju pos kedua untuk terima umpan assis
			pos1 = nc.readData(Lid)['pos']
			Node1 = KO_POS_START[Lid]
			print "Target_3: ",KO_POS_START[Lid], "Heading_3: ", pos1
			if pos1[0] >= Node1[0] - 4 and pos1[0] <= Node1[0] + 4 and pos1[1] >= Node1[1] - 4 and pos1[1] <= Node1[1] + 4 :
				print "Goxyt lagi dong"
				data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START3[Lid], 'speed':LOW_SPEED}
				nc.publish(data)
				break
		while True:
			pos1 = nc.readData(Lid)['pos']
			Node1 = KO_POS_START3[Lid]
			# kondisi berhenti untuk pos kedua terima umpan 
			print "Pos 3 akan Tercapai"
			print "Target: ",KO_POS_START3[Lid], "Heading: ", pos1
			if pos1[0] >= Node1[0] - 7 and pos1[0] <= Node1[0] + 7 and pos1[1] >= Node1[1] - 7 and pos1[1] <= Node1[1] + 7 :
				break		
		while True:
			data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 700 , 'drib': True}
			nc.publish(data)			
			pos  = nc.readData(Rid)['pos']
			# if pos == None:
			# 	continue
			Node = KO_POS_START2[Rid]
			print "Target: ",KO_POS_START2[Rid], "Heading: ", pos
			if pos[2] >= Node[2] - 3 and pos[2] <= Node[2] + 3:
				print Rid + "Tendang Assist"
				data[Rid] = {'CMD':'Kick','pwm' : 10}
				nc.publish(data)
				whileNe(Rid, data, 'Kick')
				break		

		# tracking untuk goal
		lkick = KO_POS_Kiper
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					heading = -1 * degrees(atan2(lkick[0] - pos[0], lkick[1] - pos[1]))
					# heading = -1 * lkick[2]				
					if pos[2] >= (heading - 4) and pos[2] <= (heading + 4):
						print "Kick"
						print "Target: ",heading, "Heading: ",pos[2]
						data[Lid] = {'CMD':'Kick','pwm' : 80}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						break
					else:
						print  Lid + "Belum Kick"
						print "Target: ",heading, "Heading: ",pos[2] 					
						pos[2] = heading
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':pos}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':FAST_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)

		print "Selesai......."
					
	except KeyboardInterrupt:
		print "break"

# --------- STRATEGI DUA ----------- #





# --------- STRATEGI TIGA ----------- #
def strategi3():

	KO_POS_START = {
		'r3': [400,200,0],
		'r2': [17,461,-90]
	}

	KO_POS_START1 = {
		'r2': [350,440,-177]
	}
	
	KO_POS_START2 = {
		'r3': [400,130,83],
		'r2': [157,37,-81]
	}


	KO_POS_START3 = {
		'r2': [50,100,-90]
	}

	KO_POS_Kiper = [350,0,0]

	try:
		print "Bismillahirrohmanirrohim............."
		print "Mugo - Mugo gak error"

		Rid = 'r3'
		Lid = 'r2'

		global loop
		global lkick
		loop = False

		print Lid + " Ambil bola dan " + Rid +  " putar hadap terima bola"
		data = {}
		# Rid = 'r3' => Pergi ke pos 1 untuk terima umpan bola 
		# Lid = 'r2' => ambil bola pada titik pusat
		data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':MED_SPEED}
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
		nc.publish(data)


		while True:
			# kondisi r2 berhenti untuk tracking ambil bola
			pos  = nc.readData(Lid)['pos']
			Node = KO_POS_START[Lid]	
			if pos[2] >= Node[2] - 4 and pos[2] <= Node[2] + 4 :  
				break

		
		# r2 tracking ambil bola
		sleep(0.2)
		print Lid + " Tracking"
		data[Lid] = {'CMD': 'Track', 'speed':FAST_SPEED, 'range': KICK_OFF_RANGE[Lid] , 'drib': True}
		nc.publish(data)
		print "Masih Tracking"
		whileNe(Lid, data, 'Tracking Finish' )
		
		
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			# lkick = nc.readData(Rid)['pos']
			nc.clearData()
			if rKick != None:
				# nc.clearData()
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					print  Lid + " Belum Kick"
					Node = KO_POS_START1[Lid]

					# jika r2 memenuhi untuk posisi umpan satu dua dan tendang
					print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
					if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
						sleep(1)
						print "Kick"
						data[Lid] = {'CMD':'Kick','pwm' : 70}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						sleep(0.5)					

						# Rid = 'r3' terima bola dari umpan pertama
						# Lid = 'r2' menuju posisi untuk terima umpan assist
						KO_POS_START1[Lid][1] = 305
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': KICK_OFF_RANGE[Rid] , 'drib': True}
						data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START1[Lid], 'speed':FAST_SPEED}
						nc.publish(data)
						print "Masih Tracking"
						whileNe(Rid,data,'Tracking Finish')

						# Rid = 'r3' menuju pos terakhir untuk umpan assis
						# Lid = 'r2' gedek gedek
						print "menuju pos untuk umpan"
						sleep(0.3)
						KO_POS_START[Rid][1] = 130
						# edit Ini Lid
						data[Lid] = {'CMD': 'Tracking', 'speed':MED_SPEED, 'range': 300 , 'drib': True}
						data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
						nc.publish(data)
						print "break"
						break

					else:
						print  Lid + " Belum Kick"
						print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
						# Lid = r2 menuju pos untuk umpan satu dua
						# Rid = r3 gedek gedek
						data[Lid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':KO_POS_START1[Lid]}
						data[Rid] = {'CMD': 'Tracking', 'speed':MED_SPEED, 'range': 500 , 'drib': True}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)
						
		while True:
			# robot Umpan
			KO_POS_START1[Lid][1] = 305
			pos1 = nc.readData(Lid)['pos']
			Node1 = KO_POS_START1[Lid]
			
			# robot Terima
			pos2 = nc.readData(Rid)['pos']
			Node2 = KO_POS_START[Rid]
				
			# print "Target_U: ",Node1, "Heading_U: ", pos1
			print "Target_T: ",Node2, "Heading_T: ", pos2
			print "akan Umpan satu dua"

			# Rid = jika r3 sudah posisi pas untuk umpan assis maka tendang
			# if pos1[0] >= Node1[0] - 8 and pos1[0] <= Node1[0] + 8 and pos1[1] >= Node1[1] - 8 and pos1[1] <= Node1[1] + 8   and pos2[0] >= Node2[0] - 6 and pos2[0] <= Node2[0] + 6 and pos2[1] >= Node2[1] - 6 and pos2[1] <= Node2[1] + 6  and pos2[2] >= Node2[2] - 3 and pos2[2] <= Node2[2] + 3  :
			if pos2[0] >= Node2[0] - 6 and pos2[0] <= Node2[0] + 6 and pos2[1] >= Node2[1] - 6 and pos2[1] <= Node2[1] + 6  and pos2[2] >= Node2[2] - 5 and pos2[2] <= Node2[2] + 5  :
				sleep(1)	
				print("Umpan satu dua")
				data[Rid] = {'CMD':'Kick','pwm' : 15}
				nc.publish(data)
				print "Kick Assis"
				whileNe(Rid, data, 'Kick')
				sleep(1)
				data[Rid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':HOME_POS[Rid]}
				nc.publish(data)
				break
			

		# r2 tracking unutk siap goalllllll.....!
		lkick = KO_POS_Kiper
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					heading = -1 * degrees(atan2(lkick[0] - pos[0], lkick[1] - pos[1]))
					# heading = -1 * lkick[2]				
					if pos[2] >= (heading - 6) and pos[2] <= (heading + 6):
						print "Kick"
						print "Target: ",heading, "Heading: ",pos[2]
						data[Lid] = {'CMD':'Kick','pwm' : 120}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						break
					else:
						print  Lid + "Belum Kick"
						print "Target: ",heading, "Heading: ",pos[2] 					
						pos[2] = heading
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':pos}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)

		print "Selesai......."				
	except KeyboardInterrupt:
		print "break"
	except :
		print "Force close"	

# --------- STRATEGI TIGA ----------- #



# --------- STRATEGI EMPAT ----------- #
def strategi4():

	KO_POS_START = {
		'r2': [160,80,0],
		'r3': [600,461,90]
	}
	#  ini
	KO_POS_START1 = {
		'r3': [170,440,183],
		'r2': [160,440,0]
	}
	
	KO_POS_START2 = {
		'r2': [400,130,83],
		'r3': [157,37,-81]
	}

	KO_POS_START3 = {
		'r3': [50,100,-90]
	}

	KO_POS_Kiper = [360,0,0]

	try:
		print "Bismillahirrohmanirrohim............."
		print "Mugo - Mugo gak error"

		Rid = 'r2'
		Lid = 'r3'

		global loop
		global lkick
		loop = False

		print Lid + " Ambil bola dan " + Rid +  " putar hadap terima bola"
		data = {}
		# Rid = 'r2' => Pergi ke pos 1 untuk terima umpan bola 
		# Lid = 'r3' => ambil bola pada titik pusat
		data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':MED_SPEED}
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
		nc.publish(data)


		while True:
			# kondisi r3 berhenti untuk tracking ambil bola
			pos  = nc.readData(Lid)['pos']
			Node = KO_POS_START[Lid]
			print "target: ", Node, "Heading: ", pos	
			if pos[2] >= Node[2] - 4 and pos[2] <= Node[2] + 4 :  
				break

		
		# r3 tracking ambil bola
		sleep(0.2)
		print Lid + " Tracking"
		data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Lid] , 'drib': True}
		nc.publish(data)
		print "Masih Tracking"
		whileNe(Lid, data, 'Tracking Finish' )
		
	
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			# lkick = nc.readData(Rid)['pos']
			nc.clearData()
			if rKick != None:
			
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					print  Lid + " Belum Kick"
					Node = KO_POS_START1[Lid]

					# jika r3 memenuhi untuk posisi umpan satu dua dan tendang
					if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
						sleep(1)
						print "Kick"
						data[Lid] = {'CMD':'Kick','pwm' : 20}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						sleep(0.5)

						# Rid = 'r2' terima bola dari umpan pertama
						# Lid = 'r3' menuju posisi untuk terima umpan assist

						KO_POS_START1[Lid][1] = 305
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': KICK_OFF_RANGE[Rid] , 'drib': True}
						data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START1[Lid], 'speed':FAST_SPEED}
						nc.publish(data)
						whileNe(Rid,data,'Tracking Finish')

						# Rid = 'r2' menuju pos terakhir untuk umpan assis
						# Lid = 'r3' gedek - gedek
						print "menuju pos untuk umpan"
						sleep(0.3)
						KO_POS_START[Rid][1] = 130
						# edit Ini Lid
						data[Lid] = {'CMD': 'Tracking', 'speed':LOW_SPEED, 'range': 300 , 'drib': True}
						data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
						nc.publish(data)
						print "break"
						break

					else:
						print  Lid + " Belum Kick"
						print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
						# r3 menuju pos untuk umpan satu dua
						# r2 gedek gedek
						data[Lid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':KO_POS_START1[Lid]}
						data[Rid] = {'CMD': 'Tracking', 'speed':LOW_SPEED, 'range': 300 , 'drib': True}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)
						
		
		while True:
			# robot Umpan
			KO_POS_START1[Lid][1] = 305
			pos1 = nc.readData(Lid)['pos']
			Node1 = KO_POS_START1[Lid]
			# robot Terima
			KO_POS_START[Rid][1] = 130
			pos2 = nc.readData(Rid)['pos']
			Node2 = KO_POS_START[Rid]
			# Rid = jika r3 sudah posisi pas untuk umpan assis maka tendang
			if pos2[0] >= Node2[0] - 9 and pos2[0] <= Node2[0] + 9 and pos2[1] >= Node2[1] - 9 and pos2[1] <= Node2[1] + 9 and pos2[2] >= Node2[2] - 6 and pos2[2] <= Node2[2] + 6  :
				print("Umpan Assis")
				sleep(1)
				data[Rid] = {'CMD':'Kick','pwm' : 80}
				nc.publish(data)
				whileNe(Rid, data, 'Kick')
				sleep(1)
				# r2 pergi home
				data[Rid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':HOME_POS[Rid]}
				nc.publish(data)
				break

		# r3 tracking unutk siap goalllllll.....!
		lkick = KO_POS_Kiper
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					heading = -1 * degrees(atan2(lkick[0] - pos[0], lkick[1] - pos[1]))
					# heading = -1 * lkick[2]				
					if pos[2] >= (heading - 6) and pos[2] <= (heading + 6):
						print "Kick"
						print "Target: ",heading, "Heading: ",pos[2]
						data[Lid] = {'CMD':'Kick','pwm' : 40}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						break
					else:
						print  Lid + "Belum Kick"
						print "Target: ",heading, "Heading: ",pos[2] 					
						pos[2] = heading
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':pos}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)

		print "Selesai......."				
	except KeyboardInterrupt:
		print "break"
		

# --------- STRATEGI EMPAT ----------- #



# --------- STRATEGI LIMA ----------- #
def strategi5():

	KO_POS_START = {
		'r3': [600,130,0],
		'r2': [17,461,-90]
	}

	KO_POS_START1 = {
		'r2': [300,450,-177],
		'r3': [350,130,0]     
	}
	
	KO_POS_START2 = {
		'r3': [400,130,83],
		'r2': [157,37,-81]
	}


	KO_POS_START3 = {
		'r2': [50,100,-90]
	}

	KO_POS_Kiper = [350,0,0]

	try:
		print "Bismillahirrohmanirrohim............."
		print "Mugo - Mugo gak error"

		Rid = 'r3'
		Lid = 'r2'

		global loop
		global lkick
		loop = False

		print Lid + " Ambil bola dan " + Rid +  " putar hadap terima bola"
		data = {}
		# Rid = 'r3' => Pergi ke pos L pertama
		# Lid = 'r2' => ambil bola pada titik pusat
		data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':MED_SPEED}
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
		nc.publish(data)

		while True:
			# kondisi r2 berhenti untuk tracking ambil bola
			pos  = nc.readData(Lid)['pos']
			Node = KO_POS_START[Lid]	
			if pos[2] >= Node[2] - 4 and pos[2] <= Node[2] + 4 :  
				break

		# r2 tracking ambil bola
		sleep(0.2)
		print Lid + " Tracking"
		data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Lid] , 'drib': True}
		nc.publish(data)
		whileNe(Lid, data, 'Tracking Finish' )

		# Rid = r3 pergi ke pos L terakhir
		print "masuk Posisi L terakhir"
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START1[Rid], 'speed':MED_SPEED}
		nc.publish(data)

		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			# lkick = nc.readData(Rid)['pos']
			nc.clearData()
			if rKick != None:
				# nc.clearData()
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					print  Lid + " Belum Kick"
					Node = KO_POS_START1[Lid]

					# jika r2 memenuhi untuk posisi umpan satu dua dan tendang
					print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
					if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
						sleep(1)
						print "Kick"
						data[Lid] = {'CMD':'Kick','pwm' : 30}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						sleep(0.5)					

						# Rid = 'r3' terima bola dari umpan pertama
						# Lid = 'r2' menuju posisi untuk terima umpan assist
						KO_POS_START1[Lid][1] = 305
						data[Rid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Rid] , 'drib': True}
						data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START1[Lid], 'speed':LOW_SPEED}
						nc.publish(data)
						whileNe(Rid,data,'Tracking Finish')

						# Rid = 'r3' menuju pos terakhir untuk umpan assis
						print "menuju pos untuk umpan"
						sleep(0.3)
						KO_POS_START1[Rid][1] = 130
						# edit Ini Lid
						data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 300 , 'drib': True}
						data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':LOW_SPEED}
						nc.publish(data)
						print "break"
						break

					else:
						print  Lid + " Belum Kick"
						print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
						# r2 menuju pos untuk umpan satu dua
						# r3 gedek gedek
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':KO_POS_START1[Lid]}
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 500 , 'drib': True}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)		

		while True:
			# robot Umpan
			KO_POS_START1[Lid][1] = 305
			pos1 = nc.readData(Lid)['pos']
			Node1 = KO_POS_START1[Lid]
			# robot Terima
			pos2 = nc.readData(Rid)['pos']
			Node2 = KO_POS_START[Rid]

			print "Target_U: ",Node1, "Heading_U: ", pos1
			print "Target_T: ",Node2, "Heading_T: ", pos2
			print "akan Umpan satu dua"

			# Rid = jika r3 sudah posisi pas untuk umpan assis maka tendang
			# if pos1[0] >= Node1[0] - 8 and pos1[0] <= Node1[0] + 8 and pos1[1] >= Node1[1] - 8 and pos1[1] <= Node1[1] + 8   and pos2[0] >= Node2[0] - 6 and pos2[0] <= Node2[0] + 6 and pos2[1] >= Node2[1] - 6 and pos2[1] <= Node2[1] + 6  and pos2[2] >= Node2[2] - 3 and pos2[2] <= Node2[2] + 3  :
			if pos2[0] >= Node2[0] - 6 and pos2[0] <= Node2[0] + 6 and pos2[1] >= Node2[1] - 6 and pos2[1] <= Node2[1] + 6  and pos2[2] >= Node2[2] - 3 and pos2[2] <= Node2[2] + 3  :
				sleep(1)	
				print("Umpan satu dua")
				data[Rid] = {'CMD':'Kick','pwm' : 20}
				nc.publish(data)
				print "Kick Assis"
				whileNe(Rid, data, 'Kick')
				sleep(1)
				data[Rid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':HOME_POS[Rid]}
				nc.publish(data)
				break
			


		# r2 tracking unutk siap goalllllll.....!
		lkick = KO_POS_Kiper
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					heading = -1 * degrees(atan2(lkick[0] - pos[0], lkick[1] - pos[1]))
					# heading = -1 * lkick[2]				
					if pos[2] >= (heading - 6) and pos[2] <= (heading + 6):
						print "Kick"
						print "Target: ",heading, "Heading: ",pos[2]
						data[Lid] = {'CMD':'Kick','pwm' : 30}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						break
					else:
						print  Lid + "Belum Kick"
						print "Target: ",heading, "Heading: ",pos[2] 					
						pos[2] = heading
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':pos}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':FAST_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)

		print "Selesai......."				
	except KeyboardInterrupt:
		print "break"


# --------- STRATEGI TIGA ----------- #
def strategi6():

	KO_POS_START = {
		'r3': [400,80,0],
		'r2': [17,461,-90]
	}

	KO_POS_START1 = {
		'r2': [350,440,-177]
	}
	
	KO_POS_START2 = {
		'r3': [400,130,83],
		'r2': [157,37,-81]
	}


	KO_POS_START3 = {
		'r2': [50,100,-90]
	}

	# KO_POS_Kiper = [300,0,0]
	KO_POS_Kiper = [350,0,0]

	try:
		print "Bismillahirrohmanirrohim............."
		print "Mugo - Mugo gak error"

		Rid = 'r3'
		Lid = 'r2'

		global loop
		global lkick
		loop = False

		print Lid + " Ambil bola dan " + Rid +  " putar hadap terima bola"
		data = {}
		# Rid = 'r3' => Pergi ke pos 1 untuk terima umpan bola 
		# Lid = 'r2' => ambil bola pada titik pusat
		data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Lid], 'speed':MED_SPEED}
		data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':MED_SPEED}
		nc.publish(data)


		while True:
			# kondisi r2 berhenti untuk tracking ambil bola
			pos  = nc.readData(Lid)['pos']
			Node = KO_POS_START[Lid]	
			if pos[2] >= Node[2] - 4 and pos[2] <= Node[2] + 4 :  
				break

		
		# r2 tracking ambil bola
		sleep(0.2)
		print Lid + " Tracking"
		data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Lid] , 'drib': True}
		nc.publish(data)
		whileNe(Lid, data, 'Tracking Finish' )
		
		
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			# lkick = nc.readData(Rid)['pos']
			nc.clearData()
			if rKick != None:
				# nc.clearData()
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					print  Lid + " Belum Kick"
					Node = KO_POS_START1[Lid]

					# jika r2 memenuhi untuk posisi umpan satu dua dan tendang
					print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
					if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
						sleep(1)
						print "Kick"
						data[Lid] = {'CMD':'Kick','pwm' : 50}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						sleep(0.5)					

						# Rid = 'r3' terima bola dari umpan pertama
						# Lid = 'r2' menuju posisi untuk terima umpan assist
						KO_POS_START1[Lid][1] = 305
						data[Rid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': KICK_OFF_RANGE[Rid] , 'drib': True}
						data[Lid] = {'CMD': 'Goxyt', 'pos':KO_POS_START1[Lid], 'speed':LOW_SPEED}
						nc.publish(data)
						whileNe(Rid,data,'Tracking Finish')

						# Rid = 'r3' menuju pos terakhir untuk umpan assis
						print "menuju pos untuk umpan"
						sleep(0.3)
						KO_POS_START[Rid][1] = 130
						# edit Ini Lid
						data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 300 , 'drib': True}
						data[Rid] = {'CMD': 'Goxyt', 'pos':KO_POS_START[Rid], 'speed':LOW_SPEED}
						nc.publish(data)
						print "break"
						break

					else:
						print  Lid + " Belum Kick"
						print "Target: ",KO_POS_START1[Lid], "Heading: ", pos
						# r2 menuju pos untuk umpan satu dua
						# r3 gedek gedek
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':KO_POS_START1[Lid]}
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 500 , 'drib': True}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)
						
		while True:
			# robot Umpan
			KO_POS_START1[Lid][1] = 305
			pos1 = nc.readData(Lid)['pos']
			Node1 = KO_POS_START1[Lid]
			# robot Terima
			pos2 = nc.readData(Rid)['pos']
			Node2 = KO_POS_START[Rid]

			print "Target_U: ",Node1, "Heading_U: ", pos1
			print "Target_T: ",Node2, "Heading_T: ", pos2
			print "akan Umpan satu dua"

			# Rid = jika r3 sudah posisi pas untuk umpan assis maka tendang
			# if pos1[0] >= Node1[0] - 8 and pos1[0] <= Node1[0] + 8 and pos1[1] >= Node1[1] - 8 and pos1[1] <= Node1[1] + 8   and pos2[0] >= Node2[0] - 6 and pos2[0] <= Node2[0] + 6 and pos2[1] >= Node2[1] - 6 and pos2[1] <= Node2[1] + 6  and pos2[2] >= Node2[2] - 3 and pos2[2] <= Node2[2] + 3  :
			if pos2[0] >= Node2[0] - 6 and pos2[0] <= Node2[0] + 6 and pos2[1] >= Node2[1] - 6 and pos2[1] <= Node2[1] + 6  and pos2[2] >= Node2[2] - 3 and pos2[2] <= Node2[2] + 3  :
				sleep(1)	
				print("Umpan satu dua")
				data[Rid] = {'CMD':'Kick','pwm' : 15}
				nc.publish(data)
				print "Kick Assis"
				whileNe(Rid, data, 'Kick')
				sleep(1)
				data[Rid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':HOME_POS[Rid]}
				nc.publish(data)
				break
			


		# r2 tracking unutk siap goalllllll.....!
		lkick = KO_POS_Kiper
		print Lid + " persiapan umpan"
		while True:
			rKick = nc.readData(Lid)
			nc.clearData()
			if rKick != None:
				if rKick['track'] == 0: # data sensor proximity
					# data = {}
					pos = rKick['pos']
					heading = -1 * degrees(atan2(lkick[0] - pos[0], lkick[1] - pos[1]))
					# heading = -1 * lkick[2]				
					if pos[2] >= (heading - 6) and pos[2] <= (heading + 6):
						print "Kick"
						print "Target: ",heading, "Heading: ",pos[2]
						data[Lid] = {'CMD':'Kick','pwm' : 120}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						break
					else:
						print  Lid + "Belum Kick"
						print "Target: ",heading, "Heading: ",pos[2] 					
						pos[2] = heading
						data[Lid] = {'CMD':'Goxyt', 'speed':LOW_SPEED, 'pos':pos}
						nc.publish(data)
				else:
					data[Lid] = {'CMD': 'Track', 'speed':FAST_SPEED, 'range':KICK_OFF_RANGE[Lid], 'drib':True}
					nc.publish(data)

		print "Selesai......."				
	except KeyboardInterrupt:
		print "break"

# --------- STRATEGI TIGA ----------- #





# --------- Trial Umpan Satu Dua----------#

def TrialUmpan(ID):

	try:
		if ID == 'r2':
			Lid = 'r2'
			Rid = 'r3'
		else:
			Lid = 'r3'
			Rid = 'r2'		

		print "Start Passing " + ID

		START = {
			Rid: [300,100,0],
			Lid: [300,350,170]
		}

		KICK = {
			Rid: 30,
			Lid: 50	
		}

		RANGE = {
			Lid: 76,
			Rid: 79
		}


		while True:
			rKick = nc.readData(Lid)
			nc.clearData(Lid)
			# print rKick	

			if rKick != None:
				if rKick['track'] == 0:
					pos  = rKick['pos']
					Node = START[Lid]

					print "Target: ",Node, "Heading: ", pos
					if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
						sleep(1)
						print Rid + ' Kick '
						data[Lid] = {'CMD':'Kick','pwm' : KICK[Lid]}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						data[Rid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': RANGE[Rid] , 'drib': True}
						nc.publish(data)
						break

					else:
						print Rid + " belum Kick"
						print "Target: ",Node, "Heading: ", pos
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 500 , 'drib': True}
						data[Lid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':START[Lid] }
						nc.publish(data)

				else:
					print Rid + " Tracking"
					data[Rid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':START[Rid] }
					data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range':RANGE[Lid], 'drib':True}
					nc.publish(data)

		if ID == 'r3':
			Lid = 'r3'
			Rid = 'r2'
		else:
			Lid = 'r2'
			Rid = 'r3'

		while True:
			rKick = nc.readData(Lid)
			nc.clearData(Lid)
			# print rKick	

			if rKick != None:
				if rKick['track'] == 0:
					pos  = rKick['pos']
					Node = START[Lid]

					print "Target: ",Node, "Heading: ", pos
					if pos[2] >= Node[2] - 5 and pos[2] <= Node[2] + 5:
						sleep(1)
						print Lid + ' Kick '
						data[Lid] = {'CMD':'Kick','pwm' : KICK[Lid]}
						nc.publish(data)
						whileNe(Lid, data, 'Kick')
						data[Rid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range': RANGE[Rid] , 'drib': True}
						nc.publish(data)
						break

					else:
						print Lid + " belum Kick"
						print "Target: ",Node, "Heading: ", pos
						data[Rid] = {'CMD': 'Track', 'speed':LOW_SPEED, 'range': 500 , 'drib': True}
						data[Lid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':START[Lid] }
						nc.publish(data)

				else:
					print Lid + " Tracking"
					data[Rid] = {'CMD':'Goxyt', 'speed':MED_SPEED, 'pos':START[Rid] }
					data[Lid] = {'CMD': 'Track', 'speed':MED_SPEED, 'range':RANGE[Lid], 'drib':True}
					nc.publish(data)

	except KeyboardInterrupt:
		print "Break"
		pass

	except :
		print "Force Close"					
