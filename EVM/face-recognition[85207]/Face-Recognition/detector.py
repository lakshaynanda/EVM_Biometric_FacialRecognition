import serial
import time
import cv2,os
import numpy as np
from PIL import Image 
import pickle

recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('trainer/trainer.yml')
cascadePath = "Classifiers/face.xml"
faceCascade = cv2.CascadeClassifier(cascadePath);
path = 'dataSet'

cam = cv2.VideoCapture(0)
print('Welcome')
while True:
    ret, im =cam.read()
    gray=cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    faces=faceCascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(100, 100), flags=cv2.CASCADE_SCALE_IMAGE)
    for(x,y,w,h) in faces:
        nbr_predicted, conf = recognizer.predict(gray[y:y+h,x:x+w])
        cv2.rectangle(im,(x-50,y-50),(x+w+50,y+h+50),(225,0,0),2)
        if(nbr_predicted==1 and conf < 35):
             nbr_predicted='Lakshay'
             
        elif(nbr_predicted==2 and conf < 35):
             nbr_predicted='Sir'
             
        elif(nbr_predicted==3 and conf < 35):
             nbr_predicted='Maam'
             
        elif(nbr_predicted==4 and conf < 35):
             nbr_predicted='Sir2'            
        else:
            nbr_predicted='Unknown' 
        
        font = cv2.FONT_HERSHEY_SIMPLEX
        
        color = (255, 255, 255)
        stroke = 3
        cv2.putText(im, str(nbr_predicted)+"--"+str(conf), (x,y+h), font, 1, color, stroke, cv2.LINE_AA)
        #+"--"+str(conf)
        cv2.imshow('im',im)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()






