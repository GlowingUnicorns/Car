import numpy as np
import cv2
import random
import os
os.chdir(r"C:/Users/lionm/OneDrive/Documents/Code/GUI")
def int_to_boolean_list(n):
    boolean_list = []
    while n > 0:
        bit = n & 1
        boolean_list.append(bool(bit))
        n >>= 1
    while(len(boolean_list) < 8):
        boolean_list.append(False)
    return boolean_list[::-1] if boolean_list else [False]

WHITE=(255,255,255);BLACK=(0,0,0);ORANGE=(0,125,255);BLUE=(255,0,0);GREEN=(0,255,0);RED=(0,0,255)
PURPLE = (255,0,255);GREY=(100,100,100);CYAN=(255,255,0)
title = 'Car GUI'
Gbackground = cv2.imread(r"./Assets/Background.png", cv2.IMREAD_UNCHANGED)

Cat = cv2.imread(r"./Assets/Cat.png", cv2.IMREAD_UNCHANGED)

height = Gbackground.shape[0]
width = Gbackground.shape[1]
Gbackground = cv2.cvtColor(Gbackground, cv2.COLOR_RGBA2RGB)
Cat = cv2.cvtColor(Cat, cv2.COLOR_RGB2RGBA)


#-----------------------------------------------------------------------------------------------------------------------------------------------------
#                                             InBuffer
#-----------------------------------------------------------------------------------------------------------------------------------------------------




    


batteryTemp= 24 #C
resCapacitymAh = 40 #mAh

auxVoltage = 100.0
auxCurrent = 12.2

sysVoltage = 11.8
rpm = 0.0

measureVoltage = 90
contactorState = True 
auxFeedState = False

packVoltage = 90.1
packCurrent = 0
packSOC = 95
maxCellmV = 4.15
maxCellVNum = 9
minCellmV = 4.11
minCellVNum = 5
cellDiff = 0.04

tempAverage = 0

chargeDischargeStatus = '0'
chargeFetState = False
disChargeFetState = False
bmsHeartBeat = 0
resCapacitymAh = 500

numberOfCells = 21
numOfTempSensors = 1
chargeState = False
loadState = True
bmsCycles = 255

cellVmV = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
batteryVoltage = cellVmV

cellTemperature = [0]

cellBalanceActive = False


coolingOK = False

AuxVoltageTooLow = False

ECU1Offline = False
ECU2Offline = False
ECU3Offline = False
ECU4Offline = False
ECU5Offline = False
ECU6Offline = False
alarms = [[0]*8,[0]*8,[0]*8,[0]*8]
temps = [0]*4

