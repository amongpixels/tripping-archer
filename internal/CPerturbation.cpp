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
    this->magnitude = 0.05;
    
  }
  
  void CPerturbation::apply(CHeightmap * h) {

    CSimplexNoise noise;
    CHeightmap heightmapCopy = (*h);

    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {

        // get current coords in 0.0-1.0
        float nX = (float) x / (float) (h->getWidth() - 1);
        float nY = (float) y / (float) (h->getHeight() - 1);

        int ss = (int) floor((nX + this->magnitude * noise.getNormalizedNoise(x * 0.03, y * 0.03)) * (h->getWidth())) % h->getWidth();
        int tt = (int) floor((nY + this->magnitude * noise.getNormalizedNoise(x * 0.03 + 1.5, y * 0.03 + 6.7)) * (h->getHeight())) % h->getHeight();

        if (ss < 0) {
          ss += h->getWidth();
        }

        if (tt < 0) {
          tt += h->getHeight();
        }

        h->setValue(x, y, heightmapCopy.getValue(ss, tt));

      }
    }
  }

  CPerturbation::~CPerturbation() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
