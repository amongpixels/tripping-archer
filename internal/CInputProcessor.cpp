/*
 * CInputProcessor.cpp
 *
 *  Created on: 9 Feb 2013
 *      Author: dragomir
 */

#include "CInputProcessor.h"

namespace archer
{
  CInputProcessor::CInputProcessor() {
    // TODO Auto-generated constructor stub

  }
  
  void CInputProcessor::loadFromImage(char* path) {

    CImg <unsigned char> image (path);

    unsigned char colorToCluster [] = { 255, 0, 0 };

    printf("Clustering input data... ");

    // Look for cluster marked by use of 3 different color channels
    for (int c = 0 ; c < 3 ; c++) {

      // go through all the points and look for clusters
      for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++) {

          unsigned char currentColor [] = { image(x, y, 0, 0), image(x, y, 0, 1), image(x, y, 0, 2) };

          // it's a channel we are interested in clustering
          if ((currentColor[0] == colorToCluster[0]) && (currentColor[1] == colorToCluster[1]) && (currentColor[2] == colorToCluster[2])) {
          //if (image(x, y, 0, c) != 0) {
            vector2i currentPoint (x, y);

            // check if that point already belongs to a cluster
            SPointCluster * cluster = this->findPointInCluster(currentPoint);

            // if not then cluster all the continuous points using flood fill
            if (!cluster) {

              SPointCluster * newCluster = new SPointCluster();

              memcpy(newCluster->color, colorToCluster, sizeof(unsigned char) * 3);

              this->floodFill(image, currentPoint, newCluster);

              this->clusters.push_back(newCluster);

            }
          }
        }
      }

    }

    printf("done, %d clusters found.\n", (int)(this->clusters.size()));

  }

  void CInputProcessor::floodFill(const CImg<unsigned char> & img, const vector2i & point, SPointCluster * cluster) {

    std::vector<vector2i> pointsQueue;
    CImg<unsigned char> imgCopy(img);

    pointsQueue.push_back(point);

    while (!pointsQueue.empty()) {

      vector2i n = pointsQueue[pointsQueue.size() - 1];

      pointsQueue.pop_back();

      if ((n[0] > -1) && (n[0] < img.width()) && (n[1] > -1) && (n[1] < img.height())) {

        unsigned char nColor [] = { imgCopy(n[0], n[1], 0, 0), imgCopy(n[0], n[1], 0, 1), imgCopy(n[0], n[1], 0, 2) };

        //printf("%d %d tutaj dochode %d %d %d\n", n[0], n[1], nColor[0], nColor[1], nColor[2]);

        if ((nColor[0] == cluster->color[0]) && (nColor[1] == cluster->color[1]) && (nColor[2] == cluster->color[2])) {

          const unsigned char newColor [] = { 0, 0, 0 };
          imgCopy.draw_point(n[0], n[1], newColor);

          cluster->points.addPoint(n);
          //printf("maluje punkcior %d %d %d\n", newColor[0], newColor[1], newColor[2]);

          //unsigned char chuj [] = { (*img)(n[0], n[1], 0, 0), (*img)(n[0], n[1], 0, 1), (*img)(n[0], n[1], 0, 2) };

          //printf("the color is now %d %d %d\n", chuj[0], chuj[1], chuj[2]);

          pointsQueue.push_back(vector2i(n[0] - 1, n[1]));
          pointsQueue.push_back(vector2i(n[0] + 1, n[1]));
          pointsQueue.push_back(vector2i(n[0], n[1] + 1));
          pointsQueue.push_back(vector2i(n[0], n[1] - 1));
        }
      }
    }

    //  unsigned char lineColor [] = { 0, 255, 0 };
    //  imgCopy.draw_line(cluster->bottomLeft[0], cluster->bottomLeft[1], cluster->topRight[0], cluster->topRight[1], lineColor);
    //
    //  imgCopy.save_png("output.png");

  }

  SPointCluster * CInputProcessor::findPointInCluster(const vector2i & point) {

    for (std::vector<SPointCluster *>::iterator i = this->clusters.begin() ; i != this->clusters.end() ; i++) {
      if ((*i)->points.isPointInSet(point)) {
        return (*i);
      }
    }

    return NULL;

  }

  std::vector<SPointCluster*> & CInputProcessor::getClusters() {
    return this->clusters;
  }

  vector2i CInputProcessor::getClusterMedian(SPointCluster & cluster) {
    return cluster.points.getMedianPoint();
  }

  void CInputProcessor::getSkeleton(char* path) {

    CImg <unsigned char> image (path);

    printf("chuj %d %d\n", image.width(), image.height());

    Array2i * binaryMap = new Array2i(image.width(), std::vector<int>(image.height(), 0));
    //binaryMap->resize(image.width(), std::vector<int>(image.height(), 0));

    for (int x = 0 ; x < image.width() ; x++) {
      for (int y = 0 ; y < image.height() ; y++) {
        if (image(x, y, 0, 0) == 0) {
          (*binaryMap)[x][y] = 1;
        }
      }
    }

    std::vector< std::pair<int, int> > marked;

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

        for (int x = 1 ; x < image.width() - 1 ; x++) {
          for (int y = 1 ; y < image.height() - 1 ; y++) {

            if ((*binaryMap)[x][y] == 1) {

              int connectivity = this->getConnectivity((*binaryMap), x, y);

              if (connectivity == 1) {
                std::vector< std::pair<int, int> > indices;
                this->createNeghbourIndices(&indices, x, y);

                // Count the neighbours
                int neighbourCount = 0;

                for (int j = 1 ; j < 9 ; j++) {
                  if ((*binaryMap)[indices[j].first][indices[j].second] == 1) {
                    neighbourCount++;
                  }
                }

                if ((neighbourCount >= 2) && (neighbourCount <= 6)) {

                  if (this->passesBackgroundTest((*binaryMap), x, y, bgTests[i * 2])) {
                    if (this->passesBackgroundTest((*binaryMap), x, y, bgTests[i * 2 + 1])) {
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
            (*binaryMap)[(*j).first][(*j).second] = 0;
          }
        }
        else {
          converged = true;
          break;
        }

      }
    }

    CImg <unsigned char> skeleton (image);

    skeleton.fill(0);

    unsigned char c [] = { 255, 255, 255 };

    for (int x = 0 ; x < skeleton.width() ; x++) {
      for (int y = 0 ; y < skeleton.height() ; y++) {
        if ((*binaryMap)[x][y]) {
          skeleton.draw_point(x, y, c);
        }
      }
    }

    skeleton.save_png("skeletonchuj.png");

    //CPointsSet2i s;

    //return s;
  }
  
  int CInputProcessor::getConnectivity(Array2i const & array, int x, int y) {

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

  bool CInputProcessor::passesBackgroundTest(const Array2i & array, int x, int y, int neighbours [3]) {
    std::vector< std::pair<int, int> > indices;
    this->createNeghbourIndices(&indices, x, y);

    for (int i = 0 ; i < 3 ; i++) {
      if (array[indices[neighbours[i]].first][indices[neighbours[i]].second] == 0) {
        return true;
      }
    }

    return false;
  }

  void CInputProcessor::createNeghbourIndices(std::vector< std::pair<int, int> > * pairs, int x, int y) {
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

  CInputProcessor::~CInputProcessor() {
    for (std::vector<archer::SPointCluster *>::iterator i = this->clusters.begin() ; i < this->clusters.end() ; i++) {
      delete (*i);
    }
  }
}
