/*
 * CInputProcessor.cpp
 *
 *  Created on: 9 Feb 2013
 *      Author: dragomir
 */

#include "CInputProcessor.h"

archer::CInputProcessor::CInputProcessor() {
  // TODO Auto-generated constructor stub
  
}

void archer::CInputProcessor::loadFromImage(char* path) {

  CImg <unsigned char> image (path);

  unsigned char colorToCluster [] = { 255, 0, 0 };

  // go through all the points and look for clusters
  for (int x = 0 ; x < image.width() ; x++) {
    for (int y = 0 ; y < image.height() ; y++) {

      unsigned char currentColor [] = { image(x, y, 0, 0), image(x, y, 0, 1), image(x, y, 0, 2) };

      // it's a color we are interested in clustering
      if ((currentColor[0] == colorToCluster[0]) && (currentColor[1] == colorToCluster[1]) && (currentColor[2] == colorToCluster[2])) {

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

  printf("DONE, found %d clusters\n", this->clusters.size());

}

void archer::CInputProcessor::floodFill(const CImg<unsigned char> & img, const vector2i & point, SPointCluster * cluster) {

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

archer::SPointCluster * archer::CInputProcessor::findPointInCluster(const vector2i & point) {

  for (std::vector<SPointCluster *>::iterator i = this->clusters.begin() ; i < this->clusters.end() ; i++) {
    if ((*i)->points.isPointInSet(point)) {
      return (*i);
    }
  }

  return 0;

}

std::vector<archer::SPointCluster*>* archer::CInputProcessor::getClusters() {
  return &this->clusters;
}

vector2i archer::CInputProcessor::getClusterMedian(SPointCluster & cluster) {
  return cluster.points.getMedianPoint();
}

archer::CInputProcessor::~CInputProcessor() {
  for (std::vector<archer::SPointCluster *>::iterator i = this->clusters.begin() ; i < this->clusters.end() ; i++) {
    delete (*i);
  }
}