def updateVariables():
    inbuf = [0]*90  
    batteryTemp = inbuf[29]
    resCapacityMAh = inbuf[6]
    packVoltage = inbuf[0] + 0.01*inbuf[1]
    measureVoltage = packVoltage
    packCurrent = inbuf[4]*10 + inbuf[5]*0.1
    packSOC = inbuf[2] + 0.01*inbuf[3]
    maxCellmV = (inbuf[32]+200)*0.001
    maxCellVNum = inbuf[33]
    minCellmV = (inbuf[34]+200)*0.001
    minCellVNum = inbuf[35]
    cellDiff = inbuf[36]*0.001

    tempAverage = 0.25*(inbuf[40] +inbuf[41]+inbuf[42]+inbuf[43]) 
    alarms[0] = int_to_boolean_list(inbuf[48])
    alarms[1] = int_to_boolean_list(inbuf[49])
    alarms[2] = int_to_boolean_list(inbuf[50])
    alarms[3] = int_to_boolean_list(inbuf[51])
    temps[0] = inbuf[40]
    temps[1] = inbuf[41]
    temps[2] = inbuf[42]
    temps[3] = inbuf[43]

    resCapacityMah = inbuf[6]*100
    chargeDischargeStatus = inbuf[30]
    chargeFetState = int_to_boolean_list(inbuf[31])[0]
    disChargeFetState = int_to_boolean_list(inbuf[31])[1]
    chargeState = int_to_boolean_list(inbuf[31])[2]
    loadState = int_to_boolean_list(inbuf[31])[3]
    cellBalanceActive = int_to_boolean_list(inbuf[31])[4]
    bmsCycles = inbuf[7]
    for i in range(8,29):
        cellVmV = (inbuf[i]+200)*0.01
    detailii = [auxVoltage,auxCurrent,-1,sysVoltage,rpm,cellDiff,packCurrent,packVoltage]
    statii = [False,True,False,False,True,False,False,True,True]
    dynamicStatus = [0.0,0.0,0.0,0.0, 0,0,0, 12.5, 45, 0,2,3,4]

    NumAux = [12.4,123,30,0,0,5]
    batteryOK = True

    moduleStatii[0]=inbuf[60]
    moduleStatii[1]=inbuf[61]
    moduleStatii[2]=inbuf[62]
    moduleStatii[3]=inbuf[63]
    moduleStatii[4]=inbuf[64]
    moduleStatii[5]=inbuf[65]
    moduleStatii[6]=ECU6Offline
    moduleStatii[7]=(not batteryOK)
    moduleStatii[8]=contactorState
    moduleStatii[9]=auxFeedState
    
    statii3[0]
    statii3[1]
    statii3[2]
    statii3[3]
    statii3[4] = temps[0]
    statii3[5] = temps[1]
    statii3[6] = temps[2]
    statii3[7] = temps[3]
    
    boolii3 = [True,True,False,True]
    
    status3 = [False,False,False,False,False]
    levelStatus3[0]  = [alarms[0][0],alarms[0][1]]
    levelStatus3[1]  = [alarms[0][2],alarms[0][3]]
    levelStatus3[2]  = [alarms[0][4],alarms[0][5]]
    levelStatus3[3]  = [alarms[0][6],alarms[0][7]]
    levelStatus3[4]  = [alarms[1][0],alarms[1][1]]
    levelStatus3[5]  = [alarms[1][2],alarms[1][3]]
    levelStatus3[6]  = [alarms[1][4],alarms[1][5]]
    levelStatus3[7]  = [alarms[1][6],alarms[1][7]]
    levelStatus3[8]  = [alarms[2][0],alarms[2][1]]
    levelStatus3[9]  = [alarms[2][2],alarms[2][3]]
    levelStatus3[10] = [alarms[2][6],alarms[2][7]]
    levelStatus3[10] = [alarms[3][0],alarms[3][1]]
    
    pumping = False
    Fanning = False
    Running = False
    
    status4 = [True,pumping,contactorState,True,False]

    


failure = []
failure.append(("levelOneCellVoltageTooHigh",(0,0),1))
failure.append(("levelTwoCellVoltageTooHigh",(0,1),2))
failure.append(("levelOneCellVoltageTooLow",(0,2),1))
failure.append(("levelTwoCellVoltageTooLow",(0,3),2))

failure.append(("levelOnePackVoltageTooLow",(0,4),1))
failure.append(("levelTwoCellVoltageTooLow",(0,5),2))
failure.append(("levelOnePackVoltageTooHigh",(0,6),1))
failure.append(("levelTwoPackVoltageTooHigh",(0,7),2))

failure.append(("levelOneChargeTempTooHigh",(1,0),1))
failure.append(("levelTwoChargeTempTooHigh",(1,1),2))
failure.append(("levelOneChargeTempTooLow",(1,2),1))
failure.append(("levelTwoChargeTempTooLow",(1,3),2))
failure.append(("levelOneDischargeTempTooHigh",(1,4),1))
failure.append(("levelTwoDischargeTempTooHigh",(1,5),2))
failure.append(("levelOneDischargeTempTooLow",(1,6),1))
failure.append(("levelTwoDischargeTempTooLow",(1,7),2))

