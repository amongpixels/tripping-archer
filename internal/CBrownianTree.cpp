/*
 * CBrownianTree.cpp
 *
 *  Created on: 18 Feb 2013
 *      Author: dragomir
 */

#include "CBrownianTree.h"

namespace archer
{
  CBrownianTree::CBrownianTree() {
    this->boundingPoints = NULL;
  }
  
  CBrownianTree::CBrownianTree(unsigned int w, unsigned int h, unsigned int ite) {
    this->boundingPoints = NULL;
    this->createBrownian(ite, w, h);
  }

  void CBrownianTree::createBrownian(unsigned int ite, unsigned int w, unsigned int h) {

    //w++;
    //h++;

    assert(w > 0 && h > 0);

    printf("Building a Brownian Tree... ");

    bool grid [w][h];
    bool collision = false;
    bool optionsExhausted [4];

    for (unsigned int x = 0 ; x < w ; x++) {
      for (unsigned int y = 0 ; y < h ; y++) {
        if (this->isPointInSet(vector2i(x, y))) {
          grid[x][y] = 1;
        }
        else {
          grid[x][y] = 0;
        }
      }
    }

    srand(time(NULL));

    // choose a random seed
    if (this->binaryMap.size() == 0) {
      //grid[rand() % w][rand() % h] = 1;
      grid[(int)(w * 0.5)][(int)(h * 0.5)] = 1;
    }

    for (unsigned int i = 0 ; i < ite ; i++) {

      vector2i point;
      collision = false;

      if (this->boundingPoints && this->boundingPoints->getCount() > 0) {
        int index = rand() % this->boundingPoints->getCount();
        //printf("count, rand %d, %d", this->boundingPoints->getCount(), index);
        point = this->boundingPoints->getPoints()[index];
        //helpers::printVector2i(point);
        //point.set(rand() % w, rand() % h);
      }
      else {
        point.set(rand() % w, rand() % h);
      }

      while (!collision) {

        vector2i newPoint (0, 0);
        int direction = rand() % 4;
        int tries = 0;

        while (tries < 3) {

          switch (direction) {
            case 0 : newPoint.set(point[0] == (int)(w - 1) ? point[0] : point[0] + 1, point[1]); break;
            case 1 : newPoint.set(point[0] == 0 ? 0 : point[0] - 1, point[1]); break;
            case 2 : newPoint.set(point[0], point[1] == (int)(h - 1) ? point[1] : point[1] + 1); break;
            case 3 : newPoint.set(point[0], point[1] == 0 ? 0 : point[1] - 1); break;
          }

          if (this->boundingPoints) {
            if (this->boundingPoints->isPointInSet(newPoint)) {
              break;
            }
          }
          else {
            break;
          }

          direction = (direction + 1) % 4;

          tries++;

        }

        // We didn't find a new point, we are screwed basically...
        if (tries == 4) {
          //printf("ive tried far too many times mate\n");
        }
        else {

          point = newPoint;

          // Check if any of the adjacent points is one
          for (int x = std::max(0, point[0] - 1) ; x <= std::min(point[0] + 1, (int)(w - 1)) ; x++) {
            for (int y = std::max(0, point[1] - 1) ; y <= std::min(point[1] + 1, (int)(h - 1)) ; y++) {

              if (x != point[0] || y != point[1]) {
                //if ((x == point[0] && y != point[1]) || (y == point[1] && x != point[0])) {
                  if (grid[x][y]) {

                    if (this->boundingPoints && this->boundingPoints->getCount() > 0) {
                      if (this->boundingPoints->isPointInSet(vector2i(x, y))) {
                        collision = true;
                      }
                    }
                    else {
                      collision = true;
                    }

                  }
                //}
              }

            }
          }

        }


      }

      grid[point[0]][point[1]] = 1;

    }

    // Now make that grid into a point set
    for (unsigned int x = 0 ; x < w ; x++) {
      for (unsigned int y = 0 ; y < h ; y++) {
        if (grid[x][y]) {
          this->addPoint(vector2i(x, y));
        }
      }
    }

    printf("done, created %d points.\n", this->points.size());

  }
  
  void CBrownianTree::setBoundingPoints(CPointsSet2i * p) {
    this->boundingPoints = p;
    //printf("setting bounding points of size %d\n", this->boundingPoints->getCount());
    assert(this->boundingPoints->getCount() > 0);
  }

  CBrownianTree::~CBrownianTree() {

  }

} /* namespace archer */
