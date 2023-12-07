import cv2
import numpy as np


image = cv2.imread("outdoor.jpeg")
image = cv2.resize(image, (448, 336))
cv2.imshow('Original', image)

gaussian_pyramid = [image]
gaussian_filter = np.array([1, 4, 6, 4, 1])/16

for i in range(4):
    smoothed_image = cv2.filter2D(gaussian_pyramid[-1], -1, gaussian_filter)
    downsampled_image = smoothed_image[::2, ::2]
    gaussian_pyramid.append(downsampled_image)

    cv2.imshow(f'Gaussian Pyramid -{i}', downsampled_image)
    
cv2.waitKey(0)
cv2.destroyAllWindows()