failure.append(("levelOneChargeCurrentTooHigh",(2,0),1))
failure.append(("levelTwoChargeCurrentTooHigh",(2,1),2))
failure.append(("levelOneDischargeCurrentTooHigh",(2,2),1))
failure.append(("levelTwoDischargeCurrentTooHigh",(2,3),2))
failure.append(("levelOneStateOfChargeTooHigh",(2,4),1))
failure.append(("levelTwoStateOfChargeTooHigh",(2,5),2))
failure.append(("levelOneStateOfChargeTooLow",(2,6),1))
failure.append(("levelTwoStateOfChargeTooLow",(2,7),2))

failure.append(("levelOneCellVoltageDifferenceTooHigh",(3,0),1))
failure.append(("levelTwoCellVoltageDifferenceTooHigh",(3,1),2))
failure.append(("chargeFETTemperatureTooHigh",(3,2),1))
failure.append(("dischargeFETTemperatureTooHigh",(3,3),1))

batteryOK = False

#Bottom Bar
NumAuxPosition = []
NumAuxPosition.append(("Voltage",(25,60),"v",0,))
NumAuxPosition.append(("Current",(175,60),"A",1))
NumAuxPosition.append(("Temp",(325,60),"C",2))
NumAuxPosition.append(("Fan",(475,60),"%",3))
NumAuxPosition.append(("Flush",(625,60),"A",4))
NumAuxPosition.append(("Charge",(775,60),"A",5))

NumAux = [12.4,123,30,0,0,5]

NumAuxPosition.append(("Voltage",(25,25),"v",0))
NumAuxPosition.append(("Current",(175,25),"A",1))
NumAuxPosition.append(("Temp",(325,25),"C",2))
NumAuxPosition.append(("Fan",(475,25),"%",3))
NumAuxPosition.append(("Descrepency",(625,25),"",4))

#-----------------------------------------------------------------------------------------------------------------------------------------------------
#                                             Screen 1
#-----------------------------------------------------------------------------------------------------------------------------------------------------
labelPosition = []
labelPosition.append((('Aux Pow'),(82,262) ,('Nominal','LOW')))
labelPosition.append((('Sec Sys'),(720,432),('Online','Offline')))
labelPosition.append((('Aux Sys'),(148,98) ,('Nominal','Fault')))
labelPosition.append((('Pwr Str'),(550,480),('Online','Offline')))
labelPosition.append((('Motor')  ,(838,350),('Nominal','Fault')))
labelPosition.append((('Bat Sys'),(840,55) ,('Nominal','Fault')))
labelPosition.append((('Drv Trn'),(410,105),('Online','Offline')))
labelPosition.append((('Drv Pow'),(500,50) ,('Nominal','Fault')))
labelPosition.append((('Pwr Brk'),(95,210) ,('Online','Offline')))
statii = [False,True,False,False,True,False,False,True,True]
#AUXPOW SECSYS AUXSYS POWSTR MOTOR BATSYS DRVTRN DRVPOW FTBRK

detailii = [auxVoltage,auxCurrent,-1,sysVoltage,rpm,cellDiff,packCurrent,packVoltage]
detailiiPosition = []
detailiiPosition.append((("v"),(72,292)))
detailiiPosition.append((("A"),(152,292)))
detailiiPosition.append((("GPS/Gyro/Accel"),(148,98)))
detailiiPosition.append((("v"),(720,462)))
detailiiPosition.append((("rpm"),(838,380)))
detailiiPosition.append((("v Diff"),(380,135)))
detailiiPosition.append((("A"),(480,135)))
detailiiPosition.append((("v"),(500,20)))

for i in range(len(batteryVoltage)):
   batteryVoltage[i] = 4.1

