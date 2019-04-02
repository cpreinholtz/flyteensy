import csv
import matplotlib.pyplot as plt

p=[]
i=[]
d=[]

with open('test_data.txt', 'rb') as csvfile:
  spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
  for row in spamreader:
    #print ', '.join(row)
    #print row[0]
    p.append(float(row[0]))
    i.append(float(row[1]))
    d.append(float(row[2]))
  
  plt.plot(p)
    
    
    
    
