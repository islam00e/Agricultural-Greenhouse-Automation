import cv2
import numpy as np

# Constants for camera calibration (replace these with your calibrated values)
# These are just example values; you need to obtain your actual calibration parameters
# To do so, perform camera calibration using a chessboard or a calibration pattern.
pixels_per_mm_x = 10
pixels_per_mm_y = 10

def detect_circles_from_webcam(min_radius, max_radius):
    # Open the webcam
    cap = cv2.VideoCapture(0)

    while True:
        # Read a frame from the webcam
        ret, frame = cap.read()

        # Convert the frame to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Apply Gaussian blur to reduce noise (optional but often helpful)
        gray = cv2.GaussianBlur(gray, (9, 9), 2)

        # Apply Hough Circle Transform
        circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp=1, minDist=50,
                                   param1=100, param2=30, minRadius=min_radius, maxRadius=max_radius)

        # If circles are detected, draw them on the frame and calculate the diameter and center
        if circles is not None:
            circles = np.round(circles[0, :]).astype("int")
            for (x, y, r) in circles:
                # Draw the circle
                cv2.circle(frame, (x, y), r, (0, 255, 0), 4)
                # Draw the center of the circle
                cv2.circle(frame, (x, y), 1, (0, 0, 255), 3)
                
                # Calculate the diameter in millimeters
                diameter_mm = 2 * r / pixels_per_mm_x  # Adjust for calibration factor
                
                # Print the diameter and center coordinates
                print(f"Diameter: {diameter_mm:.2f} mm, Center: ({x}, {y})")

        # Display the frame with detected circles and centers
        cv2.imshow("Detected Circles", frame)

        # Press 'q' to exit the loop and close the window
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the webcam and close all windows
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    min_radius = 10  # Minimum circle radius to detect
    max_radius = 100  # Maximum circle radius to detect
    detect_circles_from_webcam(min_radius, max_radius)