font = cv2.FONT_HERSHEY_DUPLEX
Gbase = cv2.imread(r"./Assets/Disp1/Base.png", cv2.IMREAD_UNCHANGED)
Gbattery = cv2.imread(r"./Assets/Disp1/Battery.png", cv2.IMREAD_UNCHANGED)
Gsteering = cv2.imread(r"./Assets/Disp1/Steering.png", cv2.IMREAD_UNCHANGED)
Ginverter = cv2.imread(r"./Assets/Disp1/Inverter.png", cv2.IMREAD_UNCHANGED)
Gdash = cv2.imread(r"./Assets/Disp1/Dash.png", cv2.IMREAD_UNCHANGED)
Gsecondary = cv2.imread(r"./Assets/Disp1/HandBrake.png", cv2.IMREAD_UNCHANGED)
GfootBrake = cv2.imread(r"./Assets/Disp1/FootBrake.png", cv2.IMREAD_UNCHANGED)
Gmotor = cv2.imread(r"./Assets/Disp1/Motor.png", cv2.IMREAD_UNCHANGED)
Gbattery2 = cv2.imread(r"./Assets/Disp1/Battery2.png", cv2.IMREAD_UNCHANGED)
GsideBar = cv2.imread(r"./Assets/Disp1/SideBar.png", cv2.IMREAD_UNCHANGED)
Glabel = cv2.imread(r"./Assets/Disp1/Labels.png", cv2.IMREAD_UNCHANGED)

#-----------------------------------------------------------------------------------------------------------------------------------------------------
#                                       Screen 2
#-----------------------------------------------------------------------------------------------------------------------------------------------------
G1 = cv2.imread(r"./Assets/Disp2/Green/G1.png",cv2.IMREAD_UNCHANGED)
G4 = cv2.imread(r"./Assets/Disp2/Green/G2.png",cv2.IMREAD_UNCHANGED)
G3 = cv2.imread(r"./Assets/Disp2/Green/G3.png",cv2.IMREAD_UNCHANGED)
G5 = cv2.imread(r"./Assets/Disp2/Green/G4.png",cv2.IMREAD_UNCHANGED)
G2 = cv2.imread(r"./Assets/Disp2/Green/G5.png",cv2.IMREAD_UNCHANGED)
DARKGREEN = (0,100,0)

B1 = cv2.imread(r"./Assets/Disp2/Blue/B1.png",cv2.IMREAD_UNCHANGED)
BlueMap = [B1]
DARKBLUE = (100,0,0)

O3 = cv2.imread(r"./Assets/Disp2/Orange/O1.png",cv2.IMREAD_UNCHANGED)
O5 = cv2.imread(r"./Assets/Disp2/Orange/O2.png",cv2.IMREAD_UNCHANGED)
O2 = cv2.imread(r"./Assets/Disp2/Orange/O3.png",cv2.IMREAD_UNCHANGED)
O1 = cv2.imread(r"./Assets/Disp2/Orange/O4.png",cv2.IMREAD_UNCHANGED)
O6 = cv2.imread(r"./Assets/Disp2/Orange/O5.png",cv2.IMREAD_UNCHANGED)
DARKORANGE = (0,60,120)

P7 = cv2.imread(r"./Assets/Disp2/Purple/P1.png",cv2.IMREAD_UNCHANGED)
P8 = cv2.imread(r"./Assets/Disp2/Purple/P2.png",cv2.IMREAD_UNCHANGED)
P9 = cv2.imread(r"./Assets/Disp2/Purple/P3.png",cv2.IMREAD_UNCHANGED)
P4 = cv2.imread(r"./Assets/Disp2/Purple/P4.png",cv2.IMREAD_UNCHANGED)
empty = np.zeros([height,width,4],dtype = np.uint8)
DARKPURPLE = (100,0,100)

commMap = [empty,G1,G2,G3,G4,G5]
commColors = [(BLACK,BLACK),(GREEN,RED),(GREEN,RED),(GREEN,RED),(GREEN,RED),(GREEN,RED)]
powMap =    [O1,empty,   O2,   O3,   P4,   O5,  O6,  P7,  P8,  P9]
purpleMap = [False, False,False,False,True,False,False,True,True,True]
powColors = [(ORANGE,DARKORANGE),(BLACK,BLACK),(ORANGE,DARKORANGE),(ORANGE,DARKORANGE),(PURPLE,DARKPURPLE),(ORANGE,DARKORANGE),(ORANGE,DARKORANGE),(PURPLE,DARKPURPLE),(PURPLE,DARKPURPLE),(PURPLE,DARKPURPLE)]


