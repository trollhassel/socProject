import serial #Serial port API http://pyserial.sourceforge.net/pyserial_api.html
import socket
import time
import smbus #Enables the python bindings for I2C 
from threading import Thread

bus = smbus.SMBus(1) #opens /dev/i2c-1 
address=0x52 #the Nunchuk I2C address 
bus.write_byte_data(address,0x40,0x00) 
bus.write_byte_data(address,0xF0,0x55) 
bus.write_byte_data(address,0xFB,0x00) 
incX=0
incY=0

port = "/dev/ttyACM0"
UDP_IP = "0.0.0.0"
UDP_PORT = 9050

SerialIOArduino = serial.Serial(port,9600) # setup the serial port and baudrate
SerialIOArduino.flushInput() # Remove old input's

def recvUDP(sock,SerialIOArduino):
    while True:
        data, addr = sock.recvfrom(1280) # Max recieve size is 1280 bytes
        print "UDP RECIEVED: ", data.strip()
        SerialIOArduino.write(data)
        findIP = data.split(",")
        if (findIP[0] == "$IP"):
            global UDP_IP
            UDP_IP = findIP[1]
        print "findIP: ", findIP[0]


 
print "UDP target IP:",   UDP_IP
print "UDP target port:", UDP_PORT

sock = socket.socket(socket.AF_INET,     # Internet protocol
                     socket.SOCK_DGRAM)  # User Datagram (UDP)
sock.bind(("0.0.0.0", UDP_PORT))           # Listen on all adapters


t = Thread(target=recvUDP,args=(sock,SerialIOArduino,))
t.daemon=True # Stop thread when program ends
t.start()

while True:
    if (SerialIOArduino.inWaiting() > 0):
        inputLine = SerialIOArduino.readline().strip()  # read a '\n' terminated line()

        # Send the csv string as a UDP message
        sock.sendto(inputLine, (UDP_IP, UDP_PORT))

    ## Nunchack controller
    bus.write_byte(address,0x00) 
    time.sleep(0.1) 
    data= bus.read_i2c_block_data(address, 0x00, 6)
    joy_x = data[0]
    joy_y = data[1]

    if(joy_x>210):
        SerialIOArduino.write("$CAM, 0, 10, \n")
    elif (joy_x<90):
        SerialIOArduino.write("$CAM, 0, -10, \n")
    if(joy_y>210):
        SerialIOArduino.write("$CAM, 10, 0, \n")
    elif (joy_y<90):
        SerialIOArduino.write("$CAM, -10, 0, \n")
    


