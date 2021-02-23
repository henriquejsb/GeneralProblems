#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

class Point{
public:
	double x, y, area;
	Point(){}
	Point(double x, double y){
		this -> x = x;
		this -> y = y;
		this -> area = x * y;
	}
};

vector<Point> points;
int ntotalpoints, nusable;
double curr_x;
double max_area = 0.0;
double bound[129];
double area_total = 0;


bool comparePointsX(Point p1, Point p2){
	return p1.x < p2.x;
}


void setBound(){
	bound[ntotalpoints-1] = 0;
	for(int i = ntotalpoints -1; i >= 0 ; i--){
		bound[i] = bound[i+1] + (points[i+1].x - points[i].x) * points[i+1].y;
	}
}	


void solution(int point, double aux_area, double aux_x, int aux_usable){
	if(point == ntotalpoints) return;

	double a = (points[point].x - aux_x) * points[point].y;
	if(a + aux_area + bound[point] < max_area){
		return;
	}

	// Se ja nao e possivel utilizar os k pontos
	if(ntotalpoints - point + 1 <= aux_usable) {
		return;	
	} 

	if(aux_usable == 1) {
		if(aux_area + a > max_area) {
			 max_area = aux_area + a;
		}
	}

	else {
		solution(point + 1, aux_area + a, points[point].x, aux_usable - 1);
	}
	
	solution(point + 1, aux_area, aux_x, aux_usable);
}



int main(){
	double aux1, aux2;
	double aux_x = 0;
	scanf("%d %d", &ntotalpoints, &nusable);
	
	for(int i = 0; i < ntotalpoints; i++) {
		area_total += (aux1 - aux_x) * aux2;
		aux_x = aux1;
		scanf("%lf %lf",&aux1, &aux2);
		Point aux(aux1, aux2);
		points.push_back(aux);
	}
	sort(points.begin(), points.end(), comparePointsX);
	
	setBound();
	
	solution(0, 0.0, 0.0, nusable);
	
	printf("%.12lf\n", max_area);
		
	return 0;
}