WhiteMap = cv2.imread(r"./Assets/Disp2/White/W1.png",cv2.IMREAD_UNCHANGED)

COLOR1 = GREEN
COLOR2 = RED

temporary = False

flowPosition = []

moduleStatii = []
#ECUS
moduleStatii.append(AuxVoltageTooLow)
moduleStatii.append(ECU1Offline)
moduleStatii.append(ECU2Offline)
moduleStatii.append(ECU3Offline)
moduleStatii.append(ECU4Offline)
moduleStatii.append(ECU5Offline)
moduleStatii.append(ECU6Offline)
moduleStatii.append((not batteryOK))
moduleStatii.append(contactorState)
moduleStatii.append(auxFeedState)
#not used
while(len(moduleStatii)<34):
    moduleStatii.append(True)
#powered
moduleStatii.append(False)
moduleStatii.append(False)
moduleStatii.append(False)
moduleStatii.append(False)
moduleStatii.append(False)
moduleStatii.append(False)
moduleStatii.append(False)

moduleStatii.append(True)
moduleStatii.append(True)
moduleStatii.append(False)
moduleStatii.append(False)
moduleStatii.append(True)
moduleStatii.append(True)
moduleStatii.append(True)

#central computers
flowPosition.append(("AUX POW >>",(144,370), AuxVoltageTooLow,(COLOR1,COLOR2),0,0))
flowPosition.append(("AUX ECU",(452,262), ECU1Offline,(COLOR1,COLOR2),0,1))
flowPosition.append(("CAM ECU",(450,167), ECU2Offline,(COLOR1,COLOR2),0,2))
flowPosition.append(("SENS ECU",(840,280), ECU3Offline,(COLOR1,COLOR2),0,3))
flowPosition.append(("MAIN UC",(510,430), ECU4Offline,(COLOR1,COLOR2),0,4))
flowPosition.append(("PROX",(877,57), ECU5Offline,(COLOR1,COLOR2),0,5))
flowPosition.append(("ANLG",(165,425),ECU6Offline,(COLOR1,COLOR2),0,6))
flowPosition.append(("DRV POW >>",(580,115),not batteryOK,(COLOR1,COLOR2),0,7))
flowPosition.append(("RELAY",(686,300),contactorState,(COLOR1,COLOR2),0,8))
flowPosition.append(("BUCK",(710,110),auxFeedState,(COLOR1,COLOR2),0,9))

#10

