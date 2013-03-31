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

    printf("Generating a skeleton...");

    if (!this->skeleton) {

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

    if (s) {
      (*s) = (*this->skeleton);
    }

    printf(" done, created %d points.\n", this->skeleton->getCount());

    //memcpy(s, this->skeleton, sizeof(CPointsSet2i));

  }

  void CPointsSet2i::clear() {
    TPointsSet<vector2i>::clear();
    this->binaryMap.clear();
  }

//  void CPointsSet2i::createMask(CHeightmap* h) {
//
//    if (this->points.size() == 0) {
//      return;
//    }
//
//    printf("Creating a mask...\n");
//
//    // Make sure that the points will fit on the heightmap (sanity check)
//    assert(h->getWidth() >= this->topRight[0] && h->getWidth() >= this->topRight[1]);
//
//
////    CImg <unsigned char> mask (h->getWidth(), h->getHeight(), 1, 3, 0);
//
//    CPointsSet2i skeleton;
//    this->generateSkeleton(&skeleton);
//
//    std::vector <float> distances;
//    float maxDistance = 0.0f;
//    //float heightmapSize = sqrt(h->getWidth() * h->getHeight());
//
//    //CHeightmap riverSide (h->getWidth(), h->getHeight());
//    //CHeightmap river (h->getWidth(), h->getHeight());
//
//    // For every point calculate the distance to the skeleton
//    for (std::vector<vector2i>::iterator p = this->points.begin() ; p != this->points.end() ; p++) {
//
//      std::vector <float> skeletonDistances;
//
//      // Calculate distances to every point in the skeleton
//      for (int i = 0 ; i < skeleton.getCount() ; i++) {
//        skeletonDistances.push_back( (skeleton.getPoints()[i] - (*p)).length() );
//      }
//
//      std::sort(skeletonDistances.begin(), skeletonDistances.end());
//
//      distances.push_back(skeletonDistances.front());
//
//      if (skeletonDistances.front() > maxDistance) {
//        maxDistance = skeletonDistances.front();
//      }
//
//    }
//
//    for (unsigned int i = 0 ; i < this->points.size() ; i++) {
//      float value = (1.0f - (distances[i] / (maxDistance))); // + h->getValue(this->points[i][0], this->points[i][1]);
//      h->setValue(this->points[i][0], this->points[i][1], value);
//    }
//
//    // Generate river skeleton
//    CBrownianTree riverSkeleton;
//    riverSkeleton.setBoundingPoints(this);
//    riverSkeleton.createBrownian((int)(this->getCount() * 0.05f), this->getTopRight()[0] + 1, this->getTopRight()[1] + 1);
//
//    // Deposit some river lololol
//    CParticleDeposition depositionFilter;
//    depositionFilter.setParameters(0.02f, 6, (int)(maxDistance));
//    depositionFilter.setBoundingPoints(&skeleton);
//    depositionFilter.setMode(RANDOM);
//    depositionFilter.setVentCenter(skeleton.getMedianPoint());
//    depositionFilter.setParticlesCount(skeleton.getCount() * 100);
//    depositionFilter.apply(&riverSide);
//
//    (*h) = riverSide;

    // We have a very steep downfall river, let's create a mask that will create a nice smooth middle of the river



//    vector2i centroid = this->getCentroid();
//
//    std::vector<float> distances;
//
//    for (std::vector<vector2i>::iterator i = this->points.begin() ; i != this->points.end() ; i++) {
//      distances.push_back(((*i) - centroid).length());
//    }
//
//    // Select the biggest length
//    std::vector<float> copyOfDistances = distances;
//    std::sort(copyOfDistances.begin(), copyOfDistances.end());
//    float maxDistance = copyOfDistances.back();
//
//    for (int i = 0 ; i < this->points.size() ; i++) {
//      h->setValue(this->points[i][0], this->points[i][1], 1.0f - (distances[i] / maxDistance));
//    }
//
//  }

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
  
  bool compareCentroidPairs(const std::pair<int, float> & p1, const std::pair<int, float> & p2) {
    return p1.second < p2.second;
  }

  vector2i& CPointsSet2i::getCentroid() {

    assert(this->points.size() > 0);

    vector2i centroid (0, 0);

    for (std::vector<vector2i>::iterator i = this->points.begin() ; i < this->points.end() ; i++) {
      centroid += (*i);
    }

    centroid[0] = centroid[0] / this->points.size();
    centroid[1] = centroid[1] / this->points.size();

    // If the centroid is not in the set of points, find the closest to it
    if (!this->isPointInSet(centroid)) {
      std::vector< pair<int, float> > distances;
      for (int i = 0 ; i < this->points.size() ; i++) {
        distances.push_back(std::pair<int, float>(i, (this->points[i] - centroid).length()));
      }

      std::sort(distances.begin(), distances.end(), compareCentroidPairs);

      centroid = this->points[(*distances.begin()).first];
    }

    return centroid;
  }
  
  void CPointsSet2i::shrink(CPointsSet2i * shrinked, int amount) {

    printf("Shrinking points... ");

    Array2b map = this->binaryMap;
    std::vector<vector2i> points = this->points;
    std::vector<vector2i> boundaryPoints;

    for (int i = amount ; i > 0 ; i--) {

      boundaryPoints.clear();

      for (int x = 0 ; x < map.size() ; x++) {
        for (int y = 0 ; y < map[0].size() ; y++) {

          if (map[x][y]) {

            for (int s = x - 1 ; s < x + 2 ; s++) {
              for (int t = y - 1 ; t < y + 2 ; t++) {

                bool marked = false;

                if (s < 0 || s >= map.size() || t < 0 || t >= map[0].size()) {
                  marked = true;
                }
                else if (x != s || y != t) {
                  if (map[s][t] == 0) {
                    marked = true;
                  }
                }

                if (marked) {
                  boundaryPoints.push_back(vector2i(x, y));
                }

              }
            }

          }

        }
      }

      // Remove the points found as boundary
      for (std::vector<vector2i>::iterator j = boundaryPoints.begin() ; j != boundaryPoints.end() ; j++) {
        map[(*j)[0]][(*j)[1]] = 0;
      }

    }

    for (unsigned int x = 0 ; x < map.size() ; x++) {
      for (unsigned int y = 0 ; y < map[0].size() ; y++) {
        if (map[x][y]) {
          shrinked->addPoint(vector2i(x, y));
        }
      }
    }

    printf(" done.\n");

  }

  CPointsSet2i::~CPointsSet2i() {
    if (this->skeleton) {
      delete this->skeleton;
    }
  }

} /* namespace archer */
