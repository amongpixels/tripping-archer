/*
 * CPointsSet2i.cpp
 *
 *  Created on: 19 Feb 2013
 *      Author: dragomir
 */

#include "CPointsSet2i.h"

namespace archer
{
  
  CPointsSet2i::CPointsSet2i() {
    this->skeleton = NULL;
  }
  
  int CPointsSet2i::getWidth() {
    return abs(this->topRight[0]) - abs(this->bottomLeft[0]) + 1;
  }
  
  int CPointsSet2i::getHeight() {
    return abs(this->topRight[1]) - abs(this->bottomLeft[1]) + 1;
  }
  
  void CPointsSet2i::saveAsPNG(char* path, int width, int height) {

    assert(this->binaryMap.size() > 0);

    unsigned int w = (width < 0) ? this->getWidth() : width;
    unsigned int h = (height < 0) ? this->getHeight() : height;

    CImg <unsigned char> image (w, h, 1, 3, 0);
    unsigned char color [] = { 255, 255, 255 };

    for (std::vector<vector2i>::iterator i = this->points.begin() ; i < this->points.end() ; i++) {
      vector2i p = (*i);

      if (width < 0) {
        p[0] -= this->bottomLeft[0];
      }

      if (height < 0) {
        p[1] -= this->bottomLeft[1];
      }

      if (p[0] < (int)(w) && p[1] < (int)(h)) {
        image.draw_point(p[0], p[1], color);
      }
    }

    image.save_png(path);
  }
  
  void CPointsSet2i::addPoint(const vector2i & v) {

    if (this->isPointInSet(v)) {
      return;
    }

    assert(v[0] >= 0 && v[1] >= 0);
    TPointsSet<vector2i>::addPoint(v);

    if (this->binaryMap.size() == 0) {
      this->binaryMap.resize(v[0] + 1, std::vector<unsigned char> (v[1] + 1, 0));
    }
    else {
      if ((unsigned int)(v[1]) >= this->binaryMap[0].size()) {
        for (Array2b::iterator i = this->binaryMap.begin() ; i != this->binaryMap.end() ; i++) {
          (*i).resize(v[1] + 1, 0);
        }
      }

      if ((unsigned int)(v[0]) >= this->binaryMap.size()) {
        this->binaryMap.resize(v[0] + 1, std::vector<unsigned char>(this->binaryMap[0].size(), 0));
      }
    }

    this->binaryMap[v[0]][v[1]] = 1;

    // Invalidate the skeleton
    //this->skeletonIsCorrect = false;

  }
  
  void CPointsSet2i::generateSkeleton(CPointsSet2i * s) {

    if (!this->skeleton) {

      printf("Generating a skeleton...");

      this->skeleton = new CPointsSet2i();

      if (this->binaryMap.size() == 0) {
        return;
      }

      Array2b * map = new Array2b(this->binaryMap.size() + 2, std::vector<unsigned char>(this->binaryMap[0].size() + 2, 0));

      for (unsigned int x = 0 ; x < this->binaryMap.size() ; x++) {
        for (unsigned int y = 0 ; y < this->binaryMap[0].size() ; y++) {
          if (this->binaryMap[x][y]) {
            (*map)[x+1][y+1] = 1;
          }
        }
      }

      std::vector< std::pair<int, int> > marked;

      /*
       * In first iteration at least one in each of the first two sets of
       * neighbours (specified as indices) bust be a part of the object. In
       * second iteration second half of the array is tested.
       */
      int bgTests [4][3] = {
          { 1, 3, 5},
          { 3, 5, 7},
          { 1, 3, 7},
          { 1, 5, 7}
      };

      bool converged = false;

      while (!converged) {
        for (int i = 0 ; i < 2 ; i++) {

          marked.clear();

          for (unsigned int x = 1 ; x < (*map).size() - 1 ; x++) {
            for (unsigned int y = 1 ; y < (*map)[0].size() - 1 ; y++) {

              if ((*map)[x][y] == 1) {

                if (this->getConnectivity((*map), x, y) == 1) {
                  std::vector< std::pair<int, int> > indices;
                  this->createNeghbourIndices(&indices, x, y);

                  // Count the neighbours
                  int neighbourCount = 0;

                  for (int j = 1 ; j < 9 ; j++) {
                    if ((*map)[indices[j].first][indices[j].second] == 1) {
                      neighbourCount++;
                    }
                  }

                  if ((neighbourCount >= 2) && (neighbourCount <= 6)) {

                    if (this->passesBackgroundTest((*map), x, y, bgTests[i * 2])) {
                      if (this->passesBackgroundTest((*map), x, y, bgTests[i * 2 + 1])) {
                        marked.push_back(std::pair<int, int>(x, y));
                      }
                    }

                  }
                }
              }

            }
          }

          // Delete all marked
          if (marked.size() > 0) {
            for (std::vector< std::pair<int, int> >::iterator j = marked.begin() ; j != marked.end() ; j++) {
              (*map)[(*j).first][(*j).second] = 0;
            }
          }
          else {
            converged = true;
            break;
          }

        }

      }

      // Output the skeleton
      //this->skeleton.clear();

      for (unsigned int x = 1 ; x < (*map).size() - 1 ; x++) {
        for (unsigned int y = 1 ; y < (*map)[0].size() - 1 ; y++) {
          if ((*map)[x][y]) {
            vector2i p (x-1, y-1);
            this->skeleton->addPoint(p);
          }
        }
      }

    }

    (*s) = (*this->skeleton);

    //memcpy(s, this->skeleton, sizeof(CPointsSet2i));

  }

