import cv2
import numpy as np
import time
import serial

def detect_circles_from_webcam(min_radius, max_radius):
    try:
        # Open the webcam
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Error: Could not open webcam.")
            return

        # Wait for 2 seconds before starting the circle detection
        print("Waiting for 30 seconds before starting circle detection...")
        time.sleep(2)
        print("Circle detection started!")

        # Create an array to store the center of each detected circle
        circle_centers = []

        while True:
            # Read a frame from the webcam
            ret, frame = cap.read()
            if not ret:
                print("Error: Failed to capture image.")
                break

            # Convert the frame to grayscale
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # Apply Gaussian blur to reduce noise (optional but often helpful)
            gray = cv2.GaussianBlur(gray, (9, 9), 2)

            # Apply Hough Circle Transform
            circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, dp=1, minDist=50,
                                       param1=100, param2=30, minRadius=min_radius, maxRadius=max_radius)

            # If circles are detected, draw them on the frame and store their centers
            if circles is not None:
                circles = np.round(circles[0, :]).astype("int")
                for (x, y, r) in circles:
                    # Draw the circle
                    cv2.circle(frame, (x, y), r, (0, 255, 0), 4)
                    # Draw the center of the circle
                    cv2.circle(frame, (x, y), 1, (0, 0, 255), 3)
                    
                    # Store the center coordinates in the array
                    circle_centers.append((x, y))

            # Display the frame with detected circles and centers
            cv2.imshow("Detected Circles", frame)

            # Press 'q' to exit the loop and close the window after storing 10 circles
            if len(circle_centers) >= 10:
                break

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        # Release the webcam and close all windows
        cap.release()
        cv2.destroyAllWindows()

        # Print the stored circle centers
        for i, center in enumerate(circle_centers):
            print(f"Circle {i+1}: Center: {center}")

        # Send the circle centers to Arduino over the serial connection
        send_circle_centers_to_arduino(circle_centers)

    except Exception as e:
        print(f"An error occurred: {e}")

def send_circle_centers_to_arduino(circle_centers):
    try:
        # Establish serial connection with the Arduino
        ser = serial.Serial('/dev/ttyACM0', 9600)  # Replace '/dev/ttyACM0' with the correct serial port of your Arduino
        time.sleep(2)  # Wait for the serial connection to initialize

        # Send the circle centers to Arduino
        for center in circle_centers:
            center_x, center_y = center
            data_to_send = f"{center_x},{center_y}\n"  # Format the data as "center_x,center_y\n"
            ser.write(data_to_send.encode())  # Convert the data to bytes and send to Arduino
            time.sleep(0.1)  # Add a small delay between sends to avoid overwhelming the serial buffer

        # Close the serial connection
        ser.close()

    except serial.SerialException as e:
        print(f"Serial communication error: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    min_radius = 100  # Minimum circle radius to detect
    max_radius = 150  # Maximum circle radius to detect
    detect_circles_from_webcam(min_radius, max_radius)
