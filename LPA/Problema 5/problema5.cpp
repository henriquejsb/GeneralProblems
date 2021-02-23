#include <iostream>
#include <cstdio>
#include <cfloat>
#include <algorithm>
#include <vector>
using namespace std;

class Line;

class Point{
public:
	double x,y;
	int type; 	//0 - line leftest point, 1 - line rightest point, 2 - intersection
	int line1_id;
	int line2_id;
	Point(){};
	Point(double x, double y): x(x), y(y) {};
	Point(double x, double y, int type, int line1_id): x(x), y(y), type(type), line1_id(line1_id) {};
	Point(double x, double y, int type, int line1_id, int line2_id): x(x), y(y), type(type), line1_id(line1_id), line2_id(line2_id) {};
};

class Line{
public:
	int id;
	// y = m*x + b
	double m;
	double b;
	bool intersects;
	bool vertical;
	bool first;
	Point leftP, rightP;
	Line(){};
	Line(int id, double x1, double y1, double x2, double y2) : id(id){
		if(x1 < x2){
			leftP = Point(x1,y1, 0, id);
			rightP = Point(x2,y2, 1, id);
			vertical = false;
			m = (rightP.y - leftP.y) / (rightP.x - leftP.x);
			b = leftP.y - m * leftP.x;
		}
		else if (x1 > x2){
			leftP = Point(x2,y2, 0, id);
			rightP = Point(x1,y1, 1, id);
			vertical = false;
			m = (rightP.y - leftP.y) / (rightP.x - leftP.x);
			b = leftP.y - m * leftP.x;
		}
		else {
			if(y1 > y2){
				leftP = Point(x1,y1, 0, id);
				rightP = Point(x2,y2, 1, id);
			}
			else{
				leftP = Point(x2,y2, 0, id);
				rightP = Point(x1,y1, 1, id);
			}
			vertical = true;
			first = true;
		}
		intersects = false;
	}

	double getYatX(double x){
		if(!vertical)
			return m * x + b;
		first = !first;
		if(!first){
			return leftP.y;
		}
		return rightP.y;

	}

	bool operator== (const Line &other) const {
		return id == other.id;
	}
};

Line lasers[1000];
double xmax, xmin, ymax, ymin;
int nlasers;

bool point_cmp(Point const & a, Point const & b) {
    if (a.x < b.x){
    	return true;
    }
    else if (a.x > b.x){
    	return false;
    }
    else{
	    return a.y > b.y;
    }
}

bool counterclockwise(Point p1, Point p2, Point p3){
	return ((p2.x*p3.y - p2.y*p3.x) - p1.x*(p3.y - p2.y) + p1.y*(p3.x - p2.x)) > 0;
}

bool check_intersection(Line const & l1, Line const & l2){
	Point a = l1.leftP, b = l1.rightP, c = l2.leftP, d = l2.rightP;
	return ((counterclockwise(a, c, d) != counterclockwise(b, c, d)) && counterclockwise(a, b, c) != counterclockwise(a, b, d));
}

Point calculate_intersection(Line const &l1, Line const &l2){
	double x, y;
	if (l1.vertical){
		x = l1.leftP.x;
		y = l2.m * x + l2.b;
	}
	else if (l2.vertical){
		x = l2.leftP.x;
		y = l1.m * x + l1.b;
	}
	else{
		x = (l2.b - l1.b) / (l1.m - l2.m);
		y = l1.m * x + l1.b;
		
	}
	return Point(x, y, 3, l1.id, l2.id);	
	
}

void print_lasers(){
	for(int i = 0; i < nlasers; i++){
		printf("[%lf ;  %lf]  ->  [%lf  ;  %lf]\n",lasers[i].leftP.x, lasers[i].leftP.y, lasers[i].rightP.x, lasers[i].rightP.y);
	}
}


