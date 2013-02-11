/*
 * CParticleDeposition.cpp
 *
 *  Created on: 6 Feb 2013
 *      Author: dragomir
 */

#include "CParticleDeposition.h"

archer::CParticleDeposition::CParticleDeposition() {
  
  this->searchRadius = 1;
  this->elevationTreshold = 1.0f;
  this->particleHeight = 1.0f;

  //this->streamsCount = 32;
  this->ventCenter.set(100, 100);

  this->boundingPoints = NULL;



}

void archer::CParticleDeposition::apply(CHeightmap * h) {

  srand ( time(NULL) );

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

  printf("let me get this straight, vent center: ");
  helpers::printVector2i(this->ventCenter);
  printf("number of particles %d and number of boundingPoints %d\n\n", this->particlesCount, this->boundingPoints->size());

  //return;

  //int particlesCount = 100000;
  vector2i particle;

  srand ( time(NULL) );

  // choose a location of the volcano top
  vector2i currentPosition = this->ventCenter; //(rand() % h->getWidth(), rand() % h->getHeight());

  for (int i = 0 ; i < this->particlesCount ; i++) {

    // drop the particle
    particle.set(currentPosition[0], currentPosition[1]);
    this->deposit(h, particle);

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

      if (this->boundingPoints && !this->boundingPoints->empty()) {

        //printf("searching in bounding points\n");

        bool pointFound = false;

        for (std::vector<vector2i>::iterator j = this->boundingPoints->begin() ; j < this->boundingPoints->end() ; j++) {
          if ((*j) == newPosition) {
            pointFound = true;
          }
        }

        if (!pointFound) {
          positionCorrect = false;
        }

      }

      // check the boundaries of heightmap
      if ((newPosition[0] < 0) || (newPosition[1] < 0) || (newPosition[0] >= h->getWidth()) || (newPosition[1] >= h->getHeight())) {
        positionCorrect = false;
        //printf("POSITION INCORRECT CHUJ W DUPE\n");
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
//    // move the position a bit
//
//
//    if (pos[0] < 0) { pos[0] = h->getWidth() - 1; }
//    if (pos[0] >= h->getWidth()) { pos[0] = 0; }
//
//    if (pos[1] < 0) { pos[1] = h->getHeight() - 1; }
//    if (pos[1] >= h->getHeight()) { pos[1] = 0; }

  }


}

void archer::CParticleDeposition::deposit(CHeightmap * h, vector2i & particle) {

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

void archer::CParticleDeposition::setBoundingPoints(std::vector<vector2i>* b) {
  this->boundingPoints = b;
}

void archer::CParticleDeposition::setVentCenter(const vector2i& c) {
  this->ventCenter.set(c[0], c[1]);
}

void archer::CParticleDeposition::setParticlesCount(int i) {
  this->particlesCount = i;
}

archer::CParticleDeposition::~CParticleDeposition() {
  // TODO Auto-generated destructor stub
}

