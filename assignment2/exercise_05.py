import cv2

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
    
    print(hog_features.shape)
    
    # img = cv2.drawKeypoints(gray, hog_features, image, flags=cv2.DrawMatchesFlags_DRAW_RICH_KEYPOINTS)
    # cv2.imshow('HOG Keypoints', img)


image = cv2.imread('paris.jpg')
image = cv2.resize(image, (448, 336))

extract_sift_features(image)
extract_hog_features(image)

cv2.waitKey(0)
cv2.destroyAllWindows()
