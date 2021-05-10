# -*- coding: utf-8 -*-
"""
Created on Sat Feb 13 18:30:28 2021

@author: mnaca
"""

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

#import matplotlib.animation as animation
#from matplotlib import style
import numpy as np
import serial
import struct
import time

def encontrar_cercano(arr, value, buffer):
    array = np.copy(arr)
    idx = (np.abs(array - value)).argmin()
    if idx > buffer/2:
        array[idx] = 0
    idx = (np.abs(array - value)).argmin()
    return array[idx]


#Inicializar el puerto serial
ser = serial.Serial()
ser.port = 'COM3' # Puerto USB al arduino
ser.baudrate = 9600
ser.timeout = None # Timeout
ser.open()

#fs = 2867.6
fs = 8969
buffer = round(fs/61)*2 # Tclock/Ts Tomo un solo periodo
line = []
x = np.zeros(buffer)
tiempo = (np.arange(0, np.size(x)/fs, 1/fs))*1e3 # Divido fs/2 porque es el tiempo que se toma entre muestra y muestra
#tiempo = (np.arange(0, np.size(x)))


ser.write(b'1')
ser.readline()
print('...')
time.sleep(1)
print('...')

for i in range(0, buffer):
    line = ser.readline()
    line = line.rstrip()
    line = line.decode('ascii', errors = 'ignore')
    try:
        x[i] = line
    except ValueError:
        print(line)
        #x[i] = 0
        i -= 1
        
ser.close()


x = x*5/1023
#tau = tiempo[np.where(x==encontrar_cercano(x, 3.15, buffer))] - tiempo[np.where(x==np.amin(x))]
#print(tau[0], 'ms')

plt.figure()
plt.rc('font', size=20)     
fig, ax = plt.subplots(1)
ax.plot(tiempo, x, color = 'red', linewidth = 2)
#ax.plot(tiempo[np.where(x==encontrar_cercano(x, 3.15, buffer))], x[np.where(x==encontrar_cercano(x, 3.15, buffer))], 'go', markersize =12)
#ax.xaxis.set_major_locator(ticker.MultipleLocator(0.5))
ax.yaxis.set_major_locator(ticker.MultipleLocator(1))
plt.ylim([0,5])
plt.xlim([0,1e3*buffer/fs])
plt.xlabel('Tiempo [ms]')
plt.ylabel('Tension [V]')
plt.grid(True)
fig.show()
