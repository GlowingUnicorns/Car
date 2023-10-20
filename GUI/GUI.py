import numpy as np
import cv2
import time
import random
from GUIVariables import *
from GUIFunctions import *

import time
import copy
systemOK = False

#-----------------------------------------------------------------------
#                             Side Bar
#-----------------------------------------------------------------------
def setSideBar(): 

    sideBar = copy.deepcopy(GsideBar)
    for i in range(11):
        H = 37
        if (batteryVoltage[i] < 3):
            COLOR = RED
        elif (batteryVoltage[i] > 4):
            COLOR = GREEN
        else:
            COLOR = WHITE
        cv2.putText(sideBar,str(i+1) + "  "+ str(batteryVoltage[i]) + "v",(5,220+i*H),font,0.5,COLOR,1,cv2.LINE_AA,)
    for i in range(10):
        if (batteryVoltage[i+10] < 3):
            COLOR = RED
        elif (batteryVoltage[i+10] > 4):
            COLOR = GREEN
        else:
            COLOR = WHITE
        cv2.putText(sideBar,str(i+12) + "  "+ str(batteryVoltage[10+i]) + "v",(110,220+i*37),font,0.5,COLOR,1,cv2.LINE_AA,)

    if(batteryTemp < 30):
        COLOR = (255,125,0)
    else:
        COLOR = ORANGE
    cv2.putText(sideBar, "   " + str(batteryTemp) + "C",(110,220+10*37),font,0.5,COLOR,1,cv2.LINE_AA,)


    if(packVoltage > 4.2*21):
        COLOR = RED
    elif(packVoltage < 3*21):
        COLOR = BLUE
    else:
        COLOR = WHITE

    if packVoltage > 70:
        COLOR = GREEN
    else:
        COLOR = RED
    cv2.putText(sideBar,str(packVoltage) + "v",(15,60),font,1.5,COLOR,1,cv2.LINE_AA,)
    if packCurrent > 5:
        COLOR = GREEN
        if packCurrent > 150:
            COLOR = RED
    else:
        COLOR = WHITE
    cv2.putText(sideBar,str(packCurrent) + "A",(15,120),font,1.5,COLOR,1,cv2.LINE_AA,)
    if(packSOC < 20):
        COLOR = RED
    else:
        COLOR = GREEN
    cv2.putText(sideBar,"SOC: "+str(95) + "%",(15,150),font,0.5,COLOR,1,cv2.LINE_AA,)
    cv2.putText(sideBar,"Cap: "+str(3094) + "mAh",(15,175),font,0.5,COLOR,1,cv2.LINE_AA)
    return sideBar
#-----------------------------------------------------------------------
#                             Bottom Bar
#-----------------------------------------------------------------------
def bottomBar():
    botBar = np.zeros([82,1018+3,4],dtype = np.uint8)
    #botBar.fill(255)
    botBar[:,:,3] = np.full([botBar.shape[0],botBar.shape[1]],255)
    botBar = np.hstack((botBar,Cat))
    for aux in NumAuxPosition:
        cv2.putText(botBar,str(aux[0]) + ":" + str(NumAux[aux[3]]) + str(aux[2]),aux[1],font,0.5,WHITE,1,cv2.LINE_AA)
    return botBar

line1 = np.zeros([2,1018+200+3,4],dtype = np.uint8)
line1.fill(255)
line1[:,:,3] = np.full([line1.shape[0],line1.shape[1]],255)
line = np.zeros([height,3,4],dtype = np.uint8)
line.fill(255)
line[:,:,3] = np.full([line.shape[0], line.shape[1]],255)
Gimg = Gbackground

