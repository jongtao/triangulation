#!/bin/python

import numpy as np
import matplotlib.pyplot as plt
import subprocess


# Computes the X,Y coordinate pairs on the circumference of a given circle
def generateCircle(radius, offsetX, offsetY, nPoints):
	x = [];
	y = [];
	angles = np.linspace(0, 2*np.pi, nPoints)	

	for angle in angles:
		xp = radius * np.cos(angle) + offsetX
		yp = radius * np.sin(angle) + offsetY
		x.append(xp)
		y.append(yp)
	
	return x, y



class Circle(object):
	def __init__(self, radius, posX, posY, resolution):
		self.radius = radius
		self.resolution = resolution
		self.posX = posX
		self.posY = posY
		(self.x, self.y) = generateCircle(radius, posX, posY, resolution);



def getSol(circle1, circle2, circle3):
	stepSize = 0.05
	tolerance = 0.1

	p = subprocess.Popen(["./cisect",
		str(circle1.posX), str(circle1.posY), str(circle1.radius),
		str(circle2.posX), str(circle2.posY), str(circle2.radius),
		str(circle3.posX), str(circle3.posY), str(circle3.radius),
		str(stepSize), str(tolerance)],
		stdout=subprocess.PIPE, universal_newlines=True)

	output = p.communicate()

  # print stderr for debugging
	print(output[1]);

	# get stdout
	output = output[0]

	# remove trailing newline
	output = output[:-1]

	sol = output.split(' '); 
	x = sol[0];
	y = sol[1];
	return (x, y)
		


# main
#plt.axis([-5, 5, -5, 5])
plt.ion()


circleList = [];

test_set = 1;

if test_set == 0:
	circleList.append(Circle(
		radius = 1, posX = 0, posY = 1, resolution = 1000));
	circleList.append(Circle(
		radius = 1, posX = 0, posY = -1, resolution = 1000));
	circleList.append(Circle(
		radius = 2, posX = 2, posY = 0, resolution = 1000));
else:
	circleList.append(Circle(
		radius = 10, posX = 0, posY = 10, resolution = 1000));
	circleList.append(Circle(
		radius = 11, posX = 0, posY = -10, resolution = 1000));
	circleList.append(Circle(
		radius = 5, posX = 10, posY = 0, resolution = 1000));

for circle in circleList:
	plt.plot(circle.x, circle.y);

plt.show();

(solX, solY) = getSol(circleList[0], circleList[1], circleList[2]);
plt.plot(solX, solY, 'o');
plt.grid();
plt.axis('equal');

print("({}, {})".format(solX, solY))
#p=subprocess.Popen(["echo", "asdf"], stdout=subprocess.PIPE,
#universal_newlines=True);
#output = p.communicate()
#output = output[0]
#print(output)
#print("asdf\n");
#
#if output == "asdf\n":
#	print("yes")
#else:
#	print("no")




while True:
	plt.pause(1)
