import cv2
import os
import numpy as np

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

os.chdir(r"C:/Users/lionm/OneDrive/Documents/Code/GUI")

#path = r'C:/Users/Rajnish/Desktop/geeksforgeeks.png'

Gbackground = cv2.imread(r"./Assets/Background.png", cv2.IMREAD_UNCHANGED)
Gbase = cv2.imread(r"./Assets/Disp1/Base.png", cv2.IMREAD_UNCHANGED)
Gbattery = cv2.imread(r"./Assets/Disp1/Battery.png", cv2.IMREAD_UNCHANGED)

  
window_name = 'image'
image = overlay_images(Gbase,Gbackground)
image = overlay_images(Gbattery,image)

cv2.imshow(window_name, image) 
  
cv2.waitKey(0) 
  
cv2.destroyAllWindows() 