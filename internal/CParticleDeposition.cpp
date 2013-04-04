/*
 * CParticleDeposition.cpp
 *
 *  Created on: 6 Feb 2013
 *      Author: dragomir
 */

#include "CParticleDeposition.h"

namespace archer
{
  CParticleDeposition::CParticleDeposition() {

    this->searchRadius = 1;
    this->particleHeight = 0.02f;
    this->elevationThreshold = this->particleHeight * 2;//0.06f * 2.0f;
    this->depositionType = MOVING;
    this->particlesCount = 0;

    //this->streamsCount = 32;
    this->ventCenter.zero();

    this->boundingPoints = NULL;

  }
  
  void CParticleDeposition::apply(CHeightmap * h) {

    srand ( time(NULL) );

    // If we stuck all the available particles one on another we will not get over the limit
    //this->particleHeight = 100.0f / this->particlesCount;
    //this->elevationTreshold = this->particleHeight;

    // initialize the streams
    /*for (int i = 0 ; i < this->streamsCount ; i++) {

    vector2f stream (cos((rand() % 360) * cml::constantsf::deg_per_rad()), sin((rand() % 360) * cml::constantsf::deg_per_rad()));
    stream *= (rand() % 5 + 20);

    helpers::printVector2f(stream);

    this->streams.push_back(stream);

  }

  //printf("streamy zainicjalizowane\n");

  for (int o = 0 ; o < 40 ; o++) {

    for (std::vector<vector2f>::iterator i = this->streams.begin() ; i < this->streams.end() ; i++) {

      int length = (int)(*i).length();

      //printf("asdasd %d\n", length);

      vector2f direction = normalize(*i);

      // go along the stream
      for (int l = 0 ; l < length ; l++) {

        //printf("iteracja %d\n", l);
        vector2i dropPoint = this->ventCenter + (direction * l);

        //helpers::printVector2i(dropPoint);

        this->deposit(h, dropPoint);

      }

    }
  }*/

    //printf("let me get this straight, vent center: ");
    //helpers::printVector2i(this->ventCenter);
    //printf("number of particles %d and number of boundingPoints %d\n\n", this->particlesCount, this->boundingPoints->getCount());
    //printf("particle size %f, treshold %f\n", this->particleHeight, this->elevationThreshold);

    printf("Performing particle deposition with %d particles... ", this->particlesCount);
    //return;

    //int particlesCount = 100000;
    vector2i particle;

    srand ( time(NULL) );

    // choose a location of the volcano top
    vector2i currentPosition = this->ventCenter; //(rand() % h->getWidth(), rand() % h->getHeight());

    for (int i = 0 ; i < this->particlesCount + 100 ; i++) {

      // drop the particle
      particle.set(currentPosition[0], currentPosition[1]);
      this->deposit(h, particle);

      if (this->depositionType == MOVING) {

        // select new drop position
        vector2i newPosition;

        bool positionCorrect = false;
        int triesCount = 0;

        while (!positionCorrect) {

          switch (rand() % 4) {
            case 0 : newPosition.set(currentPosition[0] + 1, currentPosition[1]); break;
            case 1 : newPosition.set(currentPosition[0] - 1, currentPosition[1]); break;
            case 2 : newPosition.set(currentPosition[0], currentPosition[1] + 1); break;
            case 3 : newPosition.set(currentPosition[0], currentPosition[1] - 1); break;
          }

          //helpers::printVector2i(newPosition);

          // new drop position has been randomly chosen, now verify that it's correct

          positionCorrect = true;

          if (this->boundingPoints && !this->boundingPoints->isEmpty()) {
            if (!this->boundingPoints->isPointInSet(newPosition)) {
              positionCorrect = false;
            }
          }

          // check the boundaries of heightmap
          if ((newPosition[0] < 0) || (newPosition[1] < 0) || (newPosition[0] >= h->getWidth()) || (newPosition[1] >= h->getHeight())) {
            positionCorrect = false;
          }

          triesCount++;

          if (triesCount > 5) {
            newPosition = currentPosition;
            positionCorrect = true;
          }

          if (positionCorrect) {
            currentPosition = newPosition;
          }

        }
      }
      else if (this->depositionType == RANDOM) {

        // If the mode is RANDOM then just choose one random point
        if (this->boundingPoints && !this->boundingPoints->isEmpty()) {
          currentPosition = (this->boundingPoints->getPoints())[rand() % this->boundingPoints->getCount()];
        }
        else {
          currentPosition = vector2i (rand() % h->getWidth(), rand() % h->getHeight());
        }

      }

    }

    printf("done.\n");


  }

  void CParticleDeposition::deposit(CHeightmap * h, vector2i & particle, int ite) {

    bool positionFound = false;

    // This tends to go into infinite loops when height difference is small. To stop that let's limit number of iterations
    if (ite < 1000) {

      for (int r = 1 ; r <= this->searchRadius ; r++) {

        // search the radius around the particle to see if it's in stable position or not
        for (int x = particle[0] - r ; x <= particle[0] + r ; x++) {
          for (int y = particle[1] - r ; y <= particle[1] + r ; y++) {

            if ((x != particle[0]) || (y != particle[1])) {
              // we are searching in a rectangle first but test proper radius now, also test if we are not out of bounds
              if (
                  ( (x >= 0) && (x < h->getWidth()) && (y >= 0) && (y < h->getHeight()) ) &&
                  ( (x - particle[0]) * (x - particle[0]) + (y - particle[1]) * (y - particle[1]) <= r * r )
              ) {

                float diff = h->getValue(particle[0], particle[1]) + this->particleHeight - h->getValue(x, y);

                if (diff >= this->elevationThreshold) {
                  particle.set(x, y);
                  positionFound = true;

                  break;
                }

              }
            }
          }

          if (positionFound) {
            break;
          }
        }
        if (positionFound) {
          break;
        }
      }

    }

    if (!positionFound) {
      if (particle[0] >= 0 && particle[0] < h->getWidth() && particle[1] >= 0 && particle[1] < h->getHeight()) {
        h->setValue(particle[0], particle[1], h->getValue(particle[0], particle[1]) + this->particleHeight);
        this->activityMask.addPoint(particle); // Keep track of which parts of the heightmap were affected
      }
    }
    else {
      this->deposit(h, particle, ite + 1);
    }


  }

  void CParticleDeposition::setBoundingPoints(CPointsSet2i * b) {
    this->boundingPoints = b;
    assert(this->boundingPoints->getCount() > 0);
  }

  void CParticleDeposition::setVentCenter(const vector2i& c) {
    this->ventCenter.set(c[0], c[1]);
  }

  void CParticleDeposition::setParticlesCount(int i) {
    this->particlesCount = i;
  }

  void CParticleDeposition::setMode(DepositionDropType t) {
    this->depositionType = t;
  }

  void CParticleDeposition::setParameters(float h, int t, int r) {
    this->elevationThreshold = h * t;
    this->particleHeight = h;
    this->searchRadius = r;
  }
  
  CPointsSet2i & CParticleDeposition::getActivityMask() {
    return this->activityMask;
  }

  CParticleDeposition::~CParticleDeposition() {
    // TODO Auto-generated destructor stub
  }
}

