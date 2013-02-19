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

namespace archer {

  enum DepositionType { MOVE, PICK };

  class CParticleDeposition : IHeightmapProcessor
  {
    private:

      int searchRadius;
      float elevationTreshold;
      float particleHeight;
      DepositionType depositionType;

      vector2i ventCenter;
      //int streamsCount;
      int particlesCount;
      std::vector <vector2f> streams;

      std::vector <vector2i> * boundingPoints;
      std::vector <vector2i> brownianTree;

      void deposit (CHeightmap * h, vector2i & particle);

    public:

      CParticleDeposition();

      void apply(CHeightmap * h);

      void setBoundingPoints(std::vector <vector2i> * b);
      void setMode(DepositionType t);
      void setVentCenter(const vector2i & c);
      void setParticlesCount(int i);

      virtual ~CParticleDeposition();
  };

}

#endif /* CPARTICLEDEPOSITION_H_ */