#-----------------------------------------------------------------------
#                             screen 1
#-----------------------------------------------------------------------
def screen1():
    img = copy.deepcopy(Gimg)
    if(statii[0] == True):
        battery2 = setBGR(Gbattery2,GREEN)
    else:
        battery2=Gbattery2
    if(statii[1] == True):
        secondary = setBGR(Gsecondary,GREEN)
    else:
        secondary=Gsecondary
    if(statii[2] == True):
        dash = setBGR(Gdash,GREEN)
    else:
        dash=Gdash
    if(statii[3] == True):
        steering = setBGR(Gsteering,GREEN)
    else:
        steering=Gsteering
    if(statii[4] == True):
        motor = setBGR(Gmotor,GREEN)
    else:
        motor=Gmotor
    if(statii[6] == True):
        inverter = setBGR(Ginverter,GREEN)
    else:
        inverter=Ginverter
    if(statii[7] == True):
        battery = setBGR(Gbattery,GREEN)
    else:
        battery=Gbattery
    if(statii[8] == True):
        footBrake = setBGR(GfootBrake,GREEN)
    else:
        footBrake=GfootBrake
    if(statii[5] == False ):
        battery = setBGR(battery,ORANGE)
    else:
        battery = battery
    img = overlay_images(img,steering)
    img = overlay_images(img,secondary)
    img = overlay_images(img,footBrake)
    img = overlay_images(img,battery)
    img = overlay_images(img,inverter)
    img = overlay_images(img,dash)
    img = overlay_images(img,motor)
    img = overlay_images(img,Gbase)
    img = overlay_images(img,battery2)
    img = overlay_images(img,Glabel)

    img = cv2.cvtColor(img, cv2.COLOR_RGB2RGBA)

    j = 0
    for status in labelPosition:
        if(statii[j] == True):
            COLOR = GREEN
            s = status[2][0]
        else:
            COLOR = RED
            if(j==5):
                COLOR = ORANGE
            s = status[2][1]
        cv2.putText(img,status[0] +": "+ s,status[1],font,0.5,COLOR,1,cv2.LINE_AA)

        j+=1
    cv2.putText(img,"System Diagram",(13,32),font,1,WHITE,1,cv2.LINE_AA)
    j = 0
    for detail in detailiiPosition:
        if detailii[j] >= 0:
            cv2.putText(img,str(detailii[j]) + detail[0],detail[1],font,0.5,WHITE,1,cv2.LINE_AA)
        j+=1
        
    return img
#-----------------------------------------------------------------------
#                             screen 2
#-----------------------------------------------------------------------
screen2Mode = 0
screenMode = 0
def screen2():
    img = copy.deepcopy(Gimg)
    j = 0
    for comp in flowPosition:

        if(comp[4] == 0):

            if(moduleStatii[j]):
                k = 1
                COLOR = comp[3][1]
            else:
                k = 0
                COLOR = comp[3][0]

            
            if screen2Mode == 0:
                COLOR2 = powColors[j][k]
            elif screen2Mode == 2:
                if (purpleMap[j] == False):
                    COLOR2 = powColors[j][k]
                else:
                    COLOR2 = GREY
            elif screen2Mode == 3:
                if purpleMap[j]:
                    COLOR2 = powColors[j][k]
                else:
                    COLOR2 = GREY     
            else:
                COLOR2 = GREY
            powMap[j]=setBGR(powMap[j],COLOR2)
            if(j < 6):
                if screen2Mode == 0:
                    COLOR2 = commColors[j][k]
                elif screen2Mode == 1:
                    COLOR2 = commColors[j][k]
                else:
                    COLOR2 = GREY
                commMap[j]=setBGR(commMap[j],COLOR2)
                
        elif(comp[4] == 1):
            if(moduleStatii[comp[5]] == True):
                COLOR = comp[3][1]
            else:
                COLOR = comp[3][0]
        elif(comp[4] == 2):
            COLOR = WHITE
        else:
            if(moduleStatii[comp[5]] == True and moduleStatii[j] == True):
                COLOR = comp[3][1]
            else:
                COLOR = comp[3][0]
        j+=1
        cv2.putText(img,comp[0],comp[1],font,0.4,COLOR,1,cv2.LINE_AA)

    if(screen2Mode == 0 or screen2Mode == 4):
        COLOR2 = CYAN
    else:
        COLOR2 = GREY
    BlueMap[0] = setBGR(BlueMap[0],COLOR2)

    j = 0
    for dynamic in dynamicPosition:
        if(flowPosition[dynamic[4]][2] == True):
            COLOR = dynamic[3][1]
        else:
            COLOR = dynamic[3][0]
        cv2.putText(img,str(dynamicStatus[j])+dynamic[2],dynamic[1],font,0.4,COLOR,1,cv2.LINE_AA)

    img = overlay_images(img,commMap[0])
    img = overlay_images(img,commMap[1])
    img = overlay_images(img,commMap[2])
    img = overlay_images(img,commMap[3])
    img = overlay_images(img,commMap[4])
    img = overlay_images(img,commMap[5])

    img = overlay_images(img,powMap[0])
    img = overlay_images(img,powMap[1])
    img = overlay_images(img,powMap[2])
    img = overlay_images(img,powMap[3])
    img = overlay_images(img,powMap[4])
    img = overlay_images(img,powMap[5])
    img = overlay_images(img,powMap[6])
    img = overlay_images(img,powMap[7])
    img = overlay_images(img,powMap[8])
    img = overlay_images(img,powMap[9])

    img = overlay_images(img,BlueMap[0])


    img = overlay_images(img,WhiteMap)

    cv2.putText(img,"Flow Diagram",(13,32),font,1,WHITE,1,cv2.LINE_AA)

    return img