  void CPointsSet2i::clear() {
    TPointsSet<vector2i>::clear();
    this->binaryMap.clear();
  }

  void CPointsSet2i::createMask(CHeightmap* h, float blur) {

    // Make sure that the points will fit on the heightmap (sanity check)
    assert(h->getWidth() >= this->topRight[0] && h->getWidth() >= this->topRight[1]);

    CImg <unsigned char> mask (h->getWidth(), h->getHeight(), 1, 3, 0);
    unsigned char c [] = { 255, 255, 255 };

    // Just blur current points
    for (std::vector<vector2i>::iterator i = this->points.begin() ; i != this->points.end() ; i++) {
      mask.draw_point((*i)[0], (*i)[1], c);
    }

    if (blur) {
      mask.blur(blur);
    }
    h->zero();

    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {
        h->setValue(x, y, (float)(mask(x, y, 0, 0)) / 255.0f);
      }
    }

//    vector2i median = this->getMedianPoint();
//
//    std::vector<float> distances;
//
//    for (std::vector<vector2i>::iterator i = this->points.begin() ; i != this->points.end() ; i++) {
//      distances.push_back(((*i) - median).length());
//    }
//
//    // Select the biggest length
//    std::vector<float> copyOfDistances = distances;
//    std::sort(copyOfDistances.begin(), copyOfDistances.end());
//    float maxDistance = (*copyOfDistances.end());
//
//    h->zero();
//
//    for (int i = 0 ; i < this->points.size() ; i++) {
//      h->setValue(this->points[i][0], this->points[i][1], 1.0f - (distances[i] / maxDistance));
//    }

  }

  bool CPointsSet2i::isPointInSet(const vector2i & v) {
    if (this->binaryMap.size() > 0) {
      if (v[0] >= 0 && v[0] < this->binaryMap.size() && v[1] >= 0 && v[1] < this->binaryMap[0].size()) {
        if (this->binaryMap[v[0]][v[1]]) {
          return true;
        }
      }
    }

    return false;
  }

  int CPointsSet2i::getConnectivity(Array2b const & array, int x, int y) {

    std::vector< std::pair<int, int> > indices;
    this->createNeghbourIndices(&indices, x, y);
    indices.push_back(indices[1]); // Remember to loop back

    int changes = 0;

    for (int i = 1 ; i < 10 ; i++) {
      if (array[indices[i].first][indices[i].second] == 1) {
        if (array[indices[i-1].first][indices[i-1].second] == 0) {
          changes++;
        }
      }
    }

    return changes;
  }

  bool CPointsSet2i::passesBackgroundTest(const Array2b & array, int x, int y, int neighbours [3]) {
    std::vector< std::pair<int, int> > indices;
    this->createNeghbourIndices(&indices, x, y);

    for (int i = 0 ; i < 3 ; i++) {
      if (array[indices[neighbours[i]].first][indices[neighbours[i]].second] == 0) {
        return true;
      }
    }

    return false;
  }

  void CPointsSet2i::createNeghbourIndices(std::vector< std::pair<int, int> > * pairs, int x, int y) {
    pairs->push_back(std::pair<int, int>(x, y));
    pairs->push_back(std::pair<int, int>(x, y-1));
    pairs->push_back(std::pair<int, int>(x+1, y-1));
    pairs->push_back(std::pair<int, int>(x+1, y));
    pairs->push_back(std::pair<int, int>(x+1, y+1));
    pairs->push_back(std::pair<int, int>(x, y+1));
    pairs->push_back(std::pair<int, int>(x-1, y+1));
    pairs->push_back(std::pair<int, int>(x-1, y));
    pairs->push_back(std::pair<int, int>(x-1, y-1));
  }

  CPointsSet2i & CPointsSet2i::operator += (const CPointsSet2i & p) {
    for (std::vector<vector2i>::const_iterator i = p.getPoints().begin() ; i != p.getPoints().end() ; i++) {
      this->addPoint((*i));
    }

    return *this;
  }
  
  const CPointsSet2i CPointsSet2i::operator +(const CPointsSet2i & p) {
    CPointsSet2i points = *this;
    points += p;
    return points;
  }
  
  CPointsSet2i::~CPointsSet2i() {
    if (this->skeleton) {
      delete this->skeleton;
    }
  }

} /* namespace archer */
