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

  }
  
  CBrownianTree::CBrownianTree(unsigned int w, unsigned int h, unsigned int ite) {
    this->init(w, h, ite);
  }

  void CBrownianTree::init(unsigned int w, unsigned int h, unsigned int ite) {

    w++;
    h++;

    bool grid [w][h];

    for (unsigned int x = 0 ; x < w ; x++) {
      for (unsigned int y = 0 ; y < h ; y++) {
        grid[x][y] = 0;
      }
    }

    srand(time(NULL));

    // choose a random seed
    grid[rand() % w][rand() % h] = 1;

    for (unsigned int i = 0 ; i < ite ; i++) {

      vector2i point (rand() % w, rand() % h);
      bool collision = false;

      while (!collision) {
        switch (rand() % 4) {
          case 0 : point.set((point[0] + 1) % w, point[1]); break;
          case 1 : point.set(point[0] == 0 ? w - 1 : (point[0] - 1), point[1]); break;
          case 2 : point.set(point[0], (point[1] + 1) % h); break;
          case 3 : point.set(point[0], point[1] == 0 ? h - 1 : (point[1] - 1)); break;
        }

        // check if any of the adjacent points is one
        if ((grid[(point[0] + 1) % w][point[1]]) || (grid[point[0] == 0 ? w - 1 : (point[0] - 1)][point[1]]) ||
            (grid[point[0]][(point[1] + 1) % h]) || (grid[point[0]][point[1] == 0 ? h - 1 : (point[1] - 1)]) ) {
          collision = true;
        }
      }

      grid[point[0]][point[1]] = 1;

    }

    // now make that grid into a point set
    for (unsigned int x = 0 ; x < w ; x++) {
      for (unsigned int y = 0 ; y < h ; y++) {
        if (grid[x][y]) {
          this->addPoint(vector2i(x, y));
        }
      }
    }

    printf("Brownian tree created! %d\n", this->points.size());

  }
  
  CBrownianTree::~CBrownianTree() {

  }

} /* namespace archer */
