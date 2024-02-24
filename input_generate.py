import numpy as np

i = int(input("Enter number of points: "))

points = []
for j in range(i):
    points.append((float(100 * np.random.rand()), float(100 * np.random.rand())))

file = open("test_input.txt", "w")
file.write(str(len(points)) + "\n")
for j in points:
    file.write(str(j[0]) + " " + str(j[1]) + "\n")
file.close()