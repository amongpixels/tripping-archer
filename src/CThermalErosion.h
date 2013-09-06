/*
 * CThermalErosion.h
 *
 *  Created on: 21 Feb 2013
 *      Author: dragomir
 *  Adapted from: http://oddlabs.com/download/terrain_generation.pdf
 */

#ifndef CTHERMALEROSION_H_
#define CTHERMALEROSION_H_

#include <utility>

#include "IHeightmapProcessor.h"

namespace archer
{
  /**
   * Thermal erosion implementation.
   */
  class CThermalErosion : public IHeightmapProcessor
  {
    private:

      float talus;
      int iterations;

    public:
      CThermalErosion();

      void setTalus(float f);

      /**
       * Number of iterations
       */
      void setStrength(int i);


      void apply(CHeightmap * h);

      virtual ~CThermalErosion();
  };

} /* namespace archer */
#endif /* CTHERMALEROSION_H_ */
