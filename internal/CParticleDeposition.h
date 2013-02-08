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
#include <cml/cml.h>

#include "CHeightmap.h"

using namespace cml;

class CParticleDeposition
{
  private:

    int searchRadius;
    float elevationTreshold;
    float particleHeight;

    void deposit (CHeightmap * h, vector2i & particle);

  public:

    CParticleDeposition();

    void apply(CHeightmap * h);

    virtual ~CParticleDeposition();
};

#endif /* CPARTICLEDEPOSITION_H_ */
