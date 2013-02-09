/*
 * CInputProcessor.cpp
 *
 *  Created on: 9 Feb 2013
 *      Author: dragomir
 */

#include "CInputProcessor.h"

CInputProcessor::CInputProcessor() {
  // TODO Auto-generated constructor stub
  
}

void CInputProcessor::loadFromImage(char* path) {

  CImg <unsigned char> image (path);

  vector2i startingPoint (0, 0);

  SPointCluster cluster;
  cluster.color[0] = 0;
  cluster.color[1] = 200;
  cluster.color[2] = 0;
  cluster.points.clear();

  this->floodFill(&image, &startingPoint, &cluster);

  image.save_png("tescik.png");

  printf("DONE\n");

//  for (int x = 0 ; x < image.width() ; x++) {
//    for (int y = 0 ; y < image.height() ; y++) {
//
//
//
//    }
//  }

}

void CInputProcessor::floodFill(CImg<unsigned char> * img, vector2i * point, SPointCluster * cluster) {

  std::vector<vector2i> pointsQueue;

  pointsQueue.push_back(*point);

  // remember the colour of the cluster
  for (int i = 0 ; i < 3 ; i++) {
    cluster->color[i] = img->data((*point)[0], (*point)[1])[i];
  }

  printf("start n %d %d\n", (*point)[0], (*point)[1]);

  printf("color for the cluster is %d %d %d\n", cluster->color[0], cluster->color[1], cluster->color[2]);
  //printf("first point is %d %d", (*point)[0], (*point)[1]);

  while (!pointsQueue.empty()) {

    vector2i n = pointsQueue[pointsQueue.size() - 1];

    //printf("start n %d %d\n", n[0], n[1]);

    pointsQueue.pop_back();

    if ((n[0] > -1) && (n[0] < img->width()) && (n[1] > -1) && (n[1] < img->height())) {

      unsigned char nColor [] = { (*img)(n[0], n[1], 0, 0), (*img)(n[0], n[1], 0, 1), (*img)(n[0], n[1], 0, 2) };

      printf("%d %d tutaj dochode %d %d %d\n", n[0], n[1], nColor[0], nColor[1], nColor[2]);

      if ((nColor[0] == cluster->color[0]) && (nColor[1] == cluster->color[1]) && (nColor[2] == cluster->color[2])) {

        const unsigned char newColor [] = { 0, 0, 0 };
        img->draw_point(n[0], n[1], newColor);

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

}

bool CInputProcessor::isPointInCluster(CImg<unsigned char> * img, vector2i point) {
  return false;
}

CInputProcessor::~CInputProcessor() {
  // TODO Auto-generated destructor stub
}