def screen3():
    img = copy.deepcopy(Gimg)

    j = 0
    for label in labelPosition3:
        if cellVmV[j]<3:
            COLOR = RED
        elif cellVmV[j]>4:
            COLOR = GREEN
        else:
            COLOR = WHITE
        
        if(minCellVNum - 1 == j):
            COLOR = RED
            cv2.rectangle(img,(label[0] - 5,label[1]+5),(label[0] + 35,label[1] - 15),COLOR,-1)
            cv2.putText(img,str(cellVmV[j]),label,font,0.4,BLACK,1,cv2.LINE_AA)
        elif(maxCellVNum - 1 == j):
            COLOR = GREEN
            cv2.rectangle(img,(label[0] - 5,label[1]+5),(label[0] + 35,label[1] - 15),COLOR,-1)
            cv2.putText(img,str(cellVmV[j]),label,font,0.4,BLACK,1,cv2.LINE_AA)    
        else:
            cv2.putText(img,str(cellVmV[j]),label,font,0.4,COLOR,1,cv2.LINE_AA)           
       
        j+=1
    j = 0
    for label in booliiPosition3:
        if(boolii3[j] == True):
            COLOR = GREEN
        else:
            COLOR = RED
        cv2.putText(img,label[1],label[0],font,label[3],COLOR,1,cv2.LINE_AA)
        Fans[j] = setBGR(Fans[j],COLOR)
        j+=1
    j = 0
    for label in statiiPosition3:
        if(statii3[j] > label[5][1]):
            COLOR = GREEN
        elif(statii[j] < label[5][0]):
            COLOR = RED
        else:
            COLOR = WHITE

        cv2.putText(img,str(statii3[j]) + label[2],label[0],font,label[3],COLOR,1,cv2.LINE_AA)
        j+=1
    j = 0
    for bar in barPosition3:
        set1 = (bar[0][0],bar[0][1])
        set2 = ((bar[0][0]+bar[1]),int(map_range(statii3[j],(bar[4][0]),(bar[4][1]),(bar[0][1]),(bar[0][1]-bar[2]))))
        if set2[1] < set1[1]:
            cv2.rectangle(img,set1,set2,bar[3],-1)
        j+=1
    for label in staticPosition3:
        cv2.putText(img,str(label[0]),label[1],font,label[2],WHITE,1,cv2.LINE_AA)
    for line in linePosition3:
        cv2.line(img,(line[0],line[1]),(line[2],line[3]),WHITE,2)

    for status in levelStatusPosition3:
        COLOR = GREEN
        if levelStatus3[status[2]][0] == True:
            COLOR = RED
        cv2.putText(img,str(status[0]) + ":" + str(levelStatus3[status[2]][0]),status[1],font,0.4,COLOR,1,cv2.LINE_AA)
    for status in statusPosition3:
        COLOR = GREEN
        if status3[status[2]] == True:
            COLOR = RED
        cv2.putText(img,str(status[0]) + ":" + str(status3[status[2]]),status[1],font,0.4,COLOR,1,cv2.LINE_AA)

    img = overlay_images(img,Gbatteries3)
    img = overlay_images(img,GbatteryTop3)
    img = overlay_images(img,Glabel3)
    img = overlay_images(img,Fans[0])
    img = overlay_images(img,Fans[1])
    img = overlay_images(img,Fans[2])
    img = overlay_images(img,Fans[3])
    cv2.putText(img,"BATT Diagram",(13,32),font,1,WHITE,1,cv2.LINE_AA)

    return img

        
