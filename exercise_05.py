import cv2
import numpy as np

def draw_keypoints(image, keypoints):
    # Create a copy of the image
    image_copy = image.copy()

    # Draw keypoints on the image
    for keypoint in keypoints:
        cv2.circle(image_copy, (int(keypoint.pt[0]), int(keypoint.pt[1])), 5, (0, 0, 255), -1)

    # Return the image with keypoints drawn
    return image_copy

def draw_hog_features(image, hog_features):
    # Create a copy of the image
    image_copy = image.copy()

    # Reshape the HOG features into a 2D array
    hog_features = hog_features.reshape((hog_features.shape[0], hog_features.shape[1] // 4))

    # Normalize the HOG features
    hog_features = cv2.normalize(hog_features, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)

    # Draw HOG features on the image
    for i in range(hog_features.shape[0]):
        for j in range(hog_features.shape[1]):
            cv2.rectangle(image_copy, (j * 8, i * 8), ((j + 1) * 8, (i + 1) * 8), hog_features[i, j], -1)

    # Return the image with HOG features drawn
    return image_copy

def extract_sift_features(image):
    # Convert image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Initialize SIFT detector
    sift = cv2.SIFT.create()

    # Detect keypoints and descriptors
    keypoints, descriptors = sift.detectAndCompute(gray, None)

    # Return keypoints and descriptors
    return keypoints, descriptors

def extract_hog_features(image):
    # Convert image to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Initialize HOG descriptor
    hog = cv2.HOGDescriptor()

    # Compute HOG descriptor
    hog_features, _ = hog.compute(gray, winStride=(8, 8),
                                    padding=(4, 4))

    # Normalize the histograms
    cv2.normalize(hog_features, None, alpha=0, beta=1.0,
                  norm_type=cv2.NORM_L1, dtype=cv2.CV_32F)

    # Return HOG features
    return hog_features


# Read the image
image = cv2.imread('image.jpg')
image = cv2.resize(image, (448, 336))

# Extract SIFT features
sift_keypoints, sift_descriptors = extract_sift_features(image)

# Extract HOG features
hog_features = extract_hog_features(image)

# Show the results
cv2.imshow('SIFT Keypoints', draw_keypoints(image, sift_keypoints))
cv2.imshow('HOG Features', draw_hog_features(image, hog_features))

cv2.waitKey(0)
cv2.destroyAllWindows()