void find_solution(){
	vector<Point> points;
	for (int i = 0; i < nlasers; i++){
		//printf("Recebi o laser [%lf ;  %lf]  ->  [%lf  ;  %lf]\n",lasers[i].leftP.x, lasers[i].leftP.y, lasers[i].rightP.x, lasers[i].rightP.y);
		for (int j = i+1; j < nlasers; j++){
			if(check_intersection(lasers[i], lasers[j])){
				points.push_back(calculate_intersection(lasers[i], lasers[j]));
				lasers[i].intersects = true;
				lasers[j].intersects = true;
			}
		}
		//if (lasers[i].intersects) printf("tinha intersecoes\n");
		// se uma recta nao intersectar com nenhuma outra nem com ymax ou ymin, podemos ignora-a
		if (lasers[i].intersects || lasers[i].leftP.y == ymax || lasers[i].leftP.y == ymin  || lasers[i].rightP.y == ymax || lasers[i].rightP.y == ymin){
			points.push_back(lasers[i].leftP);
			points.push_back(lasers[i].rightP);	
		}
		
	}
	sort(points.begin(), points.end(), point_cmp);
	
	/*
	printf("_______________________________________\n");
	for (unsigned int i=0; i<points.size(); i++){
		int t = points[i].type;
		if(t == 0){
			printf("%lf; %lf - starting point of line %d\n", points[i].x, points[i].y, points[i].line1_id);
		}
		else if (t == 1){
			printf("%lf; %lf - ending point of line %d\n", points[i].x, points[i].y, points[i].line1_id);
		}
		else{
			printf("%lf; %lf - intersection between line %d and line %d\n", points[i].x, points[i].y, points[i].line1_id, points[i].line2_id);
		}
	}
	*/
	
	vector<int> sections;
	sections.push_back(0);
	vector<Line> active_lines;
	for (unsigned int i=0; i<points.size(); i++){
		Point current = points[i];
		//printf("Recebi o ponto %lf - %lf\n", current.x, current.y);
		if(current.type == 0){
			unsigned int j = 0;
			for(; j<active_lines.size(); j++){
				if(active_lines[j].getYatX(current.x) < current.y) break;
			}
			active_lines.insert(active_lines.begin()+j, lasers[current.line1_id]);

			if(current.y == ymax)
				sections.insert(sections.begin() + j, sections[j] + 1);
			else if (current.y == ymin)
				sections.insert(sections.begin() + j + 1, sections[j] + 1);
			else 
				sections.insert(sections.begin() + j, sections[j]);
		}
		else if(current.type == 1){
			ptrdiff_t pos = find(active_lines.begin(), active_lines.end(), lasers[current.line1_id]) - active_lines.begin();
			active_lines.erase(active_lines.begin()+pos);

			if(current.y == ymax){
				sections[pos] = min(sections[pos] + 1, sections[pos + 1]);
				sections.erase(sections.begin() + pos + 1);	
			}
			else if (current.y == ymin){
				sections[pos] = min(sections[pos], sections[pos + 1] + 1);
				sections.erase(sections.begin() + pos + 1);		
			}
			else{
				sections[pos] = min(sections[pos], sections[pos + 1]);
				sections.erase(sections.begin() + pos + 1);	
			}
			
		}
		else{
			unsigned int j = 0;
			int index1, index2;
			for(; j<active_lines.size(); j++){
				if (active_lines[j].id == current.line1_id){
					index1 = j;
				}
				else if (active_lines[j].id == current.line2_id){
					index2 = j;
				}
			}
			iter_swap(active_lines.begin()+index1, active_lines.begin()+index2);

			int auxmax, auxmin;
			if(index1 > index2){
				auxmax = index1;
				auxmin = index2;
			}
			else{
				auxmax = index2;
				auxmin = index1;
			}
			sections[auxmin + 1] = min(sections[auxmin], sections[auxmax+1]) + 1;
		}
		/*
		printf("e nao crashei\n");
		printf("\n_____\nLines:\n");
		for (unsigned int j=0; j<active_lines.size(); j++){
			printf("Line %d\n", active_lines[j].id);
		}
		printf("\n......\nSections:\n");
		for (unsigned int j=0; j<sections.size(); j++){
			printf("Section %d\n", sections[j]);
		}
		*/
	}
	printf("%d\n", sections[0]);
}



int main(){
	int ncases;
	double x1,x2,y1,y2;
	scanf("%d",&ncases);
	for(int k = 0; k < ncases; k++){
		scanf("%lf %lf %lf %lf\n",&xmin,&ymin,&xmax,&ymax);
		scanf("%d",&nlasers);
		for(int i = 0; i < nlasers; i++){
			scanf("%lf %lf %lf %lf",&x1,&y1,&x2,&y2);
			lasers[i] = Line(i, x1, y1, x2, y2);
		}
		//print_lasers();
		find_solution();
	}

	return 0;
}