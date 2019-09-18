
// author: Johannes Obenaus

// program that computes representative line set of a point set
// ATTENTION (see below) !!!! : This is not meant to be a robust implementation
// but only serves to compute the representative line set of certain point sets.
// In particular, it might fail, if the point set is to close to non-general position.

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// ATTENTION HERE !!!!
// this is a handcrafted value to shift the lines determined by two points in such a way
// that they "look" nice when drawn. This might cause trouble for point sets to close 
// to non-general position.
const double EPS = 1;

struct Point
{
	double x;
	double y;
	int id;
};

struct Segment
{
	Point p;
	Point q;
};

struct Line
{
	Point p;
	Point q;
	std::vector<int> above_list;
};


double compute_slope(Point p1, Point p2) 
{
	return (p2.y - p1.y) / (p2.x - p1.x);
}


bool is_equal(std::vector<int> a, std::vector<int> b) 
{
	if (a.size() != b.size()) return false;
	// maybe better do the sorting here

	for (int i = 0; i < a.size(); ++i) 
	{
		if (a[i] != b[i]) return false;
	}
	return true;
} 

bool contains(vector<int> &vec, int a) 
{
	for (int v : vec) 
	{
		if (v == a) return true;
	}
	return false;
}


std::vector<int> relativeComplement(vector<int> arr, int n) 
{ 
	vector<int> res;
	for (int i = 0; i < n; ++i) 
	{
		if (contains(arr, i)) continue;
		res.push_back(i);
	}
	return res;
} 


bool is_valid(std::vector<Line> chosen, std::vector<int> above, int n) 
{
	if (above.size() == 0 || above.size() == n) return false;

	for (Line l : chosen) {
		if (is_equal(l.above_list, above)) return false;

		vector<int> complement = relativeComplement(l.above_list, n);
		if (is_equal(complement, above)) return false;
	}

	return true;
}


bool is_above(Point p, Line l) 
{
	double s = (l.q.y - l.p.y) * p.x + (l.p.x - l.q.x) * p.y + (l.q.x * l.p.y - l.p.x * l.q.y);

	if (s > 0) return true;
	else return false;
}

vector<int> compute_points_above(vector<Point> pset, Line l) {
	std::vector<int> res;

	for (Point p : pset) {
		if (is_above(p, l)) res.push_back(p.id);
	}

	std::sort(res.begin(), res.end());
	return res;
}




vector<Line> compute_repr_lines(std::vector<Point> pset) 
{
	std::vector<Line> res;
	int n = pset.size();

	// for any 2 points consider 4 lines
	for (Point p1 : pset) {
		for (Point p2 : pset) {
			if (p1.id == p2.id) continue;
			
			double slope = compute_slope(p1, p2);
			Line l1,l2,l3,l4;
			
			if (std::abs(slope) < 1) {
				l1 = { {p1.x, p1.y + EPS}, {p2.x, p2.y + EPS}, vector<int>() };
				l2 = { {p1.x, p1.y - EPS}, {p2.x, p2.y - EPS}, vector<int>() };
				l3 = { {p1.x, p1.y + EPS}, {p2.x, p2.y - EPS}, vector<int>() };
				l4 = { {p1.x, p1.y - EPS}, {p2.x, p2.y + EPS}, vector<int>() };
			} else {
				l1 = { {p1.x + EPS, p1.y}, {p2.x + EPS, p2.y}, vector<int>() };
				l2 = { {p1.x - EPS, p1.y}, {p2.x - EPS, p2.y}, vector<int>() };
				l3 = { {p1.x + EPS, p1.y}, {p2.x - EPS, p2.y}, vector<int>() };
				l4 = { {p1.x - EPS, p1.y}, {p2.x + EPS, p2.y}, vector<int>() };
			}

			// for each line l compute set of points above l and put l into map
			l1.above_list = compute_points_above(pset, l1);
			l2.above_list = compute_points_above(pset, l2);
			l3.above_list = compute_points_above(pset, l3);
			l4.above_list = compute_points_above(pset, l4);

			if (is_valid(res, l1.above_list, n)) res.push_back(l1);
			if (is_valid(res, l2.above_list, n)) res.push_back(l2);
			if (is_valid(res, l3.above_list, n)) res.push_back(l3);
			if (is_valid(res, l4.above_list, n)) res.push_back(l4);	
		}
	}

	return res;
}

void save_results(vector<Line> repr_lines) 
{
	std::ofstream outfile("lines_res/repr_lines"+std::to_string(repr_lines.size())+".txt");

	for (Line line : repr_lines) {
		outfile << line.p.x << " " << line.p.y << " " << line.q.x << " " << line.q.y << " " << endl;
	}
	outfile.close();
}



std::vector<Point> get_pointset(std::string path) 
{
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



int main(int argc, char** argv) {

	if (argc != 2) {
		std::cout << "please provide path to point set" << endl;
		return 0;
	}

	std::string path_to_points = argv[1];
	vector<Point> pointset = get_pointset(path_to_points);

	vector<Line> repr_lines = compute_repr_lines(pointset);

	// save lines

	std::cout << "Size of representative line set = " << repr_lines.size() << endl;
	save_results(repr_lines);
	std::cout << "Results saved." << endl;

	return 0;
}





