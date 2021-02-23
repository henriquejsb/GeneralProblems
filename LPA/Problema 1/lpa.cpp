#include <iostream>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;

class Point {
  private:
    unsigned int x, y;

  public:
    Point() {}

    Point(unsigned int x1, unsigned int y1) {
      x = x1;
      y = y1;
    }

    unsigned int getX() {
      return x;
    }

    unsigned int getY() {
      return y;
    }

    void set(unsigned int x1, unsigned int y1) {
      x = x1;
      y = y1;
    }
};

class Rectangle {
  private:
    Point cie;
    Point csd;

  public:
    Rectangle() {}

    Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
      cie = Point(x1,y1);
      csd = Point(x2,y2);
    }

    Point getCIE() const {
      return cie;
    }

    Point getCSD() const {
      return csd;
    }
};

// Checks if rectangles are the same and on the same position (by comparing their corners)
bool compareRect(const Rectangle& b, const Rectangle& a) {
  if(b.getCIE().getX() == a.getCIE().getX() && b.getCIE().getY() == a.getCIE().getY() && b.getCSD().getX() == a.getCSD().getX() && b.getCSD().getY() == a.getCSD().getY()){
    return true;
  }
  return false;
}


unsigned int rectangles[8][3];
bool visited[8] = {false, false, false, false, false, false, false, false};
unsigned int nrectangles;
unsigned int totalarea = 0;
unsigned int nsolutions = 0;
std::vector<Rectangle> rectcoords;
std::vector<std::vector<Rectangle> > usedcombinations;

// Needed for std::sort comparisons
bool CompareData(const Rectangle& a, const Rectangle& b) {
  if (a.getCSD().getX() < b.getCSD().getX()) return true;
  if (b.getCSD().getX() < a.getCSD().getX()) return false;

  if (a.getCSD().getY() < b.getCSD().getY()) return true;
  if (b.getCSD().getY() < a.getCSD().getY()) return false;

  return false;
}


// Copies "original" array to a new one, replacing the element in "position" for the newVertices
unsigned int concatenate(Point* original, unsigned int size, Point* newV, unsigned int position, unsigned int nvertices, Point* newVertices){
  unsigned int i, j, newSize = 0;

  for(i = 0; i < size; i++) {
    if(i != position) {
      newV[newSize++] = original[i];
    }

    else {
      for(j = 0; j < nvertices; j++) {
        newV[newSize++] = newVertices[j];
      }
    }
  }

  return newSize;
}


