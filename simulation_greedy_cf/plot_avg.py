import numpy as np
import matplotlib.pyplot as plt
import sys

# plots the average size of the greedy crossing families

if (len(sys.argv) < 2): 
	print("please provide a path to the file containing results.")
	print("For example: python plot_avg.py 'example_results/avg_crossing_numbers.txt'")

else:
	data = np.loadtxt(sys.argv[1])
	plt.plot(data[:,0], data[:,1])
	plt.xlabel('number of points')
	plt.ylabel('average size of greedy crossing family')

	plt.show()



