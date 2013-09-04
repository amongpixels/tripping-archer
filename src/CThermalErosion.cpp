/*
 * CThermalErosion.cpp
 *
 *  Created on: 21 Feb 2013
 *      Author: dragomir
 */

#include "CThermalErosion.h"

namespace archer
{
  
  CThermalErosion::CThermalErosion() {
    this->talus = 0.007f;
    this->iterations = 10;
  }
  
  void CThermalErosion::apply(CHeightmap * h) {

    float T = this->talus;

    printf("Calculating thermal erosion... ");

    for (int i = 0 ; i < this->iterations ; i++) {

      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          std::vector< tuple<vector2i, float> > vicninities;
          float dMax = 0.0f;
          float dTotal = 0.0f;

          // check the vicinity of the point for height differences
          for (int s = max(0, x - 1) ; s <= min(x + 1, h->getWidth() - 1) ; s++) {
            for (int t = max(0, y - 1) ; t <= min(y + 1, h->getHeight() - 1) ; t++) {

              // skip the point itself
              if (x != s || y != t) {

                float d = h->getValue(x, y) - h->getValue(s, t);

                // if the inclination is bigger than talus then break some of that material
                if (d > T) {
                  vicninities.push_back( tuple<vector2i, float> (vector2i(s, t), d) );

                  dTotal += d;

                  if (d > dMax) {
                    dMax = d;
                  }
                }
              }

            }
          }

          //printf("dmax is %f and dtotal is %f\n", dMax, dTotal);

          // distribute a portion of material over neighbouring cells
          for (std::vector< tuple<vector2i, float> >::iterator i = vicninities.begin() ; i < vicninities.end() ; i++) {
            float value = 0.5f * (dMax - T) * ((*i).get<1>() / dTotal);
            h->setValue(x, y, h->getValue(x, y) - value);
            h->setValue((*i).get<0>()[0], (*i).get<0>()[1], h->getValue((*i).get<0>()[0], (*i).get<0>()[1]) + value);
          }

        }
      }
    }

    printf("done.\n");
  }
  
  void CThermalErosion::setTalus(float f) {
    this->talus = f;
  }
  
  void CThermalErosion::setStrength(int i) {
    this->iterations = i;
  }

  CThermalErosion::~CThermalErosion() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