void recursive(unsigned int* coord, unsigned int orientacao, Point* vertices, unsigned int size, unsigned int iteration, unsigned int i) {
  unsigned int j, k, l, x, y, nvertices, newSize;
  bool flag;
  Point newArray[9];
  Point newVertices[2];
  nvertices = 0;

  if(orientacao == 0) {
    x = vertices[i].getX() + coord[0];
    y = vertices[i].getY() + coord[1];
  }

  else {
    x = vertices[i].getX() + coord[1];
    y = vertices[i].getY() + coord[0];
  }

  Rectangle newrect = Rectangle(vertices[i].getX(), vertices[i].getY(), x, y);
  rectcoords.push_back(newrect);

  // Check number of vertices. If we have over two more vertices than rectangles to be placed, we can't build a rectangle
  if(size > nrectangles - iteration + 2) {
    return;
  }

  // Not the first or last
  if(i != 0 && i != size - 1) {

    // Check if new rectangle is 'loose' (leaves empty spaces to its left or bottom)
    if(y > vertices[i-1].getY()) {
      return;
    }
    if(x > vertices[i+1].getX()) {
      return;
    }

    // Create new vertex at the top
    if(y < vertices[i-1].getY()) {
      newVertices[nvertices++].set(vertices[i].getX(), y);
    }

    // Create new vertex in the right
    if(x < vertices[i+1].getX()) {
      newVertices[nvertices++].set(x, vertices[i].getY());
    }
  }

  // First recursion
  else if(vertices[i].getX() == 0 && vertices[i].getY() == 0) {
    // Creates two new vertices
    newVertices[nvertices++].set(vertices[i].getX(), y);
    newVertices[nvertices++].set(x, vertices[i].getY());
  }

  // Top vertex (left, first)
  else if(i == 0){
    if(x > vertices[i+1].getX()) {
      return;
    }

    // Create vertex at the top
    newVertices[nvertices++].set(vertices[i].getX(), y);

    if(x < vertices[i+1].getX()){
      // Create vertex at the right
      newVertices[nvertices++].set(x, vertices[i].getY());
    }
  }

  // Right vertex (ultimo da escadinha)
  else if(i == size - 1) {
    if(y > vertices[i-1].getY()) {
      return;
    }

    if(y < vertices[i-1].getY()) {
      // Create vertex at the top
      newVertices[nvertices++].set(vertices[i].getX(), y);
    }

    // Create vertex at the right
    newVertices[nvertices++].set(x, vertices[i].getY());
  }

  // Join new vertices
  newSize = concatenate(vertices, size, newArray, i, nvertices, newVertices);


  // If multiplying the sides goes over the total area, we're sure it's an invalid iteration
  if (newArray[0].getY() * newArray[newSize-1].getX() > totalarea){
    return;
  }

  // End
  if (iteration == nrectangles - 1){

    // It isn't a valid rectangle
    if(newSize != 2){
      return;
    }

    // copy rectcoord to aux
    std::vector<Rectangle> aux = rectcoords;

    // sort aux
    sort(aux.begin(), aux.end(), CompareData);

    // Check if rectangle is unique
    flag = true;
    for(k = 0; k < usedcombinations.size(); k++) {
      flag = false;

      for(l = 0; l < usedcombinations[k].size(); l++) {
        if(!compareRect(aux[l], usedcombinations[k][l])) {
          flag = true;
          break;
        }
      }

      if(!flag){
        break;
      }
    }

    if(!flag){
      return;
    }

    // We have a new unique rectangle
    usedcombinations.push_back(aux);
    nsolutions++;
    return;
  }

  flag = false;

  // Call the next iteration, for all the rectangles that weren't visited yet, rotating them if they're not squares
  for(j = 0; j < nrectangles; j++) {
    if(!visited[j]){
      for(i = 0; i < newSize; i++) {
        flag = true;
        visited[j] = true;
        recursive(rectangles[j], 0, newArray, newSize, iteration+1, i);
        rectcoords.pop_back();

        if(rectangles[j][0] != rectangles[j][1]){
          recursive(rectangles[j], 1, newArray, newSize, iteration+1, i);
          rectcoords.pop_back();
        }
        visited[j] = false;
      }
    }
  }
}


int main(){
  unsigned int i, j;
  Point vertices[9];
  vertices[0].set(0, 0);

  scanf("%u", &nrectangles);

  for(i = 0; i < nrectangles; i++) {
    scanf("%u %u", &rectangles[i][0], &rectangles[i][1]);
    rectangles[i][2] = 0;

    for (j = 0; j < i; j++) {
	  // Checks if rectangles are equal and flags them if they are so. For example 20, 40 is the same as 40,20.
      if ((rectangles[i][0] == rectangles[j][0] && rectangles[i][1] == rectangles[j][1]) || (rectangles[i][1] == rectangles[j][0] && rectangles[i][0] == rectangles[j][1])) {
        rectangles[i][2] = 1;
        break;
      }
    }
    totalarea += rectangles[i][0] * rectangles[i][1];
  }

  for(i = 0; i < nrectangles; i++) {
    if (rectangles[i][2] == 0) {
      visited[i] = true;
      recursive(rectangles[i], 0, vertices, 1, 0, 0);
      rectcoords.pop_back();

      // Rotate rectangle if it isn't a square
      if(rectangles[i][0] != rectangles[i][1]) {
        recursive(rectangles[i], 1, vertices, 1, 0, 0);
        rectcoords.pop_back();
      }
      visited[i] = false;
    }
  }
  cout << nsolutions << "\n";
}
