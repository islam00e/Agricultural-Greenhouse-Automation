import cv2
import numpy as np
import time
import serial

# Constants for camera calibration (replace these with your calibrated values)
# These are just example values; you need to obtain your actual calibration parameters
# To do so, perform camera calibration using a chessboard or a calibration pattern.
pixels_per_mm_x = 10
pixels_per_mm_y = 10

def detect_circles_from_webcam(min_radius, max_radius):
    # Open the webcam
    cap = cv2.VideoCapture(0)

    # Wait for 30 seconds before starting the circle detection
    print("Waiting for 30 seconds before starting circle detection...")
    time.sleep(30)
    print("Circle detection started!")

    # Create an array to store the diameter and center of each detected circle
    circle_data = []

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
                
                # Store the diameter and center coordinates in the array
                circle_data.append({'diameter': diameter_mm, 'center': (x, y)})

        # Display the frame with detected circles and centers
        cv2.imshow("Detected Circles", frame)

        # Add a delay of 1 second between each frame processing
        time.sleep(1)

        # Press 'q' to exit the loop and close the window after storing 10 circles
        if len(circle_data) >= 10:
            break

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the webcam and close all windows
    cap.release()
    cv2.destroyAllWindows()

    # Print the stored circle data
    for i, circle in enumerate(circle_data):
        diameter = circle['diameter']
        center = circle['center']
        print(f"Circle {i+1}: Diameter: {diameter:.2f} mm, Center: {center}")

    # Send the circle data to Arduino over the serial connection
    send_circle_data_to_arduino(circle_data)

def send_circle_data_to_arduino(circle_data):
    # Establish serial connection with the Arduino
    ser = serial.Serial('/dev/ttyACM0', 9600)  # Replace '/dev/ttyACM0' with the correct serial port of your Arduino

    # Send the circle data to Arduino
    for circle in circle_data:
        diameter = circle['diameter']
        center_x, center_y = circle['center']
        data_to_send = f"{diameter:.2f},{center_x},{center_y}\n"  # Format the data as "diameter,center_x,center_y\n"
        ser.write(data_to_send.encode())  # Convert the data to bytes and send to Arduino

    # Close the serial connection
    ser.close()

if __name__ == "__main__":
    min_radius = 10  # Minimum circle radius to detect
    max_radius = 100  # Maximum circle radius to detect
    detect_circles_from_webcam(min_radius, max_radius)
