/*
 * CVoronoi.cpp
 *
 *  Created on: 8 Feb 2013
 *      Author: dragomir
 */

#include "CVoronoi.h"

bool compareDistances (voronoiDistance d1, voronoiDistance d2) {
  return d1.distance < d2. distance;
}

CVoronoi::CVoronoi() {
  this->pointsCount = 100;
  
}

void CVoronoi::apply(CHeightmap* h) {

  srand(time(NULL));

  this->points.resize(this->pointsCount);

  // generate some random points
  for (int i = 0 ; i < this->pointsCount ; i++) {
    this->points[i].position.set(rand() % h->getWidth(), rand() % h->getHeight());
    this->points[i].distances.clear();//resize(this->pointsCount - 1);
  }

//  // for each point calculate the distances to each other point
//  for (int i = 0 ; i < this->pointsCount ; i++) {
//    for (int j = 0 ; j < this->pointsCount ; j++) {
//
//      if (i != j) {
//
//        voronoiDistance d;
//        d.pointIndex = j;
//        d.distance = (this->points[i].position - this->points[j].position).length();
//
//        this->points[i].distances.push_back(d);
//
//      }
//    }
//
//    // now sort the distances
//    std::sort(this->points[i].distances.begin(), this->points[i].distances.end(), compareDistances);
//  }

  for (int x = 0 ; x < h->getWidth() ; x++) {
    for (int y = 0 ; y < h->getHeight() ; y++) {

      vector2i point (x, y);

      // calculate distances to all the points
      std::vector <voronoiDistance> distances;

      for (int i = 0 ; i < this->pointsCount ; i++) {
        voronoiDistance d;
        d.pointIndex = i;

        // oh god remember to convert vector to float coz otherwise distances are integers.....
        vector2f v = this->points[i].position - point;
        d.distance = v.length();

        distances.push_back(d);
      }

      // sort the distances
      std::sort(distances.begin(), distances.end(), compareDistances);

      // calculate the elevation
      float value = -1.0f * distances[0].distance + distances[1].distance;

      h->setValue(x, y, h->getValue(x, y) * value);

    }
  }




}

CVoronoi::~CVoronoi() {
  // TODO Auto-generated destructor stub
}

