
// author: Johannes Obenaus

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <cassert>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include <unordered_set>
#include <chrono>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;


const double EPSILON = 0.0000000001;

const double GRIDSIZE = 10.0; // size of the region we draw points from (side length of the square)

std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<> dist(0, GRIDSIZE);

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
	int num_proper_crossings;
	std::unordered_set<int> crossing_seg_list; // contains list of the ids of the segments crossed by this one
	int id; 
};


bool collinear(Point p1, Point p2, Point p3) 
{
	if (std::abs(p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) < EPSILON) return true;
	else return false;
}


// naive implementation of general position test (TODO, improve running time)
bool general_position(std::vector<Point> pset) 
{
	for (int i = 0; i < pset.size() - 2; i++) 
	{
		for (int j = i + 1; j < pset.size() - 1; j++) 
		{
			for (int k = j + 1; k < pset.size(); k++) 
			{
				if (collinear(pset[i], pset[j], pset[k])) return false;
			}
		}
	}
	return true;
}


void save_points_for_printing(std::vector<Point> pointset, int i) 
{
	std::ofstream outfile ("results_quadratic_crossings/test_points"+std::to_string(i)+".txt");
	for (auto p : pointset) 
	{
		outfile << p.x << " " << p.y << endl;
	}
	outfile.close();
}


void save_segments_for_printing(std::vector<Segment> seg_set, int i) 
{
	std::ofstream outfile ("results_quadratic_crossings/test_segments"+std::to_string(i)+".txt");
	for (auto s : seg_set) 
	{
		outfile << s.p.x << " " << s.p.y << endl;
		outfile << s.q.x << " " << s.q.y << endl;
	}
	outfile.close();
}


std::vector<Segment> create_all_segments(std::vector<Point> points) 
{
	std::vector<Segment> res;
	int counter = 0;
	for (int i = 0; i < points.size() - 1; i++) 
	{
		for (int j = i+1; j < points.size(); j++) 
		{
			res.push_back((Segment){ points[i], points[j], 0, {}, counter });
			counter++;
		}
	}

	return res;
}


std::vector<Point> create_rand_pointset_square(int n) 
{
	std::vector<Point> pointset;
	for (int i = 0; i < n; ++i) 
	{
		Point tmp = {dist(e2), dist(e2), i};
		pointset.push_back(tmp);
	}
	return pointset;
}


//std::vector<Point> create_rand_pointset_disk(int n) {
	// TODO
//}

// taken from https://stackoverflow.com/questions/35473936/find-whether-two-line-segments-intersect-or-not-in-c
bool is_properly_crossing(Segment s1, Segment s2) 
{
	if (s1.p.id == s2.p.id && s1.q.id == s2.q.id) return false;
	if (s1.p.id == s2.p.id || s1.p.id == s2.q.id || s1.q.id == s2.p.id || s1.q.id == s2.q.id) return false;

	float ax = s1.q.x - s1.p.x;     // direction of line a
    float ay = s1.q.y - s1.p.y;     // ax and ay as above

    float bx = s2.p.x - s2.q.x;     // direction of line b, reversed
    float by = s2.p.y - s2.q.y;     // really -by and -by as above

    float dx = s2.p.x - s1.p.x;   // right-hand side
    float dy = s2.p.y - s1.p.y;

    float det = ax * by - ay * bx;

    if (std::abs(det) < EPSILON) return false;

    float r = (dx * by - dy * bx) / det;
    float s = (ax * dy - ay * dx) / det;

    if (r > 0 && r < 1 && s > 0 && s < 1) return true;

	return false;
}


void update_all_segments(std::vector<Segment> &all_segments) 
{
	for (auto &s : all_segments) 
	{
		assert(s.num_proper_crossings == 0);
		for (auto t : all_segments) 
		{
			if (is_properly_crossing(s,t)) 
			{
				s.num_proper_crossings = s.num_proper_crossings + 1;
				s.crossing_seg_list.insert(t.id);
			}
		}
	}
}


int find_max_crossing_seg(std::vector<Segment> all_segments, std::vector<int> eligible) 
{
	assert(!eligible.empty());
	int maxi = -1;
	int maxi_id = -1;
	for (int a : eligible) 
	{
		if (all_segments[a].num_proper_crossings > maxi) 
		{
			maxi = all_segments[a].num_proper_crossings;
			maxi_id = a;
		}
	}
	return maxi_id;
}


