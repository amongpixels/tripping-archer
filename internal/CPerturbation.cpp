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

        int ss = max(0, min((int)(h->getWidth() - 1), (int)floor(x + this->magnitude * noise.getNormalizedNoise(x * 0.03, y * 0.03))));
        int tt = max(0, min((int)(h->getHeight() - 1), (int)floor(y + this->magnitude * noise.getNormalizedNoise(x * 0.03 + 1.5, y * 0.03 + 6.7))));

        // get current coords in 0.0-1.0
        //float nX = (float) x / (float) (h->getWidth() - 1);
        //float nY = (float) y / (float) (h->getHeight() - 1);

//        int ss = min((int)(h->getWidth() - 1), (int)floor((nX + this->magnitude * noise.getNormalizedNoise(x * 0.03, y * 0.03)) * (h->getWidth())));
//        int tt = min((int)(h->getHeight() - 1), (int)floor((nY + this->magnitude * noise.getNormalizedNoise(x * 0.03 + 1.5, y * 0.03 + 6.7)) * (h->getHeight())));
//
//        if (ss < 0) {
//          ss += h->getWidth();
//        }
//
//        if (tt < 0) {
//          tt += h->getHeight();
//        }

        //float ss = max(0.0f, min(1.0f, (float)(nX + this->magnitude * noise.getNormalizedNoise(nX * 10, nY * 10))));
        //float tt = max(0.0f, min(1.0f, (float)(nY + this->magnitude * noise.getNormalizedNoise(nX * 10 + 1.5, nY * 10 + 6.7))));

       // h->setValue(x, y, heightmapCopy.getValue((int)(ss * (h->getWidth() - 1)), (int)(tt * (h->getHeight() - 1))));

        h->setValue(x, y, heightmapCopy.getValue(ss, tt));

      }
    }
  }
  
  void CPerturbation::setMagnitude(int i) {
    this->magnitude = i;
  }

  CPerturbation::~CPerturbation() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
