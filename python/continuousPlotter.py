#! /usr/bin/ipython

import matplotlib.pyplot as plt
import numpy as np
import time


class continuousPlotter:
    """A instance-generator for continous plotting"""

    def __init__(self):
        # some X and Y data
        self.x = np.arange(10000)
        self.data = np.random.randn(10000)
        
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111)
        self.li, = self.ax.plot(self.x, self.data)

        # draw and show it
        self.fig.canvas.draw()
        plt.show(block=False)

    def plotNewValues(self, newValues):
        #update plot with vector [newValues] (of arbiturary length)
        lengthOfNewData = len(newValues)

        #try:
        self.data[:-lengthOfNewData] = self.data[lengthOfNewData:]
            # move values N positions to the left.
        self.data[-lengthOfNewData:] = newValues
            # insert the new (N) values in the right of data vector
        self.li.set_ydata(self.data)
            # update plot data
        self.fig.canvas.draw()
            # redraw plot
        #except KeyboardInterrupt:
            


if __name__ == "__main__":
    import numpy as np
    import time

    sig = np.sin( np.linspace(0, 2*np.pi, 10000) )

    mittPlottevindu = continuousPlotter();
    N = 30; # Ti verdier om gangen..
    for nextStartOfRange in range(1,len(sig), N):
        plottingRange = sig[nextStartOfRange:nextStartOfRange+N]
        mittPlottevindu.plotNewValues( plottingRange )
        time.sleep(0.001)
    
