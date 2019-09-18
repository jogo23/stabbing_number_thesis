

This is the simulation to analyze the behaviour of the greedy crossing family for random point sets.
Points are drawn uniformly at random from a square. main.cpp runs the experiment, the parameter for the 
size of the sampled point sets can be set in the main function in main.cpp and is set to 20 at the moment.
Note that it takes a lot more time for larger values, therefore we recommend to leave it like this. Results
for simulation of point sets up to size 60 are included in the files avg_crossing_numbers.txt, crossing_numbers.txt 
and folders pointsets and crossing_families. 

The simulations can be executed as follows (compiling the .cpp files of course depends on your compiler).
Also, for plotting the results, there are python files provided, requiring Python 3.
All commands that are optional, because the required results are already provided are marked by [OPTIONAL]:


[OPTIONAL]
$ g++ -std=c++11 -o main main.cpp
$ ./main

[PLOT RESULTS]
$ python plot_avg.py 'example_results/avg_crossing_numbers.txt'
$ python plot_cr_family.py 'example_results/pointsets/pointset_6_0.txt' 'example_results/crossing_families/crossing_family_6_0.txt'

Just change the names of the point sets and crossing families in the last line if you want to plot other sets.

