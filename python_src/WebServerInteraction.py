from SimpleCV import Camera,Display,Image
from elaphe import barcode
import time
cam=Camera()

from flask import Flask,send_file,request
app=Flask(__name__)

@app.route('/')
def SendImage():
    newImg=cam.getImage()
    newImg.save("test.jpg")
    return send_file('test.jpg', mimetype='text') 


    


if __name__=='__main__':
    app.run(host='0.0.0.0') #Grants everyone access

    
