
// author: Johannes Obenaus

// this program computes stabbing numbers of spanning trees
// it requires three inputs:
//    - a point set
//    - a set of representative lines
//    - spanning trees

// it returns the stabbing numbers of all spanning trees.

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

struct Point {
	double x;
	double y;
	int id;
};

struct Line {
	Point p;
	Point q;
	int num_crossings;
};

struct Segment {
	Point p;
	Point q;
};

// taken from: https://stackoverflow.com/questions/35473936/find-whether-two-line-segments-intersect-or-not-in-c
bool intersection(Point p1, Point q1, Point p2, Point q2) 
// first two points correspond to line and last two to segment
{
    double ax = q1.x - p1.x;     // direction of line a
    double ay = q1.y - p1.y;     // ax and ay as above

    double bx = p2.x - q2.x;     // direction of line b, reversed
    double by = p2.y - q2.y;     // really -by and -by as above

    double dx = p2.x - p1.x;   // right-hand side
    double dy = p2.y - p1.y;
    
    double det = ax * by - ay * bx;

    if (std::abs(det) < 0.00000000001) return false;

    double r = (dx * by - dy * bx) / det;
    double s = (ax * dy - ay * dx) / det;

    if (s > 0 && s < 1) return true; // this has to be only s between 0 and 1 and not r because first object is a line
    else return false;
}


std::vector<Point> get_pointset(std::string path) {
	std::vector<Point> points;
	std::ifstream infile(path);

	double a, b;
	int counter = 0;
	while(infile >> a >> b) { 
		points.push_back( {a,b,counter} ); 
		counter++; 
	}
	infile.close();

	return points;
}


std::vector<Line> get_lines(std::string path) {
	std::vector<Line> lines;
	std::ifstream infile(path);

	double a,b,c,d;
	while (infile >> a >> b >> c >> d) { lines.push_back( {{a,b}, {c,d}, 0} ); }
	infile.close();

	return lines;
}


std::vector< std::vector<Segment> > get_segments(std::string path, std::vector<Point> &points) {
	int n = points.size();

	std::ifstream infile(path);
	
	int num_graphs;
	infile >> num_graphs;
	std::vector< std::vector<Segment> > graphs(num_graphs);

	for (int i = 0; i < num_graphs; ++i)
	{
		std::vector<Segment> segs(n-1);
		for (int j = 0; j < n-1; ++j) {
			int s1, s2;
			infile >> s1 >> s2;
			segs[j] = {points[s1], points[s2]};
		}
		graphs[i] = segs;
	}

	infile.close();

	return graphs;
}






int main(int argc, char** argv) {

	if (argc != 4) {
		std::cout << "please provide three paths: to_pointset, to_lines, to_trees" << endl;
		return 0;
	}

	std::cout << "might take 2 minutes... " << endl;

	std::string path_to_points = argv[1];
	std::string path_to_lines = argv[2];
	std::string path_to_trees = argv[3];

	// read data
	std::vector<Point> pointset = get_pointset(path_to_points);
	std::vector<Line> lines = get_lines(path_to_lines);
	std::vector< std::vector<Segment> > graphs = get_segments(path_to_trees, pointset);


	std::vector<long> res(pointset.size(), 0);

	long counter = 1;
	for (auto graph : graphs) {
		for (auto &line : lines) { line.num_crossings = 0; }

		for (int l = 0; l < lines.size(); ++l) {
			for (int s = 0; s < graph.size(); ++s) {
				if (intersection(lines[l].p, lines[l].q, graph[s].p, graph[s].q) == true) {
					lines[l].num_crossings++;
				}
			}
			//std::cout << "num_intersections of line " << l << " are " << lines[l].num_crossings << endl;
		}



		int max_crossings = 0;
		for (int l = 0; l < lines.size(); ++l) {
			if (lines[l].num_crossings > max_crossings) max_crossings = lines[l].num_crossings;
		}

		res[max_crossings] = res[max_crossings] + 1;
		if (max_crossings == 3) {
			std::cout << "The " << counter << "st graph (with following edges) has stabbing number 3:" << endl;
			for (auto seg : graph) { std::cout << seg.p.id << " " << seg.q.id << " "; }
			std::cout << endl;
		}
		counter++;

	}

	for (int i = 0; i < res.size(); ++i) {
		std::cout << res[i] << " graphs have stabbing number " << i << "." << endl;
	}
	
	return 0;
}










