import cv2
import time
import os
import numpy as np
import random
os.chdir(r"C:/Users/lionm/OneDrive/Documents/Code/")

time.sleep(0.1)
title = 'test'
showProximity = True

WHITE=(255,255,255);BLACK=(0,0,0);ORANGE=(0,125,255);BLUE=(255,0,0);GREEN=(0,255,0);RED=(0,0,255)
PURPLE = (255,0,255);GREY=(100,100,100);CYAN=(255,255,0)
font = cv2.FONT_HERSHEY_DUPLEX

proximityData = [10.0,1.2,2.3,4.5]

proximityLocations = []
proximityLocations.append((150,550))
proximityLocations.append((425,610))
proximityLocations.append((720,610))
proximityLocations.append((950,550))



def setBGR(matrix,bgr):
    matrix[:,:,0] = np.full([matrix.shape[0], matrix.shape[1]],bgr[0])#Blue
    matrix[:,:,1] = np.full([matrix.shape[0], matrix.shape[1]],bgr[1])#Green
    matrix[:,:,2] = np.full([matrix.shape[0], matrix.shape[1]],bgr[2])#Red
    return matrix

pointsLL = np.array([[50, 450], [50, 680], [330, 680],
            [330, 630], [110, 630],[80, 450]])
pointsL  = np.array([[370, 630], [370, 680], [610, 680],
            [610, 660]])
pointsR = np.array([[1280-50, 450], [1280-50, 680], [1280-330, 680],
            [1280-330, 630], [1280-110, 630],[1280-80, 450]])
pointsRR  = np.array([[1280-370, 630], [1280-370, 680], [1280-610, 680],
            [1280-610, 660]])
while True:
    frame = cv2.imread(r'./GUIRear/Assets/TestImage.png')	
    if (showProximity):


        
        j = 0
        if(proximityData[0] < 5.5): COLOR = CYAN
        else: COLOR = WHITE
        cv2.fillPoly(frame, pts=[pointsLL], color=COLOR)
        cv2.putText(frame,str(proximityData[0]) + "m" ,proximityLocations[0],font,2,COLOR,2,cv2.LINE_AA)
        if(proximityData[1] < 5.5): COLOR = CYAN
        else: COLOR = WHITE
        cv2.fillPoly(frame, pts=[pointsL], color=COLOR)
        cv2.putText(frame,str(proximityData[1]) + "m" ,proximityLocations[1],font,2,COLOR,2,cv2.LINE_AA)
        if(proximityData[2] < 5.5): COLOR = CYAN
        else: COLOR = WHITE
        cv2.fillPoly(frame, pts=[pointsR], color=COLOR)
        cv2.putText(frame,str(proximityData[2]) + "m" ,proximityLocations[2],font,2,COLOR,2,cv2.LINE_AA)
        if(proximityData[3] < 5.5): COLOR = CYAN
        else: COLOR = WHITE
        cv2.fillPoly(frame, pts=[pointsRR], color=COLOR)
        cv2.putText(frame,str(proximityData[3]) + "m" ,proximityLocations[3],font,2,COLOR,2,cv2.LINE_AA)
    
    for i in range(4):
        proximityData[i] = random.randint(0,10)
                
    cv2.imshow(title,frame)
    time.sleep(0.1)

    if(cv2.waitKey(1)==ord('q')):
        break
cv2.destroyAllWindows()