#grouped sensors
flowPosition.append(("SCREEN",(160,140),ECU2Offline,(COLOR1,COLOR2),1,2))
flowPosition.append(("IMU",(328,128),ECU2Offline,(COLOR1,COLOR2),1,2))
flowPosition.append(("RTC",(310,165),ECU2Offline,(COLOR1,COLOR2),1,2))
flowPosition.append(("GPS",(340,218),ECU2Offline,(COLOR1,COLOR2),1,2))
flowPosition.append(("Antenna",(203,223),ECU2Offline,(COLOR1,COLOR2),1,2))
flowPosition.append(("Panel ",(550,230),ECU2Offline,(COLOR1,COLOR2),1,2))
#6 (16)
flowPosition.append(("ECAM ",(580,310),ECU1Offline,(COLOR1,COLOR2),1,1))
flowPosition.append(("Dash ",(577,350),ECU1Offline,(COLOR1,COLOR2),1,1))
flowPosition.append(("Screen ",(915,345),ECU1Offline,(COLOR1,COLOR2),1,1))
#3 (19)
flowPosition.append(("Gear",(440,340),ECU4Offline,(COLOR1,COLOR2),1,4))
flowPosition.append(("Interf",(630,410),ECU4Offline,(COLOR1,COLOR2),1,4))
flowPosition.append(("Pedal",(630,450),ECU4Offline,(COLOR1,COLOR2),1,4))
flowPosition.append(("SPD DEC",(730,510),ECU4Offline,(COLOR1,COLOR2),1,4))
flowPosition.append(("Speed ",(832,430),ECU4Offline,(COLOR1,COLOR2),1,4))
#5 (24)
flowPosition.append(("Vsense",(767,215),ECU3Offline,(COLOR1,COLOR2),1,3))
flowPosition.append(("Prox Sens",(860,175),ECU3Offline,(COLOR1,COLOR2),1,3))
#2 (26)
flowPosition.append(("Blinkers",(773,30),ECU3Offline,(COLOR1,COLOR2),1,5))
flowPosition.append(("Audio",(773,60),ECU3Offline,(COLOR1,COLOR2),1,5))
flowPosition.append(("Lighting",(773,90),ECU3Offline,(COLOR1,COLOR2),1,5))
flowPosition.append(("Wheel",(773,120),ECU3Offline,(COLOR1,COLOR2),1,5))
#4 (30)
#constants
flowPosition.append(("Tank",(86,530),AuxVoltageTooLow,(COLOR1,COLOR2),2,-1))
flowPosition.append(("F",(243,280),AuxVoltageTooLow,(COLOR1,COLOR2),2,-1))
flowPosition.append(("S",(177,303),AuxVoltageTooLow,(COLOR1,COLOR2),2,-1))
flowPosition.append(("V",(320,295),AuxVoltageTooLow,(COLOR1,COLOR2),2,-1))
#4 (34)
#powered
flowPosition.append(("P ",(230,460),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
flowPosition.append(("Mo",(288,460),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
flowPosition.append(("Iv",(345,460),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
flowPosition.append(("Ba",(405,460),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
#4 (38)
flowPosition.append(("Head",(178,547),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
flowPosition.append(("Steer",(300,547),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
flowPosition.append(("Vacc",(415,547),AuxVoltageTooLow,(COLOR1,COLOR2),3,6))
#3 (41)
#drive circuit

flowPosition.append(("BMS",(600,150),AuxVoltageTooLow,(COLOR1,COLOR2),1,7))
flowPosition.append(("DEC",(600,190),AuxVoltageTooLow,(COLOR1,COLOR2),1,7))
flowPosition.append(("DISCH",(705,175),AuxVoltageTooLow,(COLOR1,COLOR2),3,7))
flowPosition.append(("CHRG",(700,245),AuxVoltageTooLow,(COLOR1,COLOR2),3,7))
flowPosition.append(("BUCK",(610,260),AuxVoltageTooLow,(COLOR1,COLOR2),1,4))
flowPosition.append(("INVERTER",(660,370),AuxVoltageTooLow,(COLOR1,COLOR2),1,8))
flowPosition.append(("M ",(848,384),AuxVoltageTooLow,(COLOR1,COLOR2),1,8))
#7 (48)
dynamicPosition = []

dynamicPosition.append(("Wheel1",(715,415),"rpm",(COLOR1,COLOR2),4))
dynamicPosition.append(("Wheel2",(765,415),"rpm",(COLOR1,COLOR2),4))
dynamicPosition.append(("Wheel3",(715,450),"rpm",(COLOR1,COLOR2),4))
dynamicPosition.append(("Wheel4",(765,450),"rpm",(COLOR1,COLOR2),4))

dynamicPosition.append(("Fan1",(287,445),"%",(COLOR1,COLOR2),6))
dynamicPosition.append(("Fan2",(342,445),"%",(COLOR1,COLOR2),6))
dynamicPosition.append(("Fan3",(410,445),"%",(COLOR1,COLOR2),6))

dynamicPosition.append(("Au Voltage",(300,360),"v",(COLOR1,COLOR2),0))

dynamicPosition.append(("water level",(90,500),"%",(COLOR1,COLOR2),6))

dynamicPosition.append(("prox1",(930,140),"m",(COLOR1,COLOR2),3))
dynamicPosition.append(("prox2",(930,160),"m",(COLOR1,COLOR2),3))
dynamicPosition.append(("prox3",(930,200),"m",(COLOR1,COLOR2),3))
dynamicPosition.append(("prox4",(930,220),"m",(COLOR1,COLOR2),3)) 

dynamicStatus = [0.0,0.0,0.0,0.0, 0,0,0, 12.5, 45, 0,2,3,4]

#-----------------------------------------------------------------------------------------------------------------------------------------------------
#                                       Screen 3
#-----------------------------------------------------------------------------------------------------------------------------------------------------



Gbatteries3 = cv2.imread(r"./Assets/Disp3/batteries.png",cv2.IMREAD_UNCHANGED)
GbatteryTop3 = cv2.imread(r"./Assets/Disp3/batteryTop.png",cv2.IMREAD_UNCHANGED)
Glabel3 = cv2.imread(r"./Assets/Disp3/labels.png",cv2.IMREAD_UNCHANGED)

F1 = cv2.imread(r"./Assets/Disp3/Fans/F1.png",cv2.IMREAD_UNCHANGED)
F2 = cv2.imread(r"./Assets/Disp3/Fans/F2.png",cv2.IMREAD_UNCHANGED)
F3 = cv2.imread(r"./Assets/Disp3/Fans/F3.png",cv2.IMREAD_UNCHANGED)
F4 = cv2.imread(r"./Assets/Disp3/Fans/F4.png",cv2.IMREAD_UNCHANGED)
Fans = [F1,F2,F3,F4]

labelPosition3 = []
labelPosition3.append((5,100))
labelPosition3.append((50,100))
labelPosition3.append((95,100))
labelPosition3.append((140,100))

labelPosition3.append((177,80))
labelPosition3.append((230,80))
labelPosition3.append((280,80))
labelPosition3.append((350,80))

labelPosition3.append((405,50))
labelPosition3.append((450,50))

labelPosition3.append((480,80))
labelPosition3.append((550,80))
labelPosition3.append((610,80))
labelPosition3.append((665,80))

labelPosition3.append((700,100))
labelPosition3.append((745,100))
labelPosition3.append((790,100))
labelPosition3.append((835,100))
labelPosition3.append((880,100))
labelPosition3.append((925,100))
labelPosition3.append((970,100))


booliiPosition3 = []
booliiPosition3.append(((40,440),"Fan1 ","",1))
booliiPosition3.append(((240,440),"Fan2 ","",1))
booliiPosition3.append(((440,440),"Fan3 ","",1))
booliiPosition3.append(((640,440),"Fan4 ","",1))

statiiPosition3 = []
statiiPosition3.append(((40,460),"","%",0.5,0,(-1,0)))
statiiPosition3.append(((240,460),"","%",0.5,1,(-1,0)))
statiiPosition3.append(((440,460),"","%",0.5,2,(-1,0)))
statiiPosition3.append(((640,460),"","%",0.5,3,(-1,0)))

statiiPosition3.append(((40,480),"","c",0.5,0,(50,30)))
statiiPosition3.append(((240,480),"","c",0.5,1,(30,50)))
statiiPosition3.append(((440,480),"","c",0.5,2,(30,50)))
statiiPosition3.append(((640,480),"","c",0.5,3,(30,50)))

statii3 = [100,100,100,100,22,22,23,24]
statii3[0]
statii3[1]
statii3[2]
statii3[3]
statii3[4] = temps[0]
statii3[5] = temps[1]
statii3[6] = temps[2]
statii3[7] = temps[3]

boolii3 = [True,True,True,True]

barPosition3 = []
barPosition3.append(((130,480),20,65,GREEN,(0,100)))
barPosition3.append(((330,480),20,65,GREEN,(0,100)))
barPosition3.append(((530,480),20,65,GREEN,(0,100)))
barPosition3.append(((730,480),20,65,GREEN,(0,100)))


barPosition3.append(((160,480),20,65,RED,(30,100)))
barPosition3.append(((360,480),20,65,RED,(30,100)))
barPosition3.append(((560,480),20,65,RED,(30,100)))
barPosition3.append(((760,480),20,65,RED,(30,100)))

linePosition3 = []
linePosition3.append([40,490,180,490])
linePosition3.append([240,490,380,490])
linePosition3.append([440,490,580,490])
linePosition3.append([640,490,780,490])


staticPosition3 = []
staticPosition3.append(("ANODE",(3,188),0.8))
staticPosition3.append(("CATHODE",(900,427),0.8))
staticPosition3.append(("21S15P",(897,448),0.5))
staticPosition3.append(("4.7 kWh",(887,480),0.5))

statusPosition3 = []
statusPosition3.append(("ContactorState",(25,510),0))
statusPosition3.append(("AuxFeedState",(225,510),0))
statusPosition3.append(("ChargeFETTempHigh",(425,510),0))
statusPosition3.append(("DischargeFETTempHigh",(625,510),0))
statusPosition3.append(("CellBalanceState",(800,30),0))

status3 = [False,False,False,False,False]
status3[0]
status3[1]
status3[2] = alarms[3][2]
status3[3] = alarms[3][3]
status3[4] = cellBalanceActive

levelStatusPosition3 = []
levelStatusPosition3.append(("CellVoltageHigh",(25,530),0))
levelStatusPosition3.append(("CellVoltageLow",(225,530),1))
levelStatusPosition3.append(("PackVoltageHigh",(425,530),2))
levelStatusPosition3.append(("PackVoltageLow",(625,530),3))

levelStatusPosition3.append(("ChargeTempHigh",(25,550),4))
levelStatusPosition3.append(("ChargeTempLow",(225,550),5))
levelStatusPosition3.append(("DischargeTempHigh",(425,550),6))
levelStatusPosition3.append(("DischargeTempLow",(625,550),7))

levelStatusPosition3.append(("ChargeCurrentHigh",(25,570),8))
levelStatusPosition3.append(("DischargeCurrentHigh",(225,570),9))
levelStatusPosition3.append(("StateOfChargeLow",(425,570),10))
levelStatusPosition3.append(("CellDifferenceHigh",(625,570),10))


#levelStatus3 = [False,False]*12


levelStatus3 = [[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,True],[False,False],[False,True],[False,False],[False,False],[False,True],[False,False],[True,False],[False,True]]
#muteStatus3 = [False,False,False,False,False,False,False]
#failurePosition3.append(("",))




#-----------------------------------------------------------------------------------------------------------------------------------------------------
#                                       Screen 4
#-----------------------------------------------------------------------------------------------------------------------------------------------------
MotorBase = cv2.imread(r"./Assets/Disp4/Base.png",cv2.IMREAD_UNCHANGED)
MotorPower = cv2.imread(r"./Assets/Disp4/Power.png",cv2.IMREAD_UNCHANGED)
MotorCooling = cv2.imread(r"./Assets/Disp4/Cooling.png",cv2.IMREAD_UNCHANGED)
MotorOutput = cv2.imread(r"./Assets/Disp4/Output.png",cv2.IMREAD_UNCHANGED)

statiiPosition4 = []
statiiPosition4.append(("Output",(250,590)))

statusPosition4 = []
statusPosition4.append(("Traction",(15,310),("disabled","enabled")))
statusPosition4.append(("Pump",(600,500),("disabled","enabled")))
statusPosition4.append(("Power",(400,560),("disabled","enabled")))
statusPosition4.append(("Fan",(830,420),("disabled","enabled")))
statusPosition4.append(("Inverter",(880,360),("disabled","enabled")))

pumping = False
Fanning = False
Running = False
status4 = [True,pumping,contactorState,True,False]
