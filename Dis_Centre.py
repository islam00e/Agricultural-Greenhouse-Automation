import cv2
import numpy as np
import time

# Constants for camera calibration (replace these with your calibrated values)
pixels_per_mm_x = 10
pixels_per_mm_y = 10

def detect_one_circle_from_webcam(min_radius, max_radius):
    # Open the webcam
    cap = cv2.VideoCapture(0)

    # Wait for 5 seconds before starting circle detection
    print("Waiting for 5 seconds before starting circle detection...")
    time.sleep(2)
    print("Circle detection started!")

    while True:
        # Read a frame from the webcam
        ret, frame = cap.read()

        # Get the dimensions of the frame
        height, width, _ = frame.shape

        # Calculate the center of the frame
        center_x = width // 2
        center_y = height // 2

        # Draw a crosshair at the center of the frame
        cv2.drawMarker(frame, (center_x, center_y), (0, 255, 0), markerType=cv2.MARKER_CROSS, thickness=2)

        # Convert the frame to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Apply Gaussian blur to reduce noise (optional but often helpful)
        gray = cv2.GaussianBlur(gray, (9, 9), 2)

        # Apply Hough Circle Transform
        circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp=1, minDist=50,
                                   param1=100, param2=30, minRadius=min_radius, maxRadius=max_radius)

        # If at least one circle is detected, draw the first one and calculate the distance to the center
        if circles is not None and len(circles) > 0:
            circle = np.round(circles[0, 0]).astype("int")
            (x, y, r) = circle

            # Draw the circle
            cv2.circle(frame, (x, y), r, (0, 0, 255), 3)

            # Calculate the distance from the circle center to the frame center in millimeters
            distance_mm = ((center_x - x) / pixels_per_mm_x, (center_y - y) / pixels_per_mm_y)

            # Print the distance
            print(f"Distance to Center: {distance_mm} mm")

        # Display the frame with the detected circle and the center
        cv2.imshow("Detected Circle and Center", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the webcam and close all windows
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    min_radius = 10  # Minimum circle radius to detect
    max_radius = 100  # Maximum circle radius to detect
    detect_one_circle_from_webcam(min_radius, max_radius)
