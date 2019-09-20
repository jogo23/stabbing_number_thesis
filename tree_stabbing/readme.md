

This is the simulation to show that the tree stabbing number is not monotone (Theorem 3.3 in Section 3.2).

The two point sets consisting of 9 and 10 points can be found in representative_lines/data/

In order to compute the stabbing numbers, we need a set of representative lines. These are computed by 
compute_representative.cpp (optional, since the results are provided already).

Next, we need the spanning trees (all, in the case of the 9 point set and one for the 10 point set).
These spanning trees can be found in the files all_spanning_trees_9.txt and one_spanning_tree_10.txt.

The spanning trees of the 9 point set can be computed using the all_spanning.c file (this takes about 10 minutes,
but is also optional since the results are provided already).

The main file to compute the stabbing numbers is compute_stabbing_num.cpp.


The simulations can be executed as follows (compiling the .cpp and .c files of course depends on your compiler).
Also, if you want plot the results, there are python files provided, requiring Python 3.

All commands that are optional, because the required results are already provided are marked by [OPTIONAL]:


[NON-OPTIONAL], [takes 10 minutes], [execute in tree_stabbing directory]
```
gcc -o all_spanning all_spanning.c

./all_spanning
```

[OPTIONAL], [execute in representative_lines directory],
[lines can also be plotted individually, see plot_lines.py file]
```
cd representative_lines/

g++ -std=c++11 -o compute_representative compute_representative.cpp

./compute_representative "data/symmetric9_final.txt"

./compute_representative "data/symmetric10_final.txt"

python plot_lines.py data/symmetric9_final.txt lines_res/repr_lines36.txt

python plot_lines.py data/symmetric10_final.txt lines_res/repr_lines45.txt

cd ..
```

[MAIN PART], [execute in tree_stabbing directory], [takes a few minutes]
```
g++ -std=c++11 -o compute_stabbing_num compute_stabbing_num.cpp

./compute_stabbing_num "representative_lines/data/symmetric9_final.txt" "representative_lines/lines_res/repr_lines36.txt" "all_spanning_trees_9.txt"

./compute_stabbing_num "representative_lines/data/symmetric10_final.txt" "representative_lines/lines_res/repr_lines45.txt" "one_spanning_tree_10.txt"
```

