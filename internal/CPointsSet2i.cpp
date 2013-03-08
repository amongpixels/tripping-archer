/*
 * CPointsSet2i.cpp
 *
 *  Created on: 19 Feb 2013
 *      Author: dragomir
 */

#include "CPointsSet2i.h"

namespace archer
{
  
//  CPointsSet2i::CPointsSet2i() {
//    // TODO Auto-generated constructor stub
//
//  }
  
  int CPointsSet2i::getWidth() {
    return abs(this->topRight[0]) - abs(this->bottomLeft[0]);
  }
  
  int CPointsSet2i::getHeight() {
    return abs(this->topRight[1]) - abs(this->bottomLeft[1])
        ;
  }
  
//  CHeightmap& CPointsSet2i::getHeightmap() {
//
//    // if not default sizes were specified then just use heightmap of size enough to fit all the points
//    if (w == -1) {
//      w = this->getWidth();
//    }
//
//    if (h == -1) {
//      h = this->bottomLeft[1];
//    }
//
//    helpers::printVector2i(this->bottomLeft);
//
//    printf("TAJM TO DO SOME HEJTMAPING %d %d\n", w, h);
//
//    CHeightmap heightmap (w, h);
//    heightmap.zero();
//
//    for (std::vector<vector2i>::iterator i = this->points.begin() ; i < this->points.end() ; i++) {
//      helpers::printVector2i((*i));
//      //heightmap.setValue((*i)[0], (*i)[1], 1.0f);
//    }
//
//    return heightmap;
//
//  }

//  CPointsSet2i::~CPointsSet2i() {
//    // TODO Auto-generated destructor stub
//  }
  
  void CPointsSet2i::saveAsPNG(char* path) {
    unsigned int w = this->getWidth();
    unsigned int h = this->getHeight();

    CImg <unsigned char> image (w, h, 1, 3, 0);
    unsigned char color [] = { 255, 255, 255 };

    for (std::vector<vector2i>::iterator i = this->points.begin() ; i < this->points.end() ; i++) {
      vector2i p = (*i) - this->bottomLeft;
      image.draw_point(p[0], p[1], color);
      //heightmap.setValue((*i)[0], (*i)[1], 1.0f);
    }

    image.save_png(path);
  }
  
  void CPointsSet2i::createMask(CHeightmap* h) {

    // Make sure that the points will fit on the heightmap (sanity check)
    assert(h->getWidth() >= this->topRight[0] && h->getWidth() >= this->topRight[1]);

    vector2i median = this->getMedianPoint();

    std::vector<float> distances;

    for (std::vector<vector2i>::iterator i = this->points.begin() ; i != this->points.end() ; i++) {
      distances.push_back(((*i) - median).length());
    }

    // Select the biggest length
    std::vector<float> copyOfDistances = distances;
    std::sort(copyOfDistances.begin(), copyOfDistances.end());
    float maxDistance = (*copyOfDistances.end());

    h->zero();

    for (int i = 0 ; i < this->points.size() ; i++) {
      h->setValue(this->points[i][0], this->points[i][1], 1.0f - (distances[i] / maxDistance));
    }

  }

} /* namespace archer */
