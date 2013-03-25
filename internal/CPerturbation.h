/*
 * CPerturbation.h
 *
 *  Created on: 20 Feb 2013
 *      Author: dragomir
 */

#ifndef CPERTURBATION_H_
#define CPERTURBATION_H_

#include "CHeightmap.h"
#include "CSimplexNoise.h"
#include "CPointsSet2i.h"
//#include "IHeightmapProcessor.h"

namespace archer
{
  
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
