# -*- coding: utf-8 -*-

import numpy as np
import scipy.interpolate
import matplotlib.pyplot as plt
import warnings

def create_data(n):
    """Given an integer n, returns n data points
    x and values y as a numpy.array."""
    xmax = 52.
    s = 0.05 #Statisme, cf Enedis values
    fn = 50.
    g = 1./(s * fn) # linear slope handles decreased active power 
    f1 = 50.2 # overfrequency threshold
    Pmin = 0.6 #P/Pmax = 60%, Pmin

    x = np.linspace(50, xmax, n) # frequency range

    # Adding the condition for power (y) values
    y = np.ones_like(x)  # Initialize power with Pmax normalized value 

    try :
      # Update y values only for elements where x is greater than or equal to f1
      mask = x >= f1
      y[mask] = 1 + g * (f1 - x[mask])

      #Uncomment the line below if we want to add some noise to y
      # y += 1. * np.random.normal(size=len(x))

      # Check if any y value is less than Pmin threshold
      if np.any(y < Pmin):
        raise ValueError("Linear variation cannot be confirmed for y < Pmin.\
        Set y to a default value if no instruction")

    except ValueError as e:
        #print(f"Error: {e}")
        warnings.warn(str(e), UserWarning)
        # Handle the error. for ex. : set y values to a default value
        #y = np.ones_like(x)

    return x, y

#main program
n = 20
x = []
y = []
x, y = create_data(n)

# refine the mesh for interpolated plot
xfine = np.linspace(50.01, 52.0, n * 100)

#interpolate with piecewise constant function (p=0)
#y0 = scipy.interpolate.interp1d(x, y, kind='nearest')

#interpolate with piecewise linear func (p=1)
y1 = scipy.interpolate.interp1d(x, y, kind='linear')

#interpolate with piecewise constant func (p=2)
y2 = scipy.interpolate.interp1d(x, y, kind='quadratic')

plt.plot(x, y, 'o', label='data point', alpha=0.2)
#pylab.plot(xfine, y0(xfine), label='nearest')

plt.plot(xfine, y1(xfine), label='linear')
plt.plot(xfine, y2(xfine), label='cubic')

fmeasure = 50.3105217 # simulate mHz accuracy Siemens Q200 Power Analyzer measure
plt.plot(fmeasure, y2(fmeasure), '*')
plt.plot(fmeasure, y1(fmeasure), '*', color='red')

plt.grid(axis='x', color='0.95')
plt.grid(axis='y', color='0.95')

plt.legend(title='Data and Interpolation:')
plt.title('LFMO-O : Active power response when overfrequency occurred')

plt.xlabel('Frequency Hz')
plt.ylabel('P/Pmax')

plt.show()

# Print the P/Pmax = func(freq) gauge data in a zip format
for x_val, y_val in zip(x, y):
    print(f"x: {x_val:.3f}, y: {y_val:.3f}")
