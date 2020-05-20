import serial
import time
import cv2,os
import numpy as np
from PIL import Image 
import pickle
#import sched, time

#def do_something(sc): 
#    print('Doing stuff')
    # do your stuff
    #s.enter(60, 1, do_something, (sc,))
a_count=0
b_count=0
c_count=0
d_count=0
u_count=0
flag=0
recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read('trainer/trainer.yml')
cascadePath = "Classifiers/face.xml"
faceCascade = cv2.CascadeClassifier(cascadePath);
path = 'dataSet'
#s = sched.scheduler(time.time, time.sleep)

#cam = cv2.VideoCapture(0)
#print('Welcome')
ser = serial.Serial('COM11')  # open serial port, check system 
#print(ser.name)
ser.write(b'X')
#font = cv2.InitFont(cv2.CV_FONT_HERSHEY_SIMPLEX, 1, 1, 0, 1, 1) #Creates a font
print('Welcome, system initializing..') 
while True:
    if(flag==0):
        cam = cv2.VideoCapture(1)
        time.sleep(5)
        #cam.release()
        print('Awaiting Fingerprint Data ..')
        #cv2.destroyAllWindows()
        data=ser.read(1)
        mydata = int(data)
        if(mydata==1 or mydata==2 or mydata==3 or mydata==4):
            print('Person detected by sensor .. initializing script...')
            flag=1
        elif(mydata==7):
            print('Exiting')
            cam.release()
            cv2.destroyAllWindows()
            break
        #if(mydata==3):
        #    cv2.destroyAllWindows()
        #    print('Awaiting Trigger ..')
        #    cam.release()   
    if(flag==1):
        print('Recognizing ... Please wait')
        #s.enter(60, 1, do_something, (s,))
        #s.run()
        ret, im =cam.read()
        cv2.imshow('im',im)
        gray=cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
        faces=faceCascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(100, 100), flags=cv2.CASCADE_SCALE_IMAGE)
        for(x,y,w,h) in faces:
            nbr_predicted, conf = recognizer.predict(gray[y:y+h,x:x+w])
            cv2.rectangle(im,(x-50,y-50),(x+w+50,y+h+50),(225,0,0),2)
            if(nbr_predicted==1  and conf<40):
                nbr_predicted='Mandy'
                print('User 1 detected')
                a_count=a_count+1
                u_count = 0
                if a_count > 5:
                    ser.write(b'1')
                    flag = 0
                    print('Please proceed to vote')
                    #cv2.destroyAllWindows()
                time.sleep(1)
                #flag = 0
                #print('Flag set to 0') 
            elif(nbr_predicted==2  and conf<40):
                nbr_predicted='Madhu'
                print('User 2 detected')
                b_count=b_count+1
                u_count = 0
                if b_count > 5:
                    ser.write(b'2')
                    flag = 0
                time.sleep(1)
                #flag = 0
            elif(nbr_predicted==3  and conf<40):
                nbr_predicted='Babloo'
                print('User 3 detected')
                c_count=c_count+1
                u_count = 0
                if c_count > 5:
                    ser.write(b'3')
                    flag = 0
                time.sleep(1)
                #flag = 0
            elif(nbr_predicted==4  and conf<40):
                nbr_predicted='Bishan'
                print('User 4 detected')
                d_count=d_count+1
                u_count = 0
                if d_count > 5:
                    ser.write(b'4')
                    flag = 0
                time.sleep(1)
                #flag = 0             
            else:
                print('Unknown Person ...')
                time.sleep(0.5)
                u_count=u_count+1
                a_count = 0
                b_count = 0
                c_count = 0
                d_count = 0
                if(u_count==4):
                    ser.write(b'X')
                    flag = 0
            #cv2.PutText(cv2.fromarray(im),str(nbr_predicted)+"--"+str(conf), (x,y+h),font, 255) #Draw the text
            font = cv2.FONT_HERSHEY_SIMPLEX
            #name = labels[id_]
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





