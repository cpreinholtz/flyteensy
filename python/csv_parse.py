import csv
import tkinter
import matplotlib.pyplot as plt

import matplotlib
print(matplotlib.matplotlib_fname())

import matplotlib.rcsetup as rcsetup
print(rcsetup.all_backends)

epoch=[]
p=[]
i=[]
d=[]

with open('test_data.txt', newline='\n') as csvfile:
  spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
  for row in spamreader:
    #print ', '.join(row)
    #print (row[0])
    epoch.append(int(row[0]))
    p.append(float(row[1]))
    i.append(float(row[2]))
    d.append(float(row[3]))
  
  plt.plot(p,label='p')
  plt.plot(i,label='i')
  plt.plot(d,label='d')
  plt.show()
  #print(p)
  
  
  
    
    
    
