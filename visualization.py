import matplotlib.pyplot as plt
import numpy as np

# Load the data from the text file
with open('input.txt', 'r') as f:
    lines = f.readlines()[1:-1]

x = []
y = []
lineCnt = 0
for line in lines:
    lineCnt += 1
    x_value, y_value = line.split()
    x.append(float(x_value))
    y.append(float(y_value))

plt.xlabel('X')
plt.ylabel('Y')
plt.title('Segmented Least Squares')
plt.plot(x, y, 'ro')


# Load the data from the text file
data = np.loadtxt('output.txt')
slope = data[:, 0]
intercept = data[:, 1]
start = data[:, 2]
end = data[:, 3]

# Plot the lines
for m, b, s, e in zip(slope, intercept, start, end):
    x = np.linspace(s, e, 100)
    y = m * x + b
    plt.plot(x, y)
    # Plot the data
plt.show()
