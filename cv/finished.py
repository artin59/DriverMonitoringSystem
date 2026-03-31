#pip install numpy, opencv-python, pyttsx3, dlib, scipy, python-can
#python 3.11
#pyttsx3 future sound implementation

import cv2
import dlib
import pyttsx3
import time
import can
import json
import os
from scipy.spatial import distance


#change for testing
#bus = can.interface.Bus(interface = 'socketcan', channel = 'can0', bitrate = 500000)
bus = can.interface.Bus(interface='virtual', channel='test')


script_dir = os.path.dirname(os.path.abspath(__file__))
CALIBRATION_FILE = os.path.join(script_dir, "my_eye_data.json")
if os.path.exists(CALIBRATION_FILE):
    with open(CALIBRATION_FILE) as f:
        cal = json.load(f)
    EAR_THRESHOLD = cal["threshold"]
    print(f"Using personal threshold: {EAR_THRESHOLD}")
else:
    EAR_THRESHOLD = 0.2


EYES_CLOSED_ID  = 0x064
EYES_CLOSED_MSG = can.Message(arbitration_id=EYES_CLOSED_ID, data=[0x01], is_extended_id=False)
EYES_OPEN_MSG   = can.Message(arbitration_id=EYES_CLOSED_ID, data=[0x00], is_extended_id=False)


def send_alert(eyes_closed: bool):
    msg = EYES_CLOSED_MSG if eyes_closed else EYES_OPEN_MSG
    try:
        bus.send(msg)
        print(f"Sent: {'EYES CLOSED' if eyes_closed else 'eyes open'}")
    except can.CanError as e:
        print(f"CAN send error: {e}")



camera = cv2.VideoCapture(0) #change index to 1, 2 etc. if camera not working

face_detector = dlib.get_frontal_face_detector()
dlib_facelandmark = dlib.shape_predictor("/home/artin59/Desktop/EECS 3216/DriverMonitoringSystem/cv/shape_predictor_68_face_landmarks.dat") #change to actual directory

def Detect_Eye(eye):
    point1 = distance.euclidean(eye[1], eye[5])
    point2 = distance.euclidean(eye[2], eye[4])
    point3 = distance.euclidean(eye[0], eye[3]) #landmarks from opencv face landmarks
    EAR = (point1+point2)/(2*point3)
    return EAR #eye aspect ratio to detect open/close

sleep_timer = None
eyes_closed = False
previous_alert = None
while True:
    null, frame = camera.read()
    gray_scale = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_detector(gray_scale)
    
    if len(faces) == 0:
        sleep_timer = None
        eyes_closed = False

    for face in faces:
        face_landmarks = dlib_facelandmark(gray_scale, face)
        eye1 = []
        eye2 = []

        for n in range(42, 48): #enumeration for eye landmarks in landmark.dat
            x = face_landmarks.part(n).x
            y = face_landmarks.part(n).y
            eye1.append((x,y))
            next_point = n+1
            if n == 47:
                next_point = 42
            x2 = face_landmarks.part(next_point).x
            y2 = face_landmarks.part(next_point).y
            cv2.line(frame, (x,y), (x2,y2), (0,255,0), 1) #draw a green shape for the left eyes

        for n in range(36, 42): #enumeration for eye landmarks in landmark.dat
            x = face_landmarks.part(n).x
            y = face_landmarks.part(n).y
            eye2.append((x,y))
            next_point = n+1
            if n == 41:
                next_point = 36
            x2 = face_landmarks.part(next_point).x
            y2 = face_landmarks.part(next_point).y
            cv2.line(frame, (x,y), (x2,y2), (0,255,0), 1) #draw a green shape for the right eyes

        eyeLeft = Detect_Eye(eye1)
        eyeRight = Detect_Eye(eye2)
        EAR = (eyeLeft+eyeRight)/2

        EAR = round(EAR, 2)


        
        if EAR < EAR_THRESHOLD: #threshold for eye close imported from calibration file
            if sleep_timer is None: #to set a threshold for the alert(seconds passed)
                sleep_timer = time.time()
            time_passed = time.time() - sleep_timer

            if time_passed >= 1: #x seconds passed for alert to go off
                cv2.putText(frame, "DROWSINESS DETECTED", (50, 100), cv2.FONT_HERSHEY_PLAIN, 2, (21, 56, 210), 3)
                cv2.putText(frame, "Alert!!!! DRIVER ASLEEP", (50, 450), cv2.FONT_HERSHEY_PLAIN, 2, (21, 56, 212), 3)
                eyes_closed = True
        else:
            sleep_timer = None
            eyes_closed = False
        
        if eyes_closed != previous_alert:
            send_alert(eyes_closed)
            previous_alert = eyes_closed
        

    cv2.imshow("Drowsiness detector in OPENCV2(ESC to exit)", frame)
    key = cv2.waitKey(9)
    if key == 27:
        break

camera.release()
cv2.destroyAllWindows()
