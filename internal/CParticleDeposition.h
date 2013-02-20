/*
 * CParticleDeposition.h
 *
 *  Created on: 6 Feb 2013
 *      Author: dragomir
 */

#ifndef CPARTICLEDEPOSITION_H_
#define CPARTICLEDEPOSITION_H_

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "IHeightmapProcessor.h"
#include "helpers.h"
#include "CPointsSet2i.h"

namespace archer {

  enum DepositionDropType { MOVING, RANDOM };

  class CParticleDeposition : IHeightmapProcessor
  {
    private:

      int searchRadius;
      float elevationTreshold;
      float particleHeight;
      DepositionDropType depositionType;

      vector2i ventCenter;
      //int streamsCount;
      int particlesCount;
      std::vector <vector2f> streams;

      CPointsSet2i * boundingPoints;

      void deposit (CHeightmap * h, vector2i & particle);

    public:

      CParticleDeposition();

      void apply(CHeightmap * h);

      void setBoundingPoints(CPointsSet2i * b);
      void setMode(DepositionDropType t);
      void setVentCenter(const vector2i & c);
      void setParticlesCount(int i);

      virtual ~CParticleDeposition();
  };

}

#endif /* CPARTICLEDEPOSITION_H_ */