def screen4():
    img = copy.deepcopy(Gimg)
    i = 0
    for status in statusPosition4:
        if status4[i] == False:
            COLOR = RED
            cv2.putText(img,str(status[0]) + ":" + str(status[2][0]),status[1],font,0.6,COLOR,1,cv2.LINE_AA)
        else:
            COLOR = GREEN
            cv2.putText(img,str(status[0]) + ":" + str(status[2][1]),status[1],font,0.6,COLOR,1,cv2.LINE_AA)

        i += 1
    if(contactorState == True):
        COLOR1 = GREEN
    else:
        COLOR1 = WHITE
    if(pumping == True):
        COLOR2 = GREEN
    else:
        COLOR2 = WHITE
    if(Running == True):
        COLOR3 = GREEN
    else:
        COLOR3 = WHITE
    img = overlay_images(img,MotorBase)
    img = overlay_images(img,setBGR(MotorCooling,COLOR2))
    img = overlay_images(img,setBGR(MotorPower,COLOR1))
    img = overlay_images(img,setBGR(MotorOutput,COLOR3))
    cv2.putText(img,"Motor Diagram",(13,32),font,1,WHITE,1,cv2.LINE_AA)

    return img;




def main():

    
    output3 = bottomBar()
    output1 = setSideBar()
    if(screenMode == 0):
        output2 = screen1()
    elif(screenMode == 1): 
        output2 = screen2()
    elif(screenMode == 2):
        output2 = screen3()
    elif(screenMode ==3):
        output2 = screen4()

    cv2.line(output2,(0,50),(330,50),WHITE,2)
    cv2.line(output2,(330,50),(380,0),WHITE,2)

    cv2.putText(output2,"Max: " + str(maxCellVNum) + "  " + str(maxCellmV) + "v",(890,530),font,0.5,WHITE,1,cv2.LINE_AA)
    cv2.putText(output2,"Min: " + str(minCellVNum) + "  " + str(minCellmV)+ "v",(890,555),font,0.5,WHITE,1,cv2.LINE_AA)
    cv2.putText(output2,"Dif:     " + str(cellDiff) + "v",(890,580),font,0.5,WHITE,1,cv2.LINE_AA)
    j = 0
    for f in failure:
        if(alarms[f[1][0]][f[1][1]] == True and f[2] == 1):
            COLOR = RED
            cv2.putText(output2,f[0],(5,75 + 20*j),font,0.4,COLOR,1,cv2.LINE_AA)
            j+=1
    output2 = cv2.cvtColor(output2, cv2.COLOR_RGB2RGBA)
    output = np.hstack((output2,line,output1))
    output = np.vstack((output,line1,output3))

    output = cv2.cvtColor(output, cv2.COLOR_RGBA2RGB)
    cv2.imshow(title,output);
    #cv2.imshow(title,cv2.resize(output,(1920,1080),interpolation = cv2.INTER_AREA))
    
#cv2.namedWindow(title,cv2.WND_PROP_FULLSCREEN)
#cv2.setWindowProperty(title,cv2.WND_PROP_FULLSCREEN,cv2.WINDOW_FULLSCREEN)

temporaryNum = 0
while(True):

        # for i in range(len(moduleStatii)):
        #     moduleStatii[i] = random.getrandbits(1)

    main()
    time.sleep(0.25)

    keyPress = cv2.waitKey(1)
    if keyPress == ord('q'):
        break
    elif keyPress == ord('1'):
        screenMode = 0
    elif keyPress == ord('2'):
        screenMode = 1
    elif keyPress == ord('3'):
        screenMode = 2
    elif keyPress == ord('4'):
        screenMode = 3
    elif keyPress == ord('5'):
        screen2Mode += 1
        if(screen2Mode >= 5):
            screen2Mode = 0
    



cv2.destroyAllWindows()

