import cv2
import numpy as np

def extract_sift_features(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    sift = cv2.SIFT.create()
    keypoints, _ = sift.detectAndCompute(gray, None)
    
    img = cv2.drawKeypoints(gray, keypoints, image, flags=cv2.DrawMatchesFlags_DRAW_RICH_KEYPOINTS)
    cv2.imshow('SIFT Keypoints', img)

def extract_hog_features(image):
    hog = cv2.HOGDescriptor()
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    hog_features = hog.compute(gray, winStride=(8, 8), padding=(4, 4))
    
    hog_features = np.sqrt(hog_features**2).transpose()
    hog_features = cv2.normalize(hog_features, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)

    img = cv2.resize(hog_features, (image.shape[1], image.shape[0]))
    cv2.imshow('HOG Histogram', img)


image = cv2.imread('paris.jpg')
image = cv2.resize(image, (448, 336))
extract_sift_features(image)

image = cv2.imread('paris.jpg')
image = cv2.resize(image, (448, 336))
extract_hog_features(image)

cv2.waitKey(0)
cv2.destroyAllWindows()
