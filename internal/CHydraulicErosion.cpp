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
    // TODO Auto-generated constructor stub
    
  }
  
  void CHydraulicErosion::apply(CHeightmap* h) {

    float Kr = 0.01; // amount of rain per cell
    float Ks = 0.01; // solubility of the soil
    float Ke = 0.5; // evaporation rate
    float Kc = 0.01; // sediment capacity

    CHeightmap watermap (h->getWidth(), h->getHeight());
    CHeightmap sedimentmap (h->getWidth(), h->getHeight());

    watermap.zero();
    sedimentmap.zero();

    for (int i = 0 ; i < 1 ; i++) {

      // Step 1 - create new water
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {
          watermap.setValue(x, y, watermap.getValue(x, y) + Kr);
        }
      }

      // Step 2 - convert some height to sediment
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          float dissolvedAmount = Ks * watermap.getValue(x, y);

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

              if (s != x || t != y) {

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

            float deltaHeight = currentHeight - (currentHeight / (float)(neighbours.size()));
            float waterToMove = min(currentWater, deltaHeight) * ( (currentHeight - (*n).get<1>()) / totalDifference);

            watermap.setValue((*n).get<0>()[0], (*n).get<0>()[1], watermap.getValue((*n).get<0>()[0], (*n).get<0>()[1]) + waterToMove);
            watermap.setValue(x, y, watermap.getValue(x, y) - waterToMove);

            float sedimentToMove = sedimentmap.getValue(x, y) * (waterToMove / watermap.getValue(x, y));

            sedimentmap.setValue((*n).get<0>()[0], (*n).get<0>()[1], sedimentmap.getValue((*n).get<0>()[0], (*n).get<0>()[1]) + sedimentToMove);
            sedimentmap.setValue(x, y, sedimentmap.getValue(x, y) - sedimentToMove);

            printf("mocing sedimnet: %f, %f\n", sedimentToMove, currentHeight);

          }

        }
      }

      // Step 4 - evaporation
      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          watermap.setValue(x, y, watermap.getValue(x, y) * (1.0f - Ke));
          float maxSediment = Kc * watermap.getValue(x, y);

          float depositedSediment = max(0.0f, (sedimentmap.getValue(x, y) - maxSediment));

          //printf("deposited semen %f\n", depositedSediment);

          sedimentmap.setValue(x, y, sedimentmap.getValue(x, y) - depositedSediment);
          h->setValue(x, y, h->getValue(x, y) + depositedSediment);

        }
      }

    }
  }

  CHydraulicErosion::~CHydraulicErosion() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
