/*
 * CParticleDeposition.cpp
 *
 *  Created on: 6 Feb 2013
 *      Author: dragomir
 */

#include "CParticleDeposition.h"

CParticleDeposition::CParticleDeposition() {
  
  this->searchRadius = 1;
  this->elevationTreshold = 1.0f;
  this->particleHeight = 1.0f;

}

void CParticleDeposition::apply(CHeightmap * h) {

  int maxIterations = 100;

  int particlesCount = 10000;
  vector2i particle;
  bool isStable, lowerPositionFound;

  srand ( time(NULL) );

  // choose a location of the volcano top
  vector2i pos (rand() % h->getWidth(), rand() % h->getHeight());

  for (int i = 0 ; i < particlesCount ; i++) {

    // drop the particle
    particle.set(pos[0], pos[1]);
    this->deposit(h, particle);

    // move the position a bit
    switch (rand() % 4) {
      case 0 : pos.set(pos[0] + 1, pos[1]); break;
      case 1 : pos.set(pos[0] - 1, pos[1]); break;
      case 2 : pos.set(pos[0], pos[1] + 1); break;
      case 3 : pos.set(pos[0], pos[1] - 1); break;
    }

    if (pos[0] < 0) { pos[0] = h->getWidth() - 1; }
    if (pos[0] >= h->getWidth()) { pos[0] = 0; }

    if (pos[1] < 0) { pos[1] = h->getHeight() - 1; }
    if (pos[1] >= h->getHeight()) { pos[1] = 0; }

  }


}

void CParticleDeposition::deposit(CHeightmap * h, vector2i & particle) {

  bool positionFound = false;

  // search the radius around the particle to see if it's in stable position or not
  for (int x = particle[0] - 1 ; x <= particle[0] + 1 ; x++) {
    for (int y = particle[1] - 1 ; y <= particle[1] + 1 ; y++) {

      // we are searching in a rectangle first but test proper radius now, also test if we are not out of bounds
      if (
          ( (x >= 0) && (x < h->getWidth()) && (y >= 0) && (y < h->getHeight()) ) &&
          ( (x != particle[0]) || (y != particle[1]) ) &&
          ( (x - particle[0]) * (x - particle[0]) + (y - particle[1]) * (y - particle[1]) <= 1 )
      ) {

        float diff = h->getValue(particle[0], particle[1]) + this->particleHeight - h->getValue(x, y);

        if (diff > this->elevationTreshold) {
          particle.set(x, y);
          positionFound = true;
          break;
        }

      }
    }

    if (positionFound) {
      break;
    }
  }

  if (!positionFound) {
    h->setValue(particle[0], particle[1], h->getValue(particle[0], particle[1]) + this->particleHeight);
  }
  else {
    this->deposit(h, particle);
  }


}

CParticleDeposition::~CParticleDeposition() {
  // TODO Auto-generated destructor stub
}