std::vector<Segment> crossing_family(std::vector<Segment> all_segments) 
{
	std::vector<Segment> res;

	int n = all_segments.size();
	std::vector<int> eligible(n);
	for (int i = 0; i < n; ++i) eligible[i] = i;

	while(!eligible.empty()) 
	{
		int max_seg_id = find_max_crossing_seg(all_segments, eligible);
		res.push_back(all_segments[max_seg_id]);

		std::vector<int> new_eligible;
		for (auto i : eligible) 
		{
			if (all_segments[max_seg_id].crossing_seg_list.find(i) != all_segments[max_seg_id].crossing_seg_list.end()) new_eligible.push_back(i);
		}
		eligible = new_eligible;
	}

	return res;
}


struct Res_Object 
{
	int num_points;
	std::vector<int> all_cr_numbers;
	std::vector<std::vector<Point>> all_pointsets;  // list with all pointsets
	std::vector<std::vector<Segment>> all_cr_families;  // list with all crossing families
};


void run_exp(int num_total, int avg_size, std::vector<Res_Object> &results) 
{
	for (int n = 4; n < num_total; n += 2) 
	{
		auto start = std::chrono::high_resolution_clock::now();
		std::cout << n << " out of " << num_total << endl;
		
		std::vector<int> all_cr_numbers;
		std::vector<std::vector<Point>> all_psets;
		std::vector<std::vector<Segment>> all_cr_families;

		for (int i = 0; i < avg_size; i++) 
		{
			std::vector<Point> pointset = create_rand_pointset_square(n);
			
			while(true)
			{
				if (general_position(pointset)) break;
				pointset = create_rand_pointset_square(n);
			}

			std::vector<Segment> all_segments = create_all_segments(pointset);
			update_all_segments(all_segments);
			std::vector<Segment> c_family = crossing_family(all_segments);
			int cr_number = c_family.size();
			
			all_cr_numbers.push_back(cr_number);
			all_psets.push_back(pointset);
			all_cr_families.push_back(c_family);
		}

		results.push_back((Res_Object){n, all_cr_numbers, all_psets, all_cr_families});

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
		cout << "Time taken: " << duration.count() << " seconds" << endl;
	}
}



void save_results(std::vector<Res_Object> results) 
{
	std::string dir = "results_cf/";
	mkdir(dir.c_str(), 0777);
	
	std::ofstream outfile_cr_numbers (dir+"crossing_numbers.txt");
	std::ofstream outfile_cr_nums_avg (dir+"avg_crossing_numbers.txt");

	for (auto res : results) 
	{
		double avg_cr_num = 0.0;

		outfile_cr_numbers << res.num_points << endl;
		for (int c : res.all_cr_numbers) 
		{
			outfile_cr_numbers << c << " ";
			avg_cr_num += c;
		}
		outfile_cr_numbers << endl;
		avg_cr_num = avg_cr_num / res.all_cr_numbers.size();

		outfile_cr_nums_avg << res.num_points << " " << avg_cr_num << endl;

		std::string dir1 = dir+"pointsets";
		std::string dir2 = dir+"crossing_families";
		mkdir(dir1.c_str(), 0777);
		mkdir(dir2.c_str(), 0777);

		for (int i = 0; i < res.all_pointsets.size(); ++i) 
		{
			std::ofstream outfile_points (dir+"pointsets/pointset_"+std::to_string(res.num_points)+"_"+std::to_string(i)+".txt");
			std::ofstream outfile_segs (dir+"crossing_families/crossing_family_"+std::to_string(res.num_points)+"_"+std::to_string(i)+".txt");
			
			for (auto p : res.all_pointsets[i]) outfile_points << p.x << " " << p.y << endl;
			for (auto s : res.all_cr_families[i]) 
			{
				outfile_segs << s.p.x << " " << s.p.y << endl;
				outfile_segs << s.q.x << " " << s.q.y << endl;
			}

			outfile_points.close();
			outfile_segs.close();
		}
	}

	outfile_cr_numbers.close();
	outfile_cr_nums_avg.close();
}



int main() {
	
	auto start = std::chrono::high_resolution_clock::now();

	int n = 20;
	int avg_size = 15;

	std::vector<Res_Object> results;
	run_exp(n, avg_size, results);
	save_results(results);

	//for (auto r : results) {
	//	std::cout << "number of points is " << r.num_points << endl;
	//	std::cout << "all crossing numbers are ";
	//	for (auto cr : r.all_cr_numbers) std::cout << cr << " ";
	//	std::cout << endl;
	//}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::minutes>(stop - start);
	cout << "Total time: " << duration.count() << " minutes" << endl;
	
	return 0;
}























