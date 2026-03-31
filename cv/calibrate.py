import cv2
import dlib
import numpy as np
import json
import os
from scipy.spatial import distance

detector  = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("/home/artin59/Desktop/EECS 3216/DriverMonitoringSystem/cv/shape_predictor_68_face_landmarks.dat") #change to actual directory
script_dir = os.path.dirname(os.path.abspath(__file__))

LEFT_EYE  = list(range(42, 48))
RIGHT_EYE = list(range(36, 42))

def Detect_Eye(landmarks, indices):
    pts = [(landmarks.part(i).x, landmarks.part(i).y) for i in indices]
    point1 = distance.euclidean(pts[1], pts[5])
    point2 = distance.euclidean(pts[2], pts[4])
    point3 = distance.euclidean(pts[0], pts[3])
    return (point1 + point2) / (2.0 * point3)

def collect_samples(camera, label, seconds=4):
    print(f"\n>>> Keep your eyes {label.upper()} and press SPACE to start...")

    # wait for spacebar
    while True:
        _, frame = camera.read()
        cv2.putText(frame, f"Eyes {label.upper()} - press SPACE to start",
                    (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)
        cv2.imshow("Calibration", frame)
        if cv2.waitKey(1) & 0xFF == ord(' '):
            break

    print(f"Collecting {seconds} seconds of '{label}' data...")
    samples = []
    start = cv2.getTickCount()

    while True:
        _, frame = camera.read()
        gray  = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = detector(gray)
        elapsed = (cv2.getTickCount() - start) / cv2.getTickFrequency()
        remaining = max(0, seconds - int(elapsed))

        for face in faces:
            lms = predictor(gray, face)

            # draw shape around eyes 
            for n in range(42, 48):
                x, y = lms.part(n).x, lms.part(n).y
                next_n = 42 if n == 47 else n + 1
                cv2.line(frame, (x, y),
                         (lms.part(next_n).x, lms.part(next_n).y),
                         (0, 255, 0), 1)
            for n in range(36, 42):
                x, y = lms.part(n).x, lms.part(n).y
                next_n = 36 if n == 41 else n + 1
                cv2.line(frame, (x, y),
                         (lms.part(next_n).x, lms.part(next_n).y),
                         (0, 255, 0), 1)

            ear = (Detect_Eye(lms, LEFT_EYE) + Detect_Eye(lms, RIGHT_EYE)) / 2
            samples.append(round(ear, 4))

            cv2.putText(frame, f"EAR: {ear:.3f}",
                        (20, 40), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 0), 2)

        cv2.putText(frame, f"Eyes {label.upper()} - {remaining}s remaining",
                    (20, 80), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)
        cv2.putText(frame, f"Samples: {len(samples)}",
                    (20, 120), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0), 2)

        cv2.imshow("Calibration", frame)
        cv2.waitKey(1)

        if elapsed >= seconds:
            break

    print(f"  Collected {len(samples)} samples. Avg EAR: {np.mean(samples):.3f}")
    return samples


#run calibration
camera = cv2.VideoCapture(0)  # use same index as main script

print("=== Personal EAR Calibration ===")
print("You will be asked to hold your eyes OPEN, then CLOSED.")
print("Press SPACE when ready for each one.\n")

open_samples   = collect_samples(camera, "open",   seconds=4)
closed_samples = collect_samples(camera, "closed", seconds=4)

camera.release()
cv2.destroyAllWindows()

open_avg   = float(np.mean(open_samples))
closed_avg = float(np.mean(closed_samples))
threshold  = round((open_avg + closed_avg) / 2, 3) #the middle line between open and closed, rounded to 3 decimals

data = {
    "open_avg":       round(open_avg,   3),
    "closed_avg":     round(closed_avg, 3),
    "threshold":      threshold,
    "open_samples":   [round(x, 4) for x in open_samples],
    "closed_samples": [round(x, 4) for x in closed_samples]
}

with open(os.path.join(script_dir, "my_eye_data.json"), "w") as f: #saves data to "me_eye_data.json"
    json.dump(data, f, indent=2)

print(f"\n Done! Results:")
print(f"   Eyes open avg EAR:   {open_avg:.3f}")
print(f"   Eyes closed avg EAR: {closed_avg:.3f}")
print(f"   Your personal threshold: {threshold}")
print(f"   Saved to my_eye_data.json")