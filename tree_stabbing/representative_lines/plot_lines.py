import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import sys
import os

def newline(p1, p2):
    ax = plt.gca()
    xmin, xmax = ax.get_xbound()

    if(p2[0] == p1[0]):
        xmin = xmax = p1[0]
        ymin, ymax = ax.get_ybound()
    else:
        ymax = p1[1]+(p2[1]-p1[1])/(p2[0]-p1[0])*(xmax-p1[0])
        ymin = p1[1]+(p2[1]-p1[1])/(p2[0]-p1[0])*(xmin-p1[0])

    l = mlines.Line2D([xmin,xmax], [ymin,ymax])
    ax.add_line(l)
    return l


# plots all points and all representative lines in a single figure
def plot(lines, pointset):
    for p in pointset:
        plt.scatter(p[0],p[1])

    for line in lines:
        newline([line[0],line[1]], [line[2],line[3]])

    plt.show()

# plots all representative lines in individual figures
def plot2(lines, pointset):

    for line in lines:
        for p in pointset:
            plt.scatter(p[0],p[1])

        newline([line[0],line[1]], [line[2],line[3]])

        plt.show()


def main():

    if (len(sys.argv) != 3):
        print("please provide path to point file and lines file.")
        return

    path_points = sys.argv[1]
    path_lines = sys.argv[2]
    
    points = np.loadtxt(path_points)
    lines = np.loadtxt(path_lines)

    # plot function plots everything in a single figure and 
    # plot2 creates many figures (one for each representative line)
    plot(lines, points)
    #plot2(lines, points)

    return



if __name__ == '__main__':
    main()







    