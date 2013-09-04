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

  /**
   * The drop point can either move in a continuous but random path or jump
   * randomly from one point to another.
   */
  enum DepositionDropType { MOVING, RANDOM };

  /**
   * Particle depositor algorithm. It drops particles on heightmap either in
   * random fashion or following a path. Particles stack up on the heightmap and
   * build entire heaps. Where the particles are dropped can be controlled with
   * the use of bounding points. Hence, if a Brownian tree is used as
   * the bounding set, particle depositor will produce mountain ridges.
   */
  class CParticleDeposition : IHeightmapProcessor
  {
    private:

      int searchRadius;
      float elevationThreshold;
      float particleHeight;
      DepositionDropType depositionType;

      vector2i ventCenter;
      //int streamsCount;
      int particlesCount;
      std::vector <vector2f> streams;

      CPointsSet2i * boundingPoints;
      CPointsSet2i activityMask;

      void deposit (CHeightmap * h, vector2i & particle, int ite = 0);

    public:

      CParticleDeposition();

      void apply(CHeightmap * h);

      void setBoundingPoints(CPointsSet2i * b);
      void setMode(DepositionDropType t);
      void setVentCenter(const vector2i & c);
      void setParticlesCount(int i);
      void setParameters(float h, int t, int r);

      CPointsSet2i & getActivityMask();

      virtual ~CParticleDeposition();
  };

}

#endif /* CPARTICLEDEPOSITION_H_ */
