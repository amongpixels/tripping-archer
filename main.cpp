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
      depositionFilter.setParameters(0.02f, 2, 1);
      depositionFilter.setBoundingPoints(&skeleton);
      depositionFilter.setMode(RANDOM);
      depositionFilter.setVentCenter(skeleton.getMedianPoint());
      depositionFilter.setParticlesCount(skeleton.getCount() * 100);
      depositionFilter.apply(&heightmap);

    }
  }

  // Some peturbation postprocessing
  CPerturbation perturbationFilter;
  perturbationFilter.setMagnitude(0.02f);
  perturbationFilter.apply(&heightmap);

  for (int i = 0 ; i < inputProcessor.getClusters().size() ; i++) {
    // RIVERS
    if (inputProcessor.getClusters()[i]->channel == 1) {

      //CHeightmap riverMask (256, 256);
      //CHeightmap river (256, 256);

      CPointsSet2i * clusterPoints = &inputProcessor.getClusters()[i]->points;

      CPointsSet2i skeleton;
      clusterPoints->generateSkeleton(&skeleton);

      std::vector <float> distances;
      float maxDistance = 0.0f;
      float heightmapSize = sqrt(heightmap.getWidth() * heightmap.getHeight());

      CHeightmap riverSide (heightmap.getWidth(), heightmap.getHeight());
      CHeightmap river (heightmap.getWidth(), heightmap.getHeight());

      // For every point calculate the distance to the skeleton
      for (std::vector<vector2i>::const_iterator p = clusterPoints->getPoints().begin() ; p != clusterPoints->getPoints().end() ; p++) {

        std::vector <float> skeletonDistances;

        // Calculate distances to every point in the skeleton
        for (int i = 0 ; i < skeleton.getCount() ; i++) {
          skeletonDistances.push_back( (skeleton.getPoints()[i] - (*p)).length() );
        }

        std::sort(skeletonDistances.begin(), skeletonDistances.end());

        distances.push_back(*skeletonDistances.begin());

        if (*skeletonDistances.begin() > maxDistance) {
          maxDistance = *skeletonDistances.begin();
        }

      }

      for (unsigned int i = 0 ; i < clusterPoints->getPoints().size() ; i++) {
        float value = (1.0f - (distances[i] / maxDistance));
        //heightmap->setValue(this->points[i][0], this->points[i][1], value * value);
      }

      // Generate river skeleton
      CBrownianTree riverSkeleton;
      clusterPoints->generateSkeleton(&riverSkeleton);
      riverSkeleton.setBoundingPoints(clusterPoints);
      riverSkeleton.createBrownian((int)(
          clusterPoints->getCount() * 0.05f),
          clusterPoints->getTopRight()[0] + 1,
          clusterPoints->getTopRight()[1] + 1
      );

      // Deposit some river lololol
      CParticleDeposition depositionFilter;
      depositionFilter.setParameters(0.01f, 3, (int)(maxDistance * 0.15f));
      depositionFilter.setBoundingPoints(&riverSkeleton);
      depositionFilter.setMode(RANDOM);
      depositionFilter.setVentCenter(riverSkeleton.getMedianPoint());
      depositionFilter.setParticlesCount(riverSkeleton.getCount() * 200);
      depositionFilter.apply(&riverSide);

      CPerturbation riverPerturbation;
      riverPerturbation.setMagnitude(0.03f);
      riverPerturbation.apply(&riverSide);

      CHeightmap cpy = riverSide;

      for (int x = 0 ; x < riverSide.getWidth() ; x++) {
        for (int y = 0 ; y < riverSide.getHeight() ; y++) {

        }
      }

      heightmap -= riverSide;
      //float k
      //heightmap -= river;

      riverSide.saveAsPNG("rivermask.png");


    }
  }

  for (int i = 0 ; i < inputProcessor.getClusters().size() ; i++) {
    // VOLCANOS
    if (inputProcessor.getClusters()[i]->channel == 2) {

      CPointsSet2i * clusterPoints = &inputProcessor.getClusters()[i]->points;

      vector2i ventCenter = clusterPoints->getCentroid();

      CPointsSet2i boundingPoints;

//      // Let's make sure that there's one vent centre
//
//      for (int x = std::max(0, ventCenter[0] - 3) ; x < std::min(heightmap.getWidth(), ventCenter[0] + 4) ; x++) {
//        for (int y = std::max(0, ventCenter[1] - 3) ; y < std::min(heightmap.getHeight(), ventCenter[1] + 4) ; y++) {
//          //heightmap.setValue(x, y, 0.5f);
//          boundingPoints.addPoint(vector2i(x, y));
//        }
//      }

      float clusterSize = sqrt(clusterPoints->getWidth() * clusterPoints->getHeight());
      int particlesPerSquare = 20;
      int totalParticles = (clusterPoints->getCount() * particlesPerSquare);
      float particlesHeight = 0.02f;
      printf("Cluster size is %f\n", clusterSize);
      //boundingPoints.createBrownian(20, 256, 256);
      clusterPoints->shrink(&boundingPoints, (int)(clusterSize * 0.15f));

      //boundingPoints.saveAsPNG("shrinked.png", 256, 256);

      heightmap.setMaxHeight(2.0f);

      CParticleDeposition depositionFilter;
      depositionFilter.setParameters(particlesHeight, 2, 1);
      depositionFilter.setBoundingPoints(&boundingPoints);
      depositionFilter.setMode(MOVING);
      depositionFilter.setVentCenter(ventCenter);
      depositionFilter.setParticlesCount(totalParticles);
      depositionFilter.apply(&heightmap);

      // to calculate where the caldera should be
      float avgHeight = particlesHeight * clusterPoints->getCount() * 0.01f;

      // create caldera
      float highestPoint = 0.0f;
      for (std::vector<vector2i>::const_iterator i = clusterPoints->getPoints().begin() ; i != clusterPoints->getPoints().end() ; i++) {
        if (heightmap.getValue((*i)[0], (*i)[1]) > highestPoint) {
          highestPoint = heightmap.getValue((*i)[0], (*i)[1]);
        }
      }

      float threshold = highestPoint * 0.7f;

      for (std::vector<vector2i>::const_iterator i = clusterPoints->getPoints().begin() ; i != clusterPoints->getPoints().end() ; i++) {
        float v = heightmap.getValue((*i)[0], (*i)[1]);
        if (v > threshold) {
          heightmap.setValue((*i)[0], (*i)[1], threshold - (v - threshold));
        }
      }

      heightmap.setMaxHeight(1.0f);

    }
  }

  // Some peturbation postprocessing
  CPerturbation finalPerturbationFilter;
  finalPerturbationFilter.setMagnitude(0.03f);
  finalPerturbationFilter.apply(&heightmap);

  // Save everything
  heightmap.saveAsPNG(outputPath);
  heightmap.saveColorMapAsPNG("color.png");

  printf("Done.\n");

  return 0;


}



