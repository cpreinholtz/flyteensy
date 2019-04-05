import serial
import time
import csv
import tkinter
from multiprocessing import Process
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

#import matplotlib
#print(matplotlib.matplotlib_fname())
#import matplotlib.rcsetup as rcsetup
#print(rcsetup.all_backends)





    
    
    

##################################################################
#Serial
ser = serial.Serial('/dev/ttyACM0')

maxSamples=250*10  #(250 hz * 10 seconds)
refreshAt=80
refresh=0


epoch=[]
ep=[]
ei=[]
ed=[]
result=[]
kp=0
ki=0
kd=0


epoch_index=0
ep_index=1
ei_index=2
ed_index=3
result_index=4
kp_index=5
ki_index=6
kd_index=7
total_len=8



##################################################################
#Plotting
style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

def animate():
  global ax1
  global epoch, ep, ei, ed, result

  ax1.clear()
  #ax1.plot()
  ax1.plot(ep,label='error.p')
  ax1.plot(ei,label='error.i')
  ax1.plot(ed,label='error.d')
  ax1.plot(result,label='result')
  ax1.ylim(-75,75)
  ax1.legend()
  plt.pause(0.000001)
  
  



  





ser.flushInput()
ser_bytes = ser.readline()
ser.flushInput()


while True:
  try:

    ser_bytes = ser.readline()
    #print(ser_bytes) 
    row=str(ser_bytes).split(",")
    #print(row)
    row[epoch_index]
     
    if len(row)<total_len:
      print("str too small")
      continue
      
    else:          
    
      epoch.append( float( row[epoch_index][2:] ) )  #for some reason arduino puts b' in front of epoch    
      ep.append(float(row[ep_index]))
      ei.append(float(row[ei_index]))
      ed.append(float(row[ed_index]))     
      result.append(float(row[result_index]))    
      kp=float(row[kp_index])
      ki=float(row[ki_index])
      kd=float(row[kd_index])

      if len(ep)>maxSamples:
        epoch.pop(0)
        ep.pop(0)
        ei.pop(0)
        ed.pop(0)
        result.pop(0)      
      
    refresh=refresh+1  
    if refresh>=refreshAt:
      print("hit refresh")
      #p = Process(target=plot_graph, args=([epoch,ep,ei,ed,result]) )      
      #p.start()
      refresh=0
      animate()
      print("refreshing")
      #p.join()

  except KeyboardInterrupt:    print("quitting"); exit()
  except TypeError:    print("type error"); 
  except Exception as e: print(e)
        


