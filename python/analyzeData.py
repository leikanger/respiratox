#!/usr/bin/env python
# -*- coding: utf-8 -*-

#import argparse
import sys # for arguments
import numpy as np
import matplotlib.pyplot as plt 
import scipy.signal as sgn
#import scipy.interpolate as sciInterpol
from time import sleep

# Data class takes filepath to datafile to ctor
class DataReader:
    def __init__(self, dataFilePath):
        try:
            self.data = np.load(dataFilePath)
        except IOError:
            print "Could not open data file at path %s"%dataFilePath

    def get(self):
        return self.data
    def getX(self):
        return self.data[:,0];
    def getY(self):
        return self.data[:,0];
    def getZ(self):
        return self.data[:,0];

def main():
    # Check number of arguments:
    if (len(sys.argv) != 2):
        print "number of arguments is not 3: len = %d ----> %s\nTerminate execution"%(len(sys.argv), sys.argv)
        exit(-1)
    filePath = sys.argv[1]

    data = DataReader(filePath)

    #plt.plot(data.get())
    #plt.show(block=False)
    #sleep(2)

    averagePeriodBetweenSamples = 0.0542#4104#3367;
    plotPeriodograms(data.get(), averagePeriodBetweenSamples)

def getInterestingIndexRange(freqAxis):
    lowerFreq = 0.08    # 0.1 Hz  = 6 RPM ||| 0.08 = 4.8 RPM 
    higherFreq = 1.0 #0.67   # 0.67 Hz = 40 RPM
    # indexRange is [from-index, to-index] given above min/max frequencies
    indexRange = [np.where(freqAxis>lowerFreq)[0][0], np.where(freqAxis>higherFreq)[0][0]]
    return indexRange

def getPeriodogram(dataTimeSeries, sampleTimePeriod):
    #alt 1:
    preparedData = np.fft.fft(dataTimeSeries);
    # remove dc-component (++)
    preparedData[0:5] = 0
    preparedData = np.fft.ifft(preparedData)
    preparedData = removeOutliers(preparedData, 1)

    # zero-pad data with same amount of elements
    preparedData = np.array([preparedData, np.zeros(len(preparedData))]).flatten()
    # window function
    preparedData = preparedData * np.hamming(len(preparedData))

    f, Pxx_den = sgn.periodogram(preparedData, 1/sampleTimePeriod)
    return (f, Pxx_den)

def plotPeriodograms(data, sampleTimePeriod):
    (fX, Pxx_denX) = getPeriodogram(data[:,0],sampleTimePeriod)
    (fY, Pxx_denY) = getPeriodogram(data[:,1],sampleTimePeriod)
    (fZ, Pxx_denZ) = getPeriodogram(data[:,2],sampleTimePeriod)

    interestingXrange = getInterestingIndexRange(fX)
    print interestingXrange
    #pltRange = range(0, len(fX)/2)
    Pxx_denSUM = Pxx_denX+Pxx_denY+Pxx_denZ
    pltRange = range(0, interestingXrange[1])
    plt.semilogy(fX[pltRange]*60, Pxx_denX[pltRange], 'b', label="sensorX")
    plt.semilogy(fY[pltRange]*60, Pxx_denY[pltRange], 'g', label="sensorY")
    plt.semilogy(fZ[pltRange]*60, Pxx_denZ[pltRange], 'm', label="sensorZ")
    #plt.semilogy(fX[pltRange]*60, Pxx_denSUM[pltRange], 'r', label="sensorXYZsum")
    plt.legend()

    # mark max values
    iterMaxX = np.argmax(Pxx_denX[pltRange])
    iterMaxY = np.argmax(Pxx_denY[pltRange])
    iterMaxZ = np.argmax(Pxx_denZ[pltRange])
    iterMaxSUM = np.argmax(Pxx_denSUM[pltRange])
    plt.semilogy(fX[iterMaxX]*60, Pxx_denX[iterMaxX], 'b+', mew=5,ms=10)
    plt.semilogy(fY[iterMaxY]*60, Pxx_denY[iterMaxY], 'g+', mew=5,ms=10)
    plt.semilogy(fZ[iterMaxZ]*60, Pxx_denZ[iterMaxZ], 'm+', mew=5,ms=10)
    #plt.semilogy(fX[iterMaxZ]*60, Pxx_denSUM[iterMaxZ], 'k+', mew=5,ms=10)

    print "iterMaxX = %d --- values (%f, %f)"%(iterMaxX,fX[iterMaxX]*60, Pxx_denX[iterMaxX])
    print "iterMaxY = %d --- values (%f, %f)"%(iterMaxY,fY[iterMaxY]*60, Pxx_denY[iterMaxY])
    print "iterMaxZ = %d --- values (%f, %f)"%(iterMaxZ,fZ[iterMaxZ]*60, Pxx_denZ[iterMaxZ])
    print "iterMaxSUM = %d --- values (%f, %f)"%(iterMaxSUM,fZ[iterMaxSUM]*60, Pxx_denSUM[iterMaxSUM])

    plt.ylim([1e-5, 1e5])
    plt.xlabel('frequency [RPM]')
    plt.ylabel('PSD [V**2/Hz]')
    titleString = "RR = %f RPM"%(fX[iterMaxSUM]*60)
    print titleString
    plt.title(titleString)

    plt.figure()
    plt.plot(data)
    plt.show()

def removeOutliers(data, outlierThreshold):
    outlierIndex = np.where(data>outlierThreshold)
    if (len(outlierIndex[0]) == 0):
        return data # no outliers in data
    #print outlierIndex[0]
    if (outlierIndex[0][0] == 0):
        data[0] = 0
    for iterIndex in outlierIndex:
        data[iterIndex] = 0 #data[iterIndex-1]
    return data

# call main
if __name__ == '__main__':
  main()
    
