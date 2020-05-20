import cv2
cam = cv2.VideoCapture(0)
detector=cv2.CascadeClassifier('Classifiers/face.xml')
i=0
offset=50
name=input('Enter your id : ')
while True:
    ret, im =cam.read()
    gray=cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    faces=detector.detectMultiScale(gray, scaleFactor=1.2, minNeighbors=5, minSize=(100, 100), flags=cv2.CASCADE_SCALE_IMAGE) 
    #detects objects of different sizes in input images
    for(x,y,w,h) in faces:
        i=i+1
        cv2.imwrite("dataSet/face-"+name +'.'+ str(i) + ".jpg", gray[y-offset:y+h+offset,x-offset:x+w+offset])
        #name the image stored as dataset
        cv2.rectangle(im,(x-50,y-50),(x+w+50,y+h+50),(225,0,0),2)
        #draw rectangle around the face
        cv2.imshow('im',im[y-offset:y+h+offset,x-offset:x+w+offset])
        cv2.waitKey(100)
    if i>20:
        cam.release()
        cv2.destroyAllWindows()
        break

