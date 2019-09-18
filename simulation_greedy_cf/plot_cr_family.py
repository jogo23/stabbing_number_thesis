import numpy as np
import matplotlib.pyplot as plt
import sys


if (len(sys.argv) < 2): 
	print("please provide a path to file (first argument should be path to a point set and second (optionally) to a file with segments")
	print("For example: python plot_cr_family.py 'example_results/pointsets/pointset_6_0.txt' 'example_results/crossing_families/crossing_family_6_0.txt'")

else:
	data = np.loadtxt(sys.argv[1])
	plt.scatter(data[:,0], data[:,1])

	if (len(sys.argv) > 2):
		segments = np.loadtxt(sys.argv[2])
		for i in range(0,int(len(segments)),2):
			plt.plot(segments[i:i+2, 0], segments[i:i+2, 1])

	plt.show()



