/*
 * CPerturbation.cpp
 *
 * Simple perturbation filter implemented as per
 * "Texturing & modeling - a procedural approach" p 90-91
 *
 *  Created on: 20 Feb 2013
 *      Author: dragomir
 */

#include "CPerturbation.h"

namespace archer
{
  
  CPerturbation::CPerturbation() {
    this->magnitude = 3;
    this->boundingPoints = NULL;
  }
  
  void CPerturbation::apply(CHeightmap * h) {

    CNoise noise;
    CHeightmap heightmapCopy = (*h);

    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {

        if (this->boundingPoints && !this->boundingPoints->isPointInSet(vector2i(x, y))) {
          continue;
        }

        int ss = max(0, min((int)(h->getWidth() - 1), (int)floor(x + this->magnitude * noise.getNormalizedNoise(x * 0.03, y * 0.03))));
        int tt = max(0, min((int)(h->getHeight() - 1), (int)floor(y + this->magnitude * noise.getNormalizedNoise(x * 0.03 + 1.5, y * 0.03 + 6.7))));

        h->setValue(x, y, heightmapCopy.getValue(ss, tt));

      }
    }
  }
  
  void CPerturbation::setMagnitude(int i) {
    this->magnitude = i;
  }

  void CPerturbation::setBoundingPoints(CPointsSet2i * b) {
    this->boundingPoints = b;
    assert(this->boundingPoints->getCount() > 0);
  }

  CPerturbation::~CPerturbation() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
