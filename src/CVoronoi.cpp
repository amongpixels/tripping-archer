/*
 * CVoronoi.cpp
 *
 *  Created on: 8 Feb 2013
 *      Author: dragomir
 */

#include "CVoronoi.h"

namespace archer
{
  bool compareDistances (voronoiDistance d1, voronoiDistance d2) {
    return d1.distance < d2. distance;
  }
  
  CVoronoi::CVoronoi() {
    this->pointsCount = 20;
    this->boundingPoints = NULL;
  }

  void CVoronoi::apply(CHeightmap* h) {

    printf("Calculating Voronoi diagram for %d points... ", this->pointsCount);

    srand(time(NULL));

    this->points.resize(this->pointsCount);

    if (this->boundingPoints) {
      for (int i = 0 ; i < this->pointsCount ; i++) {
        this->points[i].position = this->boundingPoints->getPoints()[rand() % this->boundingPoints->getCount()];
        //this->points[i].distances.clear();
      }
    }
    else {
      // generate some random points
      for (int i = 0 ; i < this->pointsCount ; i++) {
        this->points[i].position.set(rand() % h->getWidth(), rand() % h->getHeight());
        //this->points[i].distances.clear();//resize(this->pointsCount - 1);
      }
    }

    float values [h->getWidth()][h->getHeight()];
    int belonging [h->getWidth()][h->getHeight()];
    float maxValue = 0.0f;
//
//    CPointsSet2i clusters [this->pointsCount];
//
//    bool isPointDiscarded [this->pointsCount];
//
//    for (int i = 0 ; i < this->pointsCount ; i++) {
//      isPointDiscarded[i] = false;
//    }
//
////    if (this->boundingPoints) {
////      // Detect all boundary points so we dont include them in the resulting voronoi diagram to stay in the correct bounds
////      for (std::vector<vector2i>::iterator i = this->boundingPoints->getPoints().begin() ; i != this->boundingPoints->getPoints().end() ; i++) {
////        for (int x = (*i)[0] - 1 ; x < (*i)[0] + 2 ; x++) {
////          for (int y = (*i)[1] - 1 ; y < (*i)[1] + 2 ; y++) {
////            if (x != (*i)[0] || y != (*i)[1]) {
////              if (!this->boundingPoints->isPointInSet(vector2i(x, y))) {
////                discarded
////              }
////            }
////          }
////        }
////      }
////    }
//
//    for (int x = 0 ; x < h->getWidth() ; x++) {
//      for (int y = 0 ; y < h->getHeight() ; y++) {
//
//        vector2i point (x, y);
//
//        if (this->boundingPoints && !this->boundingPoints->isPointInSet(point)) {
//          continue;
//        }
//
//        // calculate distances to all the points
//        std::vector <voronoiDistance> distances;
//
//        for (int i = 0 ; i < this->pointsCount ; i++) {
//          voronoiDistance d;
//          d.pointIndex = i;
//
//          // oh god remember to convert vector to float coz otherwise distances are integers.....
//          vector2f v = this->points[i].position - point;
//
//          // use square length for speed up, results dont differ that much from normal length
//          d.distance = v.length_squared();
//
//          distances.push_back(d);
//        }
//
//        // sort the distances
//        std::sort(distances.begin(), distances.end(), compareDistances);
//
//        // cluster the point
//        clusters[distances[0].pointIndex].addPoint(point);
//
//      }
//    }
//
//    for (int i = 0 ; i < this->pointsCount ; i++) {
//      vector2i c = clusters[i].getCentroid();
//      //this->points[i].position.set(c[0], c[1]);
//      //clusters[i].createMask(h);
//    }



    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {

        values[x][y] = 0.0f;

        vector2i point (x, y);

        if (this->boundingPoints && !this->boundingPoints->isPointInSet(point)) {
          continue;
        }

        // calculate distances to all the points
        std::vector <voronoiDistance> distances;

        for (int i = 0 ; i < this->pointsCount ; i++) {
          voronoiDistance d;
          d.pointIndex = i;

          // oh god remember to convert vector to float coz otherwise distances are integers.....
          vector2f v = this->points[i].position - point;

          // use square length for speed up, results dont differ that much from normal length
          d.distance = v.length_squared();

          distances.push_back(d);
        }

        // sort the distances
        std::sort(distances.begin(), distances.end(), compareDistances);

        // calculate the elevation
        float value = (-1) * distances[0].distance + distances[1].distance;
        //float value = (-1) * distances[0].distance;

        values[x][y] = value;

        if (value > maxValue) {
          maxValue = value;
        }

      }
    }

    // Saved normalized results
    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {
        //if (!isPointDiscarded[belonging[x][y]]) {
          h->setValue(x, y, h->getValue(x, y) + (values[x][y] / maxValue));
        //}
      }
    }

    printf("done.\n");
  }

  void CVoronoi::setBoundingPoints(CPointsSet2i * b) {
    this->boundingPoints = b;
    assert(this->boundingPoints->getCount() > 0);
  }
  
  void CVoronoi::setPointsCount(int i) {
    this->pointsCount = i;
  }
  
  void CVoronoi::createClusters(std::vector<CPointsSet2i> * clusters) {

    if (this->boundingPoints && this->boundingPoints->getCount() > 0) {

      printf("Calculating Voronoi diagram for %d points... ", this->pointsCount);

      srand(time(NULL));

      if (this->points.empty()) {
        this->points.resize(this->pointsCount);

        for (int i = 0 ; i < this->pointsCount ; i++) {
          this->points[i].position = this->boundingPoints->getPoints()[rand() % this->boundingPoints->getCount()];
        }
      }

      clusters->resize(this->pointsCount);

      for(std::vector<vector2i>::const_iterator i = this->boundingPoints->getPoints().begin() ; i != this->boundingPoints->getPoints().end() ; i++) {

        // calculate distances to all the points
        std::vector <voronoiDistance> distances;

        for (int j = 0 ; j < this->pointsCount ; j++) {
          voronoiDistance d;
          d.pointIndex = j;

          // oh god remember to convert vector to float coz otherwise distances are integers.....
          vector2f v = this->points[j].position - (*i);

          // use square length for speed up
          d.distance = v.length_squared();

          distances.push_back(d);
        }

        // sort the distances
        std::sort(distances.begin(), distances.end(), compareDistances);

        // cluster the point
        (*clusters)[distances[0].pointIndex].addPoint(*i);

      }

      printf("done.\n");

    }

  }

  CVoronoi::~CVoronoi() {
    // TODO Auto-generated destructor stub
  }
}
