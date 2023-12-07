import cv2
import numpy as np


image = cv2.imread("outdoor.jpeg")
image = cv2.resize(image, (448, 336))
cv2.imshow('Original', image)

gaussian_pyramid = [image]
gaussian_filter = np.array([1, 4, 6, 4, 1])/16

for i in range(4):
    next = cv2.pyrDown(gaussian_pyramid[-1])
    gaussian_pyramid.append(next)
    
    cv2.imshow(f'Gaussian Pyramid - {i}', next)
    

laplacian_pyramid = []
for i in range(len(gaussian_pyramid) - 1,0,-1):
    size = (gaussian_pyramid[i - 1].shape[1], gaussian_pyramid[i - 1].shape[0])
    upsampled_image = cv2.pyrUp(gaussian_pyramid[i], dstsize=size)
    
    laplacian_image = cv2.subtract(gaussian_pyramid[i - 1], upsampled_image)
    laplacian_pyramid.append(laplacian_image)
    
    cv2.imshow(f"Laplacian Pyramid - {i}", laplacian_image)
    
cv2.waitKey(0)
cv2.destroyAllWindows()
