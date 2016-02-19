#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
onlyLogging.py

Display data from Eupnea data from Arduino using Python

Author: Per Leikanger
"""

import sys, serial, argparse
import numpy as np
import time
import matplotlib.pyplot as plt



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
  logLenghtInSamples = int(args.logLength)

  print('reading from serial port %s...' % strPort)

  # Create data logger
  if (logLenghtInSamples > 0):
    dataLogger = DataLogger("./data.dat",logLenghtInSamples)
  else:
    print "You need to specify number of samples to log"
    sys.exit(0)

  # establish serial connection
  ser = serial.Serial(strPort, 9600)

  while (1):
    if (dataLogger.finished()):
        print "Data logger is offline -- terminating logger"
        break
    line = ser.readline()
    # split line into individual values
    data = [float(val) for val in line.split()]
    dataLogger.add3vectorValue(data)
    time.sleep(0.02)

  # finish up
  dataLogger.plotData();
  dataLogger.save();
 
  print "Average time interval: %f"%dataLogger.getAverageTimeInterval()

# call main
if __name__ == '__main__':
  main()
