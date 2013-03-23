/*
 * CHydraulicErosion.cpp
 *
 *  Created on: 22 Feb 2013
 *      Author: dragomir
 */

#include "CHydraulicErosion.h"

namespace archer
{
  
  CHydraulicErosion::CHydraulicErosion() {
    this->Kr = 0.05f;
    this->Ks = 0.05f;
    this->Ke = 0.5f;
    this->Kc = 0.05f;
  }
  
  void CHydraulicErosion::apply(CHeightmap* h) {

    CHeightmap watermap (h->getWidth(), h->getHeight());
    CHeightmap sedimentmap (h->getWidth(), h->getHeight());

    watermap.zero();
    sedimentmap.zero();

    for (int i = 0 ; i < this->iterations ; i++) {

      // Step 1 - create new water
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {
          watermap.setValue(x, y, watermap.getValue(x, y) + this->Kr);
        }
      }

      // Step 2 - convert some height to sediment
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          float dissolvedAmount = this->Ks * watermap.getValue(x, y);

          h->setValue(x, y, h->getValue(x, y) - dissolvedAmount);
          sedimentmap.setValue(x, y, sedimentmap.getValue(x, y) + dissolvedAmount);

        }
      }

      // Step 3 - examine neighbouring cells and distribute the water
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          float currentWater = watermap.getValue(x, y);
          float currentHeight = h->getValue(x, y) + currentWater;
          float totalHeight = 0.0f;
          float totalDifference = 0.0f;

          std::vector < tuple<vector2i, float> > neighbours;

          for (int s = max(0, x - 1) ; s <= min(x + 1, h->getWidth() - 1) ; s++) {
            for (int t = max(0, y - 1) ; t <= min(y + 1, h->getHeight() - 1) ; t++) {

              // use von-neuman neighbours for speedup
              if (!(s != x && t != y)) {

                float localHeight = h->getValue(s, t) + watermap.getValue(s, t);

                if (currentHeight > localHeight) {
                  // add that point to the list of points where the water will flow along with the height of that point + water height
                  neighbours.push_back(tuple<vector2i, float>(vector2i(s, t), localHeight));
                  totalHeight += localHeight;
                  totalDifference += (currentHeight - localHeight);
                }
              }

            }
          }

          // now go through the points and distribute the water
          for (std::vector < tuple<vector2i, float> >::iterator n = neighbours.begin() ; n != neighbours.end() ; ++n) {

            vector2i examinedPoint = (*n).get<0>();
            float examinedHeight = (*n).get<1>();

            float deltaHeight = currentHeight - (totalHeight / (float)(neighbours.size()));
            float waterToMove = min(currentWater, deltaHeight) * ( (currentHeight - examinedHeight) / totalDifference);

            float sedimentToMove = sedimentmap.getValue(x, y) * (waterToMove / watermap.getValue(x, y));

            watermap.setValue(examinedPoint[0], examinedPoint[1], watermap.getValue(examinedPoint[0], examinedPoint[1]) + waterToMove);
            watermap.setValue(x, y, watermap.getValue(x, y) - waterToMove);

            sedimentmap.setValue(examinedPoint[0], examinedPoint[1], sedimentmap.getValue(examinedPoint[0], examinedPoint[1]) + sedimentToMove);
            sedimentmap.setValue(x, y, sedimentmap.getValue(x, y) - sedimentToMove);

            //printf("moving sedimnet: %f, %f %f %f\n", sedimentToMove, currentHeight, waterToMove, watermap.getValue(x, y));

          }

          if (y > 1) {
            //return;
          }

        }
      }

      // Step 4 - evaporation
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          watermap.setValue(x, y, watermap.getValue(x, y) * (1.0f - this->Ke));
          float maxSediment = this->Kc * watermap.getValue(x, y);

          float depositedSediment = max(0.0f, (sedimentmap.getValue(x, y) - maxSediment));

          //printf("deposited semen %f\n", depositedSediment);

          sedimentmap.setValue(x, y, sedimentmap.getValue(x, y) - depositedSediment);
          h->setValue(x, y, h->getValue(x, y) + depositedSediment);

        }
      }

    }
  }

  void CHydraulicErosion::setParameters(float r, float s, float e, float c) {
    this->Kr = r;
    this->Ks = s;
    this->Ke = e;
    this->Kc = c;
  }

  void CHydraulicErosion::setStrength(int i) {
    this->iterations = i;
  }

  CHydraulicErosion::~CHydraulicErosion() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
