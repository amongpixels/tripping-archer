/*
 * main.cpp
 *
 *  Created on: Jan 15, 2013
 *      Author: dragomir
 */

#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
//#include <boost/program_options.hpp>

#include "internal/CNoise.h"
#include "internal/CSimplexNoise.h"
#include "internal/CFault.h"
#include "internal/CParticleDeposition.h"
#include "internal/CVoronoi.h"
#include "internal/CInputProcessor.h"
#include "internal/CBrownianTree.h"
#include "internal/CPerturbation.h"
#include "internal/CThermalErosion.h"
#include "internal/CHydraulicErosion.h"

using namespace std;
using namespace archer;

//namespace po = boost::program_options;

int main(int argc, char **argv) {

  char * inputPath;
  char * outputPath;
  int terrainType;

  if (argc != 4) {
    printf("Not enough args.\n", argc);
    return 0;
  }

  inputPath = argv[1];
  outputPath = argv[2];

  CHeightmap heightmap (256, 256);

  // Create base terrain
  CSimplexNoise simplexFilter;
  simplexFilter.setOctaves(10);
  simplexFilter.setBounds(0.0, 0.0, 1.0, 1.0);
  simplexFilter.setPersistence(0.5);
  simplexFilter.apply(&heightmap);
  heightmap *= 0.5f;

  // Load clusters
  CInputProcessor inputProcessor;
  inputProcessor.loadFromImage(inputPath);

  for (int i = 0 ; i < inputProcessor.getClusters().size() ; i++) {
    // MOUNTAINS
    if (inputProcessor.getClusters()[i]->channel == 0) {

      // Generate mountain skeleton
      CBrownianTree skeleton;
      inputProcessor.getClusters()[i]->points.generateSkeleton(&skeleton);
      skeleton.setBoundingPoints(&inputProcessor.getClusters()[i]->points);
      skeleton.createBrownian((int)(inputProcessor.getClusters()[i]->points.getCount() * 0.15f),
          inputProcessor.getClusters()[i]->points.getTopRight()[0] + 1,
          inputProcessor.getClusters()[i]->points.getTopRight()[1] + 1
      );

      // Deposit mountains
      CParticleDeposition depositionFilter;
      depositionFilter.setParameters(0.02f, 2);
      depositionFilter.setBoundingPoints(&skeleton);
      depositionFilter.setMode(RANDOM);
      depositionFilter.setVentCenter(skeleton.getMedianPoint());
      depositionFilter.setParticlesCount(skeleton.getCount() * 100);
      depositionFilter.apply(&heightmap);

    }
  }

  // Some peturbation postprocessing
  CPerturbation perturbationFilter;
  perturbationFilter.setMagnitude(0.03f);
  perturbationFilter.apply(&heightmap);

  // Save everything
  heightmap.saveAsPNG(outputPath);
  heightmap.saveColorMapAsPNG("color.png");

  printf("Done.\n");

  return 0;


}



