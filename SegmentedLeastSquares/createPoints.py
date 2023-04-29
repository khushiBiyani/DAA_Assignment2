import numpy as np
from segmented import segmented
import random
import subprocess

# Generate some data that follows a segmented linear trend
for i in range(1,1000):
    x = np.sort(np.random.uniform(0, 10, i))
    y = np.concatenate((np.linspace(-10000, 0, int(i/2)), np.linspace(0, 10000, int(i-int(i/2)))))
    for j in range(0,10):
        num = random.uniform(0.0000001, 100000)
    # Save the data points to a text file
        with open('input.txt', 'w') as f:
                f.write(str(i))
                f.write("\n")
                for point_x,point_y in zip(x,y):
                    f.write(f"{point_x} {point_y}\n")
                f.write(str(num))

        subprocess.call("./a")

    
