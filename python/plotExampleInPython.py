"""
ldr.py

Display analog data from Arduino using Python (matplotlib)

Author: Mahesh Venkitachalam
Website: electronut.in
"""

import sys, serial, argparse
import numpy as np
from time import sleep
from collections import deque

import matplotlib.pyplot as plt 
import matplotlib.animation as animation

MAX_X_VAL = 1023
MAX_Y_VAL = 10.0
    
# plot class
class AnalogPlot:
  # constr
  def __init__(self, strPort, maxLen):
      # open serial port
      self.ser = serial.Serial(strPort, 9600)

      self.ax = deque([0.0]*maxLen)
      self.ay = deque([0.0]*maxLen)
      self.az = deque([0.0]*maxLen)
      self.maxLen = maxLen

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

  # update plot
  def update(self, frameNum, a0, a1, a2):
      try:
          line = self.ser.readline()
          print(line.split())
          data = [float(val) for val in line.split()]
          # print data
          print "len(data)==%d"%len(data)

          if(len(data) == 3):
              self.add(data)
              a0.set_data(range(self.maxLen), self.ax)
              a1.set_data(range(self.maxLen), self.ay)
              a2.set_data(range(self.maxLen), self.az)
      except KeyboardInterrupt:
          print('exiting')
      
      return a0, 
  # clean up
  def close(self):
      # close serial
      self.ser.flush()
      self.ser.close()    

# main() function
def main():
  # create parser
  parser = argparse.ArgumentParser(description="LDR serial")
  # add expected arguments
  parser.add_argument('--port', dest='port', required=True)
  # parse args
  args = parser.parse_args()
  #strPort = '/dev/ttyACM1'
  strPort = args.port

  print('reading from serial port %s...' % strPort)

  # plot parameters
  analogPlot = AnalogPlot(strPort, MAX_X_VAL)

  print('plotting data...')
  # set up animation
  fig = plt.figure()
  ax = plt.axes(xlim=(-0, MAX_X_VAL), ylim=(-MAX_Y_VAL, MAX_Y_VAL))
  a0, = ax.plot([], [])
  a1, = ax.plot([], [])
  a2, = ax.plot([], [])
  anim = animation.FuncAnimation(fig, analogPlot.update, fargs=(a0, a1, a2), interval=50)
  # show plot
  plt.show()
  # clean up
  analogPlot.close()

  print('exiting.')

# call main
if __name__ == '__main__':
  main()
