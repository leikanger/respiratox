#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
ldr.py

Display analog data from Arduino using Python (matplotlib)

Author: Mahesh Venkitachalam
Website: electronut.in
"""

import sys, serial, argparse
import numpy as np
from time import sleep
import time
from collections import deque

import matplotlib.pyplot as plt 
import matplotlib.animation as animation

MAX_X_VAL = 500
MAX_Y_VAL = 10.0
    
# plot class
class AnalogPlot:
  # constr
  def __init__(self, strPort, maxLen, logLenghtInSamples):
      # open serial port
      self.ser = serial.Serial(strPort, 9600)

      self.ax = deque([0.0]*maxLen)
      self.ay = deque([0.0]*maxLen)
      self.az = deque([0.0]*maxLen)
      self.aSum = deque([0.0]*maxLen)
      self.maxLen = maxLen

      if (logLenghtInSamples > 0):
        self.dataLogger = DataLogger("./data.dat",logLenghtInSamples)
      else:
        self.dataLogger = DataLogger("/dev/null",0)

  # add to buffer
  def addToBuf(self, buf, val):
      if len(buf) < self.maxLen:
          buf.append(val)
      else:
          buf.pop()
          buf.appendleft(val)

  # add data
  def add(self, data):
      assert(len(data) == 3)
      self.addToBuf(self.ax, data[0])
      self.addToBuf(self.ay, data[1])
      self.addToBuf(self.az, data[2])
      self.addToBuf(self.aSum, 0)

      self.dataLogger.add3vectorValue(data)

  # update plot
  def update(self, frameNum, a0, a1, a2, a3):
      try:
          line = self.ser.readline()
#          print(line.split())
          data = [float(val) for val in line.split()]

          if(len(data) == 3):
              self.add(data)
              a0.set_data(range(self.maxLen), self.ax)
              a1.set_data(range(self.maxLen), self.ay)
              a2.set_data(range(self.maxLen), self.az)
              a3.set_data(range(self.maxLen), self.aSum)
          if(self.dataLogger.finished()):
              print "Data logger is offline -- value is not logged to file"
      except KeyboardInterrupt:
          print('exiting')
      
      return a0, 
  def plotLog(self):
      self.dataLogger.plotData();
  # clean up
  def close(self):
      # close serial
      self.ser.flush()
      self.ser.close()    
      self.dataLogger.save();
      print "Average time interval: %f"%self.dataLogger.getAverageTimeInterval()



## PLAN: Make a class that handles data untill N is reached, then save result to disk and somehow message this to the outside world

# logging class
class DataLogger:
  def __init__(self, fileSavePathARG, dataLength):
      # Just for checking timestamps (time between samples)
      self.timeStamps = np.zeros(dataLength).astype(float)
      # create data numpy-array
      self.data = np.zeros((dataLength,3)).astype(float)
      self.dataLength = dataLength
      # helpers variables
      self.numberOfSamlesAcquired = 0
      self.bFinishedAcquiringData = False
      # file path for data file
      self.fileSavePath = fileSavePathARG

  def add3vectorValue(self, argXYZval):
      assert(len(argXYZval)==3)
      self.numberOfSamlesAcquired = self.numberOfSamlesAcquired+1
      if(self.numberOfSamlesAcquired >= self.dataLength):
        self.bFinishedAcquiringData = True
        return
      # Limit maximum value to absolute value 20 - set to zero in case of error
      if (np.abs(argXYZval[0])>20):
          argXYZval[0] = 0
          print "WARNING: value error on X value - set to zero"
      if (np.abs(argXYZval[1])>20):
          argXYZval[1] = 0
          print "WARNING: value error on Y value - set to zero"
      if (np.abs(argXYZval[2])>20):
          argXYZval[2] = 0
          print "WARNING: value error on Z value - set to zero"
      #vector = np.array(argXYZval, dtype=float)
      self.data[self.numberOfSamlesAcquired] = np.array(argXYZval,dtype=float)
      #self.data[self.numberOfSamlesAcquired] = np.array((argXYZval[0],argXYZval[1],argXYZval[2]))
      self.timeStamps[self.numberOfSamlesAcquired] = time.time()

  def getAverageTimeInterval(self):
      plt.plot(np.diff(self.timeStamps[2:]))
      plt.show()
      return np.average(np.diff(self.timeStamps[2:]))

  def finished(self):
      return self.bFinishedAcquiringData

  def save(self):
      np.save(self.fileSavePath, self.data);

  def plotData(self):
      plt.figure()
      plt.plot(self.data)
      plt.show(block=False)


# TODO: Lag -noplot funksjonalitet for å bare lagre data
# TODO: Lag -silent funksjonalitet for å unngå å skrive ut
# main() function
def main():
  # create parser
  parser = argparse.ArgumentParser(description="LDR serial")
  # add expected arguments
  parser.add_argument('--port', dest='port', required=True)
  parser.add_argument('--log', dest='logLength', default=0)
  # parse args
  args = parser.parse_args()
  #strPort = '/dev/ttyACM1'
  strPort = args.port
  samplesInDataLog = args.logLength

  print('reading from serial port %s...' % strPort)

  # plot parameters
  analogPlot = AnalogPlot(strPort, MAX_X_VAL, int(samplesInDataLog))

  print('plotting data...')
  # set up animation
  fig = plt.figure()
  ax = plt.axes(xlim=(0, MAX_X_VAL), ylim=(-MAX_Y_VAL, MAX_Y_VAL))
  #ax = plt.axes(xlim=(0, MAX_X_VAL), ylim=(-MAX_Y_VAL, MAX_Y_VAL))
  a0, = ax.plot([], [])
  a1, = ax.plot([], [])
  a2, = ax.plot([], [])
  a3, = ax.plot([], [])
  anim = animation.FuncAnimation(fig, analogPlot.update, fargs=(a0, a1, a2, a3), interval=50)
  # show plot
  plt.show()
  # clean up
  analogPlot.close()
  analogPlot.plotLog()

  print('exiting.')

# call main
if __name__ == '__main__':
  main()
