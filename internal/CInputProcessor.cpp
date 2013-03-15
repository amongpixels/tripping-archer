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
    this->inputHeight = 0;
    this->inputWidth = 0;
  }
  
  void CInputProcessor::loadFromImage(char* path) {

    CImg <unsigned char> image (path);

    this->inputWidth = image.width();
    this->inputHeight = image.height();

    //unsigned char colorToCluster [] = { 255, 0, 0 };

    printf("Clustering input data... ");

    // Look for cluster marked by use of 3 different color channels
    for (int c = 0 ; c < 3 ; c++) {

      // go through all the points and look for clusters
      for (int x = 0 ; x < image.width() ; x++) {
        for (int y = 0 ; y < image.height() ; y++) {

          unsigned char currentColor [] = { image(x, y, 0, 0), image(x, y, 0, 1), image(x, y, 0, 2) };

          // it's a channel we are interested in clustering
          //if ((currentColor[0] == colorToCluster[0]) && (currentColor[1] == colorToCluster[1]) && (currentColor[2] == colorToCluster[2])) {
          if (image(x, y, 0, c) != 0) {
            vector2i currentPoint (x, y);

            // check if that point already belongs to a cluster
            SPointCluster * cluster = this->findPointInCluster(currentPoint);

            // if not then cluster all the continuous points using flood fill
            if (!cluster) {

              SPointCluster * newCluster = new SPointCluster();

              //memcpy(newCluster->color, colorToCluster, sizeof(unsigned char) * 3);
              newCluster->channel = c;

              this->floodFillOnChannel(image, currentPoint, newCluster, c);

              this->clusters.push_back(newCluster);

            }
          }
        }
      }

    }

    printf("done, %d clusters found.\n", (int)(this->clusters.size()));

  }

  void CInputProcessor::floodFillOnChannel (const CImg<unsigned char> & img, const vector2i & point, SPointCluster * cluster, unsigned char channel) {

    std::vector<vector2i> pointsQueue;
    CImg<unsigned char> imgCopy(img);

    pointsQueue.push_back(point);

    unsigned char newColor [] = { 0, 0, 0 };

    while (!pointsQueue.empty()) {

      vector2i n = pointsQueue[pointsQueue.size() - 1];

      pointsQueue.pop_back();

      if ((n[0] > -1) && (n[0] < img.width()) && (n[1] > -1) && (n[1] < img.height())) {
        if (imgCopy(n[0], n[1], 0, channel)) {

          imgCopy.draw_point(n[0], n[1], newColor);

          cluster->points.addPoint(n);

          pointsQueue.push_back(vector2i(n[0] - 1, n[1]));
          pointsQueue.push_back(vector2i(n[0] + 1, n[1]));
          pointsQueue.push_back(vector2i(n[0], n[1] + 1));
          pointsQueue.push_back(vector2i(n[0], n[1] - 1));
        }
      }
    }

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
  
  int CInputProcessor::getInputWidth() {
    return this->inputWidth;
  }
  
  int CInputProcessor::getInputHeight() {
    return this->inputHeight;
  }

  CInputProcessor::~CInputProcessor() {
    for (std::vector<archer::SPointCluster *>::iterator i = this->clusters.begin() ; i < this->clusters.end() ; i++) {
      delete (*i);
    }
  }
}
