# mrl_blend.py
# run AFTER calibrate.py to mix data from calibrate.py

import cv2
import dlib
import numpy as np
import json
import os
from scipy.spatial import distance

detector  = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("/home/artin59/Desktop/EECS 3216/DriverMonitoringSystem/cv/shape_predictor_68_face_landmarks.dat")

LEFT_EYE  = list(range(42, 48))
RIGHT_EYE = list(range(36, 42))

def Detect_Eye(landmarks, indices):
    pts = [(landmarks.part(i).x, landmarks.part(i).y) for i in indices]
    point1 = distance.euclidean(pts[1], pts[5])
    point2 = distance.euclidean(pts[2], pts[4])
    point3 = distance.euclidean(pts[0], pts[3])
    return (point1 + point2) / (2.0 * point3)

MRL_ROOT   = r"G:\CODING STUFF\cv\data"
MAX_IMAGES = 200

def process_dataset(root, max_per_label=200):
    open_count   = 0
    closed_count = 0

    for subfolder in os.listdir(root):
        subfolder_path = os.path.join(root, subfolder)
        if not os.path.isdir(subfolder_path):
            continue

        if open_count >= max_per_label and closed_count >= max_per_label:
            break

        for inner_folder in os.listdir(subfolder_path):
            inner_path = os.path.join(subfolder_path, inner_folder)
            if not os.path.isdir(inner_path):
                continue

            for fname in os.listdir(inner_path):
                if not fname.lower().endswith(('.png', '.jpg')):
                    continue

                parts = fname.replace('.png','').replace('.jpg','').split('_')
                if len(parts) < 5:
                    continue
                try:
                    eye_state = int(parts[4])
                except ValueError:
                    continue

                if eye_state == 1 and open_count   >= max_per_label:
                    continue
                if eye_state == 0 and closed_count >= max_per_label:
                    continue

                if eye_state == 1:
                    open_count += 1
                else:
                    closed_count += 1

                print(f"Counted: {fname} | eye_state: {eye_state} | open: {open_count}/{max_per_label} closed: {closed_count}/{max_per_label}")

    total        = open_count + closed_count
    open_ratio   = open_count   / total if total > 0 else 0.5
    closed_ratio = closed_count / total if total > 0 else 0.5

    print(f"MRL open count:   {open_count} ({open_ratio:.1%})")
    print(f"MRL closed count: {closed_count} ({closed_ratio:.1%})")

    return open_count, closed_count


# load json from calibrate.py
script_dir       = os.path.dirname(os.path.abspath(__file__))
calibration_path = os.path.join(script_dir, "my_eye_data.json")

if not os.path.exists(calibration_path):
    print("ERROR: my_eye_data.json not found. Run calibrate.py first.")
    exit()

with open(calibration_path) as f:
    personal = json.load(f)

if any(personal.get(k) is None or np.isnan(personal.get(k, float('nan')))
       for k in ["open_avg", "closed_avg"]):
    print("ERROR: personal calibration contains nan. Re-run calibrate.py first.")
    exit()

print(f"Personal open avg:   {personal['open_avg']}")
print(f"Personal closed avg: {personal['closed_avg']}")
print(f"\nProcessing MRL dataset from: {MRL_ROOT}")

mrl_open_count, mrl_closed_count = process_dataset(MRL_ROOT, MAX_IMAGES)

if mrl_open_count == 0 or mrl_closed_count == 0:
    print(f"ERROR: MRL processing failed — open: {mrl_open_count}, closed: {mrl_closed_count}")
    exit()

# Threshold is midpoint between personal open and closed avgs
blended_open_avg   = personal["open_avg"]
blended_closed_avg = personal["closed_avg"]
final_threshold    = round(blended_closed_avg + (blended_open_avg - blended_closed_avg) * 0.5, 3)

output = {
    "threshold":             final_threshold,
    "open_avg":              round(float(blended_open_avg),   3),
    "closed_avg":            round(float(blended_closed_avg), 3),
    "mrl_open_count":        mrl_open_count,
    "mrl_closed_count":      mrl_closed_count,
    "personal_open_avg":     personal["open_avg"],
    "personal_closed_avg":   personal["closed_avg"],
}

with open(calibration_path, "w") as f:
    json.dump(output, f, indent=2)

print(f"\nBlended calibration saved")
print(f"   Personal open avg:   {output['personal_open_avg']}")
print(f"   Personal closed avg: {output['personal_closed_avg']}")
print(f"   Final threshold:     {final_threshold}")