
'''
Release : 14 Juni 2019 ..

'''
 ##:::'##:'########:::'######::'########::'####:
 ##::'##:: ##.... ##:'##... ##: ##.... ##:. ##::
 ##:'##::: ##:::: ##: ##:::..:: ##:::: ##:: ##::
 #####:::: ########::. ######:: ########::: ##::
 ##. ##::: ##.. ##::::..... ##: ##.... ##:: ##::
 ##:. ##:: ##::. ##::'##::: ##: ##:::: ##:: ##::
 ##::. ##: ##:::. ##:. ######:: ########::'####:

import sys
import socket
import irlib.netCore as nc
import irlib.manager as mn
from threading import Thread
from time import sleep
from bismillah import *
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
import struct

refbox = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
broker="192.168.0.107"  #ip basestation
# broker="127.0.0.1"  #ip basestation


nc.__init(broker, 'bt')

nc.subscribe('r2')
nc.subscribe('r3')


robot3 ={}
robot2 ={}
da= 20
data='0'
dataKirim='0'
Vcontrol = 'C'
form =[]
buff='0'
skorC=0
skorM=0
detik1=0
detik2=0
menit1=0
menit2=0
time="00:00"

img = None
team = None
strategi = None
# def DataControl():
# 	global data
# 	global dataKirim
# 	global refbox
# 	global Vcontrol
# 	global form
# 	global team
# 	global strategi
# 	payload = {}
# 	buff = None


# 	while True:
# 	#Receive Data from Refbox
# 		payload['ref'] = buff
# 		buff=refbox.recv(1).decode()
# 		print 'data refebox=',
# 		print buff
# 		print team
# 		if team == 'cyan':
# 			if buff =='K':
# 				if strategi == 'k1':
# 					mn.kickOff()
# 				elif strategi =='k2':
# 					mn.kickOff2()
# 				buff=refbox.recv(1).decode()
# 				if buff == '1':
# 					buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					if strategi == 'k1':
# 						mn.startKickOff('r2')
# 					elif strategi == 'k2':
# 						mn.startKickOff2('r2')
# 			if buff == 'F':
# 				mn.freeKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startFreeKick()
# 			if buff == 'G':
# 				gk=mn.goalKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startGoalKick(gk)
# 			if buff == 'T' :
# 				mn.throwIn()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startThrowIn()
# 			if buff == 'C':
# 				mn.corner()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startCorner()
# 			if buff == 'P':
# 				mn.penalty()
# 				buff=refbox.recv(1).decode()
# 				if buff == 's':
# 					mn.startPenalty()
# 			if buff == 'k':
# 				mn.OpKickOff()
# 				buff=refbox.recv(1).decode()
# 				if buff == '1':
# 					buff=refbox.recv(1).decode()
# 				print buff
# 				if buff =='s':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'f':
# 				mn.opFreeKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'g':
# 				mn.opGoalKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 't' :
# 				mn.opThrowIn()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'c':
# 				mn.opCorner()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'N':
# 				mn.DroppedBall('r2')
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff =='s':
# 					mn.start('r2')
# 			if buff == 'S':
# 				mn.Stop()
# 		elif team == 'magenta':
# 			if buff =='k':
# 				if strategi == 'k1':
# 					mn.kickOff()
# 				elif strategi =='k2':
# 					mn.kickOff2()
# 				buff=refbox.recv(1).decode()
# 				if buff == '1':
# 					buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					if strategi == 'k1':
# 						mn.startKickOff('r3')
# 					elif strategi =='k2':
# 						mn.startKickOff2('r3')
# 			if buff =='f':
# 				mn.freeKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startFreeKick()
# 			if buff == 'g':
# 				gk=mn.goalKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startGoalKick(gk)
# 			if buff == 't' :
# 				mn.throwIn()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startThrowIn()
# 			if buff == 'c':
# 				mn.corner()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.startCorner()
# 			if buff == 'p':
# 				mn.penalty()
# 				buff=refbox.recv(1).decode()
# 				if buff == 's':
# 					mn.startPenalty()
# 			if buff == 'K':
# 				mn.OpKickOff()
# 				buff=refbox.recv(1).decode()
# 				if buff == '1':
# 					buff=refbox.recv(1).decode()
# 				print buff
# 				if buff =='s':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'F':
# 				mn.opFreeKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'G':
# 				mn.opGoalKick()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'T' :
# 				mn.opThrowIn()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'C':
# 				mn.opCorner()
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff == 's':
# 					mn.MotionBall()
# 					mn.start('r2')
# 			if buff == 'N':
# 				mn.DroppedBall('r2')
# 				buff=refbox.recv(1).decode()
# 				print buff
# 				if buff =='s':
# 					mn.start('r2')
# 			if buff == 'S':
# 				mn.Stop()
#         #Send Data to Robot

# 		#dataKirim = buff
# 		#form.prinT()

def Timer():
	global menit1
	global menit2
	global detik1
	global detik2
	global time
	global form
	while True:
		sleep(1)
		detik1+=1
		if detik1 > 9:
		   detik1=0
		   detik2+=1
		   if detik2 > 5:
			   detik2=0
			   menit1+=1
			   if menit1 > 9:
				   menit1=0
				   menit2+=1
		time=("%s%s:%s%s" %(menit2,menit1,detik2,detik1))
		form.SetTime()


def koordinat():
	global x,y
	global form
	global robot1
	global robot2
	global robot3
	while True:
		robot1=nc.readData('r1')
		robot2=nc.readData('r2')
		robot3=nc.readData('r3')
		sleep(0.1)
		#form.SetTime()
		#form.SetInfo()
		form.lowSpeed()
		form.medSpeed()
		form.fastSpeed()
		if robot1 != None:
			form.Status()
		if robot2 != None:
			form.SetCap()
			form.SetPos()
			form.SetBall()
		if robot3 != None:
			form.SetCap3()
			form.SetPos3()
			form.SetBall3()


class Mainform(QMainWindow):
	def __init__(self, parent=None):
		QMainWindow.__init__(self, parent)
		self.ui = Ui_MainWindow()
		self.ui.setupUi(self)

		global time,x,y
		global img
		#refboxButton
		# self.ui.connectBtn_2.clicked.connect(self.connectBtnClick)
		# self.ui.disconnectBtn_2.clicked.connect(self.disconnectBtnClick)
		#
		# #rb_Cyan and Magenta
		# self.ui.rb_cyan.clicked.connect(self.rb_cyanClick)
		# self.ui.rb_magenta.clicked.connect(self.rb_magentaClick)
		
		#rb_KickOff
		# self.ui.rb_KickOff1.clicked.connect(self.rb_KickOff1Click)
		# self.ui.rb_KickOff2.toggled.connect(self.rb_KickOff2Click)
		# self.ui.startkickoff2.clicked.connect(self.StartKickOff2Click)
		
		#R2
		self.ui.pb_Home1.clicked.connect(self.pb_Home1Click)
		
		#R3
		self.ui.pb_Home2.clicked.connect(self.pb_Home2Click)
		
		#engine Off
		self.ui.pb_engineOff.clicked.connect(self.pb_engineOffClick)
		
		
		
		# #back Home
		# self.ui.pb_pulang.clicked.connect(self.pb_pulangClick)
		#
		# #ready
		# self.ui.pb_ready.clicked.connect(self.pb_readyClick)
		
		
		
		# #kickoff 2
		# self.ui.pb_kickoff.clicked.connect(self.pb_kickoffClick)
		#
		# #kickoff musuh
		# self.ui.pb_kickoffmusuh.clicked.connect(self.pb_OPkickoffClick)
		#
		# #droppedball
		# self.ui.pb_dropball.clicked.connect(self.pb_DroppedBallClick)
		#
		# #freeKick
		# self.ui.pb_FreeKick.clicked.connect(self.pb_FreeKickClick)
		# self.ui.startfreekick.clicked.connect(self.StartFreeKickClick)
		#
		# #goalKick
		# self.ui.pb_GoalKick.clicked.connect(self.pb_GoalKickClick)
		# self.ui.startgoalkick.clicked.connect(self.StartGoalKickClick)
		#
		# #throwIn
		# self.ui.pb_ThrowIn.clicked.connect(self.pb_ThrowInClick)
		# self.ui.startthrowin.clicked.connect(self.StartThrowInClick)
		#
		# #corner
		# self.ui.pb_corner.clicked.connect(self.pb_CornerClick)
		# self.ui.startcorner.clicked.connect(self.StartCornerClick)
		#
		# #penalty
		# self.ui.pb_penalty.clicked.connect(self.pb_PenaltyClick)
		# self.ui.startpenalty.clicked.connect(self.StartPenaltyClick)

		#set Home All Robots
		self.ui.pb_setHome.clicked.connect(self.setHomeAll)
		
		#start KickOff
		self.ui.rb_start.clicked.connect(self.rb_startClick)

		#stop
		self.ui.pb_stop.clicked.connect(self.pb_stopClick)

		#Back Home
		self.ui.pb_backHome.clicked.connect(self.pb_BackHomeClick)

		#Trial Kick
		self.ui.pb_trialSetKick.clicked.connect(self.pb_trialSetKickClick)

		#Trial GoXYT
		self.ui.pb_trialXYT.clicked.connect(self.pb_trialXYTClick)

		#Trial Tracking
		self.ui.pb_trialTracking.clicked.connect(self.pb_trialTrackingClick)

		#Trial Dribble
		self.ui.pb_trialDribble.clicked.connect(self.pb_trialDribbleClick)

		#Trial Passing
		self.ui.pb_trialPass.clicked.connect(self.pb_trialPassClick)






		sleep(2)
		thTimer = Thread(target=Timer)
		thKoordinat = Thread(target=koordinat)

		thTimer.daemon = True
		thKoordinat.daemon = True
		thTimer.start()
		thKoordinat.start()


	def SetTime(self):
		self.ui.game_time.setText(time)


	#Robot 2
	def SetCap(self):
		self.ui.lineEdit_cap2.setText(robot2['cap'].__str__())
	def SetPos(self):
		self.ui.lineEdit_pos2.setText(robot2['pos'].__str__())
	def SetBall(self):

		self.ui.lineEdit_ball2.setText(robot2['ball'].__str__())
		self.ui.lineEdit_ballPos2.setText(robot2['status'].__str__())


	#Robot 3
	def SetCap3(self):
		self.ui.lineEdit_cap3.setText(robot3['cap'].__str__())

	def SetPos3(self):
		self.ui.lineEdit_pos3.setText(robot3['pos'].__str__())


	def SetBall3(self):
		self.ui.lineEdit_ball3.setText(robot3['ball'].__str__())
		self.ui.lineEdit_ballPos3.setText(robot3['status'].__str__())

	#Speed
	def lowSpeed(self):
		self.ui.lineEdit_lowSpeed.setText(mn.LOW_SPEED.__str__())
	def medSpeed(self):
		self.ui.lineEdit_medSpeed.setText(mn.MED_SPEED.__str__())
	def fastSpeed(self):
		self.ui.lineEdit_fastSpeed.setText(mn.FAST_SPEED.__str__())



	# def drawline(self):
	# 	self.ui.field.drawline(10,100,0,100)
	def prinT (self):
		global skorC
		global skorM
		global dataKirim

		self.ui.monitor.addItem(dataKirim)
		if buff ==' A':
			skorC=skorC + 1
			self.ui.cyan_score.display(skorC)
		elif buff == 'a':
			skorM=skorM + 1
			self.ui.magen_score.display(skorM)



	# def connectBtnClick(self):
	# 	global refbox

	# 	#IP,PORT
	# 	IP   = ('%s' % self.ui.address_val_2.text())
	# 	PORT = (self.ui.port_val_2.value())
	# 	try:
	# 		refbox.connect((IP, PORT))
	# 		self.ui.status_val_2.setText('Connected')
	# 	except:
	# 		print("BaseStation terhubung")

	# 	ref = Thread(target=DataControl)
	# 	ref.daemon = True
	# 	ref.start()


	def setHomeAll(self) :
		print 'home R3 & R2'
		mn.r2Home()
		mn.r3Home()

	def pb_Home1Click(self):
		print 'home R2'
		mn.r2Home()

	def pb_Home2Click(self):
		print 'home R3'
		mn.r3Home()

	def pb_stopClick(self):
		self.ui.lineEdit_command.setText('stop')
		print 'stop'
		mn.Stop()

	def pb_BackHomeClick(self) :
		self.ui.lineEdit_command.setText('bismillah muleh')
		print "bismillah muleh"
		mn.backHome()
	
	def rb_startClick(self) :
		print "Robot START"
		dummy1 = self.ui.dummyBot1
		dummy2 = self.ui.dummyBot2
		dummy3 = self.ui.dummyBot3
		dummy4 = self.ui.dummyBot4
		dummy5 = self.ui.dummyBot5
		dummy6 = self.ui.dummyBot6
		dummy7 = self.ui.dummyBot7
		dummy8 = self.ui.dummyBot8


		#Chance dummybot
		if dummy1.isChecked() and dummy2.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 2')
			print ("Strategi 1")
			mn.strategi1()
		elif dummy1.isChecked() and dummy3.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 3')
			print ("Strategi 2")
			mn.strategi2()
		elif dummy1.isChecked() and dummy4.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 4')
			print '1 & 4'
		elif dummy1.isChecked() and dummy5.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 5')
			print '1 & 5'
		elif dummy1.isChecked() and dummy6.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 6')
			print '1 & 6'
		elif dummy1.isChecked() and dummy7.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 7')
			print '1 & 7'
		elif dummy1.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 1 & 8')
			print '1 & 8'

		elif dummy2.isChecked() and dummy3.isChecked() :
			self.ui.lineEdit_command.setText('dummy 2 & 4')
			print '2 & 3'
		elif dummy2.isChecked() and dummy4.isChecked() :
			self.ui.lineEdit_command.setText('dummy 2 & 4')
			print '2 & 4'
		elif dummy2.isChecked() and dummy5.isChecked() :
			self.ui.lineEdit_command.setText('dummy 2 & 5')
			print '2 & 5'
		elif dummy2.isChecked() and dummy6.isChecked() :
			self.ui.lineEdit_command.setText('dummy 2 & 6')
			print '2 & 6'
		elif dummy2.isChecked() and dummy7.isChecked() :
			self.ui.lineEdit_command.setText('dummy 2 & 7')
			print '2 & 7'
		elif dummy2.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 2 & 8')
			print '2 & 8'

		elif dummy3.isChecked() and dummy4.isChecked() :
			self.ui.lineEdit_command.setText('dummy 3 & 4')
			print '3 & 4'
		elif dummy3.isChecked() and dummy5.isChecked() :
			self.ui.lineEdit_command.setText('dummy 3 & 5')
			print '3 & 5'
		elif dummy3.isChecked() and dummy6.isChecked() :
			self.ui.lineEdit_command.setText('dummy 3 & 6')
			print '3 & 6'
		elif dummy3.isChecked() and dummy7.isChecked() :
			self.ui.lineEdit_command.setText('dummy 3 & 7')
			print '3 & 7'
		elif dummy3.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 3 & 8')
			print '3 & 8'

		elif dummy4.isChecked() and dummy5.isChecked() :
			self.ui.lineEdit_command.setText('dummy 4 & 5')
			print '4 & 5'
		elif dummy4.isChecked() and dummy6.isChecked() :
			self.ui.lineEdit_command.setText('dummy 4 & 6')
			print '4 & 6'
		elif dummy4.isChecked() and dummy7.isChecked() :
			self.ui.lineEdit_command.setText('dummy 4 & 7')
			print '4 & 7'
		elif dummy4.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 4 & 8')
			print '4 & 8'

		elif dummy5.isChecked() and dummy6.isChecked() :
			self.ui.lineEdit_command.setText('dummy 5 & 6')
			print '5 & 6'
		elif dummy5.isChecked() and dummy7.isChecked() :
			self.ui.lineEdit_command.setText('dummy 5 & 7')
			print '5 & 7'
		elif dummy5.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 5 & 8')
			print '5 & 8'

		elif dummy6.isChecked() and dummy7.isChecked() :
			self.ui.lineEdit_command.setText('dummy 6 & 7')
			print '6 & 7'
		elif dummy6.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 6 & 8')
			print '6 & 8'

		elif dummy7.isChecked() and dummy8.isChecked() :
			self.ui.lineEdit_command.setText('dummy 7 & 8')
			print '7 & 8'

		else :
			self.ui.lineEdit_command.setText('pilih posisi dummy')
			print 'pilih posisi dummy'
			# mn.Coba()
			# mn.tes1()

	def pb_engineOffClick(self):
		self.ui.lineEdit_command.setText('engine off')
		print 'Engine Off'
		mn.engineOff()

	def pb_trialSetKickClick(self):
		pwm = self.ui.lineEdit_trialSetKick.text()
		
		if self.ui.radio_trialR2.isChecked() :
			command = 'R2 Kick PWM = ' + pwm
			self.ui.lineEdit_command.setText(command)
			print "Trial Kick R2"
			mn.trialKick('r2', pwm)
			
		elif self.ui.radio_trialR3.isChecked() :
			command = 'R3 Kick PWM = ' + pwm
			self.ui.lineEdit_command.setText(command)
			print "Trial Kick R3"
			mn.trialKick('r3', pwm)

		elif self.ui.radio_trialBoth.isChecked() :
			command = 'Both Kick PWM = ' + pwm
			self.ui.lineEdit_command.setText(command)
			print "Trial Kick Both"
			mn.trialKick('both', pwm)
			
		else :
			command = 'Pilih Robot!'
			self.ui.lineEdit_command.setText(command)
			print "Pilih Robot!"

	def pb_trialXYTClick(self) :
		X = self.ui.lineEdit_trialSetX.text()
		Y = self.ui.lineEdit_trialSetY.text()
		T = self.ui.lineEdit_trialSetT.text()
		speed = self.ui.cb_speedXYT.currentText()

		if speed == 'speed' :
			command = 'pilih speed !!'
			self.ui.lineEdit_command.setText(command)
			print command
			
		else :
			if self.ui.radio_trialR2.isChecked() :
				command = 'R2(' + X + ', ' + Y + ', ' + T + ', ' + speed + ')'
				self.ui.lineEdit_command.setText(command)
				print command
				mn.trialXYT('r2', int(X), int(Y), int(T), speed)

			elif self.ui.radio_trialR3.isChecked() :
				command = 'R3(' + X + ', ' + Y + ', ' + T + ', ' + speed + ')'
				self.ui.lineEdit_command.setText(command)
				print command
				mn.trialXYT('r3', int(X), int(Y), int(T), speed)

			else :
				command = 'Pilih Robot'
				self.ui.lineEdit_command.setText(command)
				print command

	def pb_trialTrackingClick(self) :
		ballRange = self.ui.lineEdit_ballRange.text()

		if self.ui.radio_trialR2.isChecked() :
			command =  'R2 ' + ballRange
			self.ui.lineEdit_command.setText(command)
			print command
			mn.trialTracking('r2', int(ballRange))

		elif self.ui.radio_trialR3.isChecked() :
			command =  'R3 ' + ballRange
			self.ui.lineEdit_command.setText(command)
			print command
			mn.trialTracking('r3', int(ballRange))

		else :
			command = 'Pilih Robot !!'
			self.ui.lineEdit_command.setText(command)
			print command

	def pb_trialDribbleClick(self) :
		if self.ui.radio_trialR2.isChecked() :
			command = 'R2 setDribble'
			self.ui.lineEdit_command.setText(command)
			print command
			mn.trialDribble('r2')

		elif self.ui.radio_trialR3.isChecked() :
			command = 'R3 setDribble'
			self.ui.lineEdit_command.setText(command)
			print command
			mn.trialDribble('r3')

		elif self.ui.radio_trialBoth.isChecked() :
			command = 'Both setDribble'
			self.ui.lineEdit_command.setText(command)
			print command
			mn.trialDribble('both')

		else :
			command = 'Pilih Robot !!'
			self.ui.lineEdit_command.setText(command)
			print command

	def pb_trialPassClick(self) :
		if self.ui.radio_trialR2.isChecked() :
			command = 'R2 Start Passing'
			self.ui.lineEdit_command.setText(command)
			print command
			mn.TrialUmpan('r2')

		elif self.ui.radio_trialR3.isChecked() :
			command = 'R3 Start Passing'
			self.ui.lineEdit_command.setText(command)
			print command
			mn.TrialUmpan('r3')

		else :
			command = 'Pilih Robot'
			self.ui.lineEdit_command.setText(command)
			print command

	


if __name__ == "__main__":

        a = QApplication(sys.argv)
        form = Mainform()
        form.show()
        a.exec_()
