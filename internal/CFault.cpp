/*
 * CFault.cpp
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#include "CFault.h"

void CFault::apply(CHeightmap * h) {

  this->iterations = 20;

  printf("ja zyje");

  int x1, x2, y1, y2;

  float initHDisp = 4.0f;
  float hDisp = initHDisp;

  for (int i = 0 ; i < this->iterations ; i++) {

    printf("jade iteracje numero %d %d\n", i, h->getWidth());

    // chose two random points
    x1 = rand() % h->getWidth();
    y1 = rand() % h->getHeight();

    printf("AKSJdhakjsdhakd asdkjhad ");

    do {
      x2 = rand() % h->getWidth();
      y2 = rand() % h->getHeight();
    }
    while ( (x1 == x2) && (y1 == y2) );

    vector3i p1 (x1, y1, 0);
    vector3i p2 (x2, y2, 0);

    vector3i vp = p2 - p1;

    // go through every point on the heightfield and see which side of the line it is on
    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {

        //vector2i o (x, y);
        vector3i vo = vector3i(x, y, 0) - p1;

        // left side of the line
        if (cross(vp, vo)[2] > 0) {
          h->setValue(x, y, h->getValue(x, y) + hDisp);
        }
        // right side
        else {
          h->setValue(x, y, h->getValue(x, y) - hDisp);
        }


        //cml::cross(&vp, &vo);

      }
    }


  }

}

