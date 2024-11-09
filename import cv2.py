import cv2
import numpy as np
import time
import serial
import struct  # Import the struct module for packing and unpacking data

# Constants for camera calibration (replace these with your calibrated values)
pixels_per_mm_x = 10
pixels_per_mm_y = 10

# Configure the serial port for communication with Arduino
#ser = serial.Serial('/dev/ttyUSB0', 9600)  # Use the appropriate port and baud rate
ser = serial.Serial('COM4', 9600)
def detect_one_circle_from_webcam(min_radius, max_radius):
    # Open the webcam
    cap = cv2.VideoCapture(0)

    # Wait for 2 seconds before starting circle detection
    print("Waiting for 2 seconds before starting circle detection...")
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

        # If a circle is detected, draw it on the frame and calculate the deviation from the center
        if circles is not None:
            circles = np.round(circles[0, :]).astype("int")
            for (x, y, radius) in circles:
                # Draw the circle
                cv2.circle(frame, (x, y), radius, (0, 0, 255), 3)
                
                # Calculate the deviation from the center in millimeters
                x_deviation = (center_x - x) / pixels_per_mm_x
                y_deviation = (center_y - y) / pixels_per_mm_y
                
                # Send the x_deviation and y_deviation to Arduino as integers
                send_data_to_arduino(int(x_deviation), int(y_deviation))
                print("sent to arduino")
                time.sleep(1)

                # Print the deviation
                print(f"x Deviation: {x_deviation} mm, y Deviation: {y_deviation} mm")

        # Display the frame with detected circle and the center
        cv2.imshow("Detected Circle and Center", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the webcam and close all windows
    cap.release()
    cv2.destroyAllWindows()

def send_data_to_arduino(x_deviation, y_deviation):
    # Pack the two integers as signed shorts and send to Arduino
    data_to_send = struct.pack('!bb', x_deviation, y_deviation)
    ser.write(data_to_send)

if __name__ == "__main__":
    min_radius = 10  # Minimum circle radius to detect
    max_radius = 100  # Maximum circle radius to detect
    detect_one_circle_from_webcam(min_radius, max_radius)

