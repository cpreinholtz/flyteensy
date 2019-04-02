import serial
import time
import csv
ser = serial.Serial('/dev/ttyACM0')
ser.flushInput()

f=open("test_data.txt","w")
f.close()

while True:
    #try:
        ser_bytes = ser.readline()
        #decoded_bytes = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
        print(ser_bytes)
        with open("test_data.txt","a") as f:#write
          f.write(ser_bytes)

        
        
        
        
