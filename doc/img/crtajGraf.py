import matplotlib.pyplot as plt
import numpy as np
import matplotlib.mlab as mlab
import math

import cairo
import pylab

x1 = np.linspace(-10,10,500)

# xs1 = [0, 1, 2, 3, 4, 5, 10, 15, 22 ]
# ys1 = [0, 0.45, 0.5, 0.61, 0.66, 0.69, 0.84, 0.89, 0.92]

# xs1 = [0, 1, 2, 3, 4, 5, 6, 7, 10]
# ys1 = [0, 0.62, 0.73, 0.88, 0.94, 0.94, 0.94, 0.98, 0.98]

xs1 = [0, 1, 2, 3, 4, 5, 6, 7, 10]
ys1 = [0, 0.54, 0.75, 0.80, 0.86, 0.89, 0.96, 0.98, 0.98]

pylab.plot(xs1, ys1, marker=".", ls="-", color="b", label="")

pylab.xlabel("$n$")
pylab.ylabel("$tocnost$")

pylab.axis([0, 10, 0, 1])
# pylab.legend(numpoints=1, prop={'size':'12'})
pylab.legend(numpoints=1)
pylab.grid(True)

pylab.show()
