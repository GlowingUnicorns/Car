import numpy as np
import cv2
import time
import keyboard
import random
from GUIVariables import *
def setBGR(matrix,bgr):
    matrix[:,:,0] = np.full([matrix.shape[0], matrix.shape[1]],bgr[0])#Blue
    matrix[:,:,1] = np.full([matrix.shape[0], matrix.shape[1]],bgr[1])#Green
    matrix[:,:,2] = np.full([matrix.shape[0], matrix.shape[1]],bgr[2])#Red
    return matrix

def emptyBGR(matrix):
    matrix[:,:,3] = np.full([matrix.shape[0], matrix.shape[1]],0)
    return matrix

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


def bleh(a,b):
    return (a-1)*8 + b

def overlay_images(background, foreground):

    # Ensure both images have the same dimensions (optional)
    if foreground.shape[:2] != background.shape[:2]:
        background = cv2.resize(background, (foreground.shape[1], foreground.shape[0]))

    # Extract the alpha channel (transparency) from the foreground image
    alpha_foreground = foreground[:, :, 3]

    # Invert the alpha channel to create a mask for the background
    alpha_background = 255 - alpha_foreground

    # Create a composite image by blending the foreground and background
    for c in range(0, 3):
        background[:, :, c] = (background[:, :, c] * (alpha_background / 255.0) +
                              foreground[:, :, c] * (alpha_foreground / 255.0)).astype(np.uint8)

    # Save the resulting image
    return background
