/*
 * CHydraulicErosion.h
 *
 *  Created on: 22 Feb 2013
 *      Author: dragomir
 *  Adapted from: http://oddlabs.com/download/terrain_generation.pdf
 */

#ifndef CHYDRAULICEROSION_H_
#define CHYDRAULICEROSION_H_

#include <utility>

#include "IHeightmapProcessor.h"

namespace archer
{
  
  /**
   * Applies hydraulic erosion to the heightmap.
   */
  class CHydraulicErosion : public IHeightmapProcessor
  {
    private:

      int iterations;

      float Kr; // amount of rain per cell
      float Ks; // solubility of the soil
      float Ke; // evaporation rate
      float Kc; // sediment capacity

    public:
      CHydraulicErosion();

      /**
       * Amount of rain, soil's solubility, evaporation rate and sediment capacity
       */
      void setParameters(float r, float s, float e, float c);

      /**
       * Number of iterations
       */
      void setStrength(int i);

      void apply(CHeightmap * h);

      virtual ~CHydraulicErosion();
  };

} /* namespace archer */

#endif /* CHYDRAULICEROSION_H_ */
