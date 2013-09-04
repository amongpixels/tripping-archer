/*
 * CPerturbation.h
 *
 *  Created on: 20 Feb 2013
 *      Author: dragomir
 */

#ifndef CPERTURBATION_H_
#define CPERTURBATION_H_

#include "CHeightmap.h"
#include "CNoise.h"
#include "CPointsSet2i.h"
#include "IHeightmapProcessor.h"

namespace archer
{
  /**
   * Applies a perturbation filter to the heightmap. This is done by moving the
   * points of the heightmap around according to a 2D slice of noise.
   */
  class CPerturbation : public IHeightmapProcessor
  {
    private:

      int magnitude;
      CPointsSet2i * boundingPoints;

    public:
      CPerturbation();
      void apply(CHeightmap * h);
      void setMagnitude(int);
      void setBoundingPoints(CPointsSet2i * b);
      virtual ~CPerturbation();

  };

} /* namespace archer */
#endif /* CPERTURBATION_H_ */
