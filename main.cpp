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

#include "internal/SSettings.h"
#include "internal/CNoise.h"
#include "internal/CGradientNoise.h"
#include "internal/CFault.h"
#include "internal/CParticleDeposition.h"
#include "internal/CVoronoi.h"
#include "internal/CInputProcessor.h"
#include "internal/CBrownianTree.h"
#include "internal/CPerturbation.h"
#include "internal/CThermalErosion.h"
#include "internal/CHydraulicErosion.h"
#include "internal/CHeightmapRenderer.h"

using namespace std;
using namespace archer;

int main(int argc, char **argv) {

  char * inputPath = "input.png";
  char * outputPath = "output.png";
  char * colorPath = NULL;
  char * renderConfigPath = NULL;

  int flag;

  /**
   * i - input path
   * o - output path
   * c - texture output path
   * r - render config file path
   * t - terrain type
   * v - use voronoi
   * s - render shadows
   *
   * Example call:
   * ./tripping-archer -i input.png -o output.png -c color.png -r alps.cfg t 2 -v -s
   */
  while ((flag = getopt(argc, argv, "i:o:c:r:t:vs")) != -1) {
    switch (flag) {
      case 'i' : inputPath = optarg; break;
      case 'o' : outputPath = optarg; break;
      case 'c' : colorPath = optarg; break;
      case 'r' : renderConfigPath = optarg; break;
      case 'v' : globalSettings.useVoronoi = true; break;
      case 's' : globalSettings.renderShadows = true; break;
      case 't' : globalSettings.terrainType = atoi(optarg); break;
      case '?' :
        if (optopt == 'i' || optopt == 'o' || optopt == 'r' || optopt == 't') {
          printf("Option -%c requires an argument.\n", optopt);
        }
        else {
          printf("Unknown option -%c.\n", optopt);
        }
        return EXIT_FAILURE;
      default: abort();
    }
  }

  printf("Processing input %s...\n", inputPath);

  globalSettings.terrainType = 1;

  // Load clusters
  CInputProcessor inputProcessor;
  inputProcessor.loadFromImage(inputPath);

  CHeightmap heightmap (inputProcessor.getInputWidth(), inputProcessor.getInputHeight());

  // Create base terrain
  CGradientNoise simplexFilter;
  simplexFilter.setOctaves(10);

  srand (time(NULL));

  float ratio = (float)(inputProcessor.getInputHeight()) / (float)(inputProcessor.getInputWidth());

  // Make sure that we always select random and proportional slice of 2d noise
  float randomOffset = 1000.0f / (rand() % 1000);
  simplexFilter.setBounds(
      randomOffset, randomOffset, randomOffset + (float)(inputProcessor.getInputWidth()) / 512.0f, randomOffset + ((float)(inputProcessor.getInputHeight()) / 512.0f) * ratio);
  simplexFilter.setPersistence(0.5);
  simplexFilter.apply(&heightmap);

  if (globalSettings.terrainType == 1) {
    heightmap *= 0.2f;
  }
  else if (globalSettings.terrainType == 2) {
    heightmap *= 0.4f;
  }
  else {
    heightmap *= 0.8f;
  }

//  CThermalErosion baseErosion;
//  //baseErosion.setParameters(0.07f, 0.07f, 0.7f, 0.07f);
//  baseErosion.setTalus(0.1f);
//  baseErosion.setStrength(10);
//  baseErosion.apply(&heightmap);


  // MOUNTAINS /////////////////////////////////////////////////////////////////
  for (unsigned int i = 0 ; i < inputProcessor.getClusters().size() ; i++) {

    if (inputProcessor.getClusters()[i]->channel == 0) {

      std::vector <CPointsSet2i> clusters;

      if (globalSettings.useVoronoi) {
        CVoronoi voronoiGenerator;
        voronoiGenerator.setPointsCount(ceil(inputProcessor.getClusters()[i]->points.getCount() * 0.0005f));
        voronoiGenerator.setBoundingPoints(&inputProcessor.getClusters()[i]->points);
        voronoiGenerator.createClusters(&clusters);
      }
      else {
        clusters.push_back(inputProcessor.getClusters()[i]->points);
      }

      // Now that we've got all clusters sorted out let's generate the mountains
      for (std::vector<CPointsSet2i>::iterator c = clusters.begin() ; c != clusters.end() ; c++) {

        // Generate mountain skeleton
        CBrownianTree skeleton;
        (*c).generateSkeleton(&skeleton);
        skeleton.setBoundingPoints(&(*c));

        int brownianParticlesCount = (int)((*c).getCount() * 0.15f);

        if (globalSettings.terrainType == 1) {

        }
        else if (globalSettings.terrainType == 2) {

        }

        skeleton.createBrownian(brownianParticlesCount, (*c).getTopRight()[0] + 1, (*c).getTopRight()[1] + 1);

        int particlesCount = skeleton.getCount() * 120;
        float particleHeight = 0.02f;
        int searchRadius = 1;
        int heightDifference = 2;

        if (globalSettings.terrainType == 1) {

        }
        else if (globalSettings.terrainType == 2) {

        }

        // Deposit mountains
        CParticleDeposition depositionFilter;
        depositionFilter.setParameters(particleHeight, heightDifference, searchRadius);
        depositionFilter.setBoundingPoints(&skeleton);
        depositionFilter.setMode(RANDOM);
        depositionFilter.setVentCenter(skeleton.getMedianPoint());
        depositionFilter.setParticlesCount(particlesCount);
        depositionFilter.apply(&heightmap);

        // Slight perturbation to get rid of regular lines
//        CPerturbation perturbationFilter;
//        perturbationFilter.setBoundingPoints(&depositionFilter.getActivityMask());
//        perturbationFilter.setMagnitude(4);
//        perturbationFilter.apply(&heightmap);

      }

      // Let's play with voronoi a bit

//      CVoronoi voronoiFilter;
//      CHeightmap testHeightmap (heightmap.getWidth(), heightmap.getHeight());
//
//      voronoiFilter.setBoundingPoints(&inputProcessor.getClusters()[i]->points);
//      voronoiFilter.setPointsCount(ceil(inputProcessor.getClusters()[i]->points.getCount() * 0.0003f));
//
//      testHeightmap.setMaxHeight(2.0f);
//      voronoiFilter.apply(&testHeightmap);
//
//      for (int x = 0 ; x < testHeightmap.getWidth() ; x++) {
//        for (int y = 0 ; y < testHeightmap.getHeight() ; y++) {
//
//          float v = testHeightmap.getValue(x, y);
//
//          v -= 0.5f;
//
//          if (v < 0.0f) {
//            v = 0.0f;
//          }
//
//          testHeightmap.setValue(x, y, 1.0f + v);
//
//        }
//      }
//
//      //testHeightmap *= 0.3f;
//      heightmap *= 0.5f;
//
//      heightmap *= testHeightmap;
//
//      //testHeightmap.zero();
//      //inputProcessor.getClusters()[i]->points.createMask(&testHeightmap);
//
//      testHeightmap.saveAsPNG("testowyvoronoi.png");



    }
  }

  // Some peturbation postprocessing
  CPerturbation perturbationFilter;
  perturbationFilter.setMagnitude(4);
  perturbationFilter.apply(&heightmap);

  // RIVERS ////////////////////////////////////////////////////////////////////
  for (int i = 0 ; i < inputProcessor.getClusters().size() ; i++) {

    if (inputProcessor.getClusters()[i]->channel == 1) {

      CPointsSet2i * clusterPoints = &inputProcessor.getClusters()[i]->points;

      CPointsSet2i skeleton;
      clusterPoints->generateSkeleton(&skeleton);

      std::vector <float> distances;
      float riverWidth = 0.0f;
      float heightmapSize = sqrt(heightmap.getWidth() * heightmap.getHeight());

      CHeightmap riverSide (heightmap.getWidth(), heightmap.getHeight());
      CHeightmap river (heightmap.getWidth(), heightmap.getHeight());

      // For every point calculate the distance to the skeleton
      for (std::vector<vector2i>::const_iterator p = clusterPoints->getPoints().begin() ; p != clusterPoints->getPoints().end() ; p++) {

        bool doTest = true;

        // Check only boundary points, inner points will most def not be the furthest to the skeleton
        for (int x = (*p)[0] - 1 ; x < (*p)[0] + 2 ; x++) {
          for (int y = (*p)[1] - 1 ; y < (*p)[1] + 2 ; y++) {
            if (x != (*p)[0] || y != (*p)[1]) {
              if (!clusterPoints->isPointInSet(vector2i(x, y))) {
                doTest = false;
                break;
              }
            }
          }

          if (!doTest) {
            break;
          }
        }

        if (!doTest) {
          continue;
        }

        std::vector <float> skeletonDistances;

        // Calculate distances to every point in the skeleton
        for (int i = 0 ; i < skeleton.getCount() ; i++) {
          skeletonDistances.push_back( (skeleton.getPoints()[i] - (*p)).length_squared() );
        }

        std::sort(skeletonDistances.begin(), skeletonDistances.end());

        distances.push_back(skeletonDistances.front());

        if (skeletonDistances.front() > riverWidth) {
          riverWidth = skeletonDistances.front();
        }

      }

      riverWidth = 2.0f * sqrt(riverWidth);



//      for (unsigned int i = 0 ; i < clusterPoints->getPoints().size() ; i++) {
//        float value = (1.0f - (distances[i] / maxDistance));
//        //heightmap->setValue(this->points[i][0], this->points[i][1], value * value);
//      }

      // Generate river skeleton
      CBrownianTree riverSkeleton;
      clusterPoints->generateSkeleton(&riverSkeleton);

      riverSkeleton.setBoundingPoints(clusterPoints);
      riverSkeleton.createBrownian((int)(
          riverSkeleton.getCount() * 2),
          clusterPoints->getTopRight()[0] + 1,
          clusterPoints->getTopRight()[1] + 1
      );

      CParticleDeposition depositionFilter;

      float searchRadius = (std::log(riverWidth * 0.1f - 1.0f) + 0.2f) * 2.5f;

      printf("River width is %f and search radius is %f\n", riverWidth, searchRadius);

      // Generate river bank
      depositionFilter.setParameters(0.02f, 2, std::ceil(searchRadius));
      depositionFilter.setMode(RANDOM);
      depositionFilter.setBoundingPoints(&riverSkeleton);
      depositionFilter.setVentCenter(riverSkeleton.getMedianPoint());
      depositionFilter.setParticlesCount(clusterPoints->getCount() * 10);
      depositionFilter.apply(&riverSide);

      // Generate the river
      depositionFilter.setParameters(0.015f, 4, 2);
      depositionFilter.setMode(RANDOM);
      depositionFilter.setBoundingPoints(&riverSkeleton);
      depositionFilter.setParticlesCount(clusterPoints->getCount() * 5);
      depositionFilter.setVentCenter(riverSkeleton.getMedianPoint());
      depositionFilter.apply(&riverSide);


      CPerturbation riverPerturbation;
      riverPerturbation.setMagnitude(3);
      riverPerturbation.apply(&riverSide);

      riverSide.flip();

      riverSide.saveAsPNG("rivermask.png");

      // slap some erosion on that bitch
      CHydraulicErosion hydraulicErosion;
      hydraulicErosion.setParameters(0.25f, 0.25f, 0.6f, 0.25f);
      hydraulicErosion.setStrength(30);
      hydraulicErosion.apply(&riverSide);

      heightmap += riverSide;

      riverSide.saveAsPNG("rivermask-eroded.png");


    }
  }

  // VOLCANOES /////////////////////////////////////////////////////////////////
  for (int i = 0 ; i < inputProcessor.getClusters().size() ; i++) {
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
      float particlesHeight = 0.015f;
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

      // create caldera
      float highestPoint = 0.0f;
      for (std::vector<vector2i>::const_iterator i = clusterPoints->getPoints().begin() ; i != clusterPoints->getPoints().end() ; i++) {
        if (heightmap.getValue((*i)[0], (*i)[1]) > highestPoint) {
          highestPoint = heightmap.getValue((*i)[0], (*i)[1]);
        }
      }

      float threshold = std::min(1.0f, highestPoint * 0.9f);

      for (std::vector<vector2i>::const_iterator i = clusterPoints->getPoints().begin() ; i != clusterPoints->getPoints().end() ; i++) {
        float v = heightmap.getValue((*i)[0], (*i)[1]);
        if (v > threshold) {
          heightmap.setValue((*i)[0], (*i)[1], threshold - (v - threshold));
        }
      }

      heightmap.setMaxHeight(1.0f);

      CPerturbation volcanoPerturbation;
      volcanoPerturbation.setMagnitude(1);
      volcanoPerturbation.setBoundingPoints(&depositionFilter.getActivityMask());
      volcanoPerturbation.apply(&heightmap);

    }
  }

  //heightmap.loadFromPNG("testout.png");

  // erode everything a bit
  CThermalErosion erosion;
  erosion.setTalus(0.05f);
  erosion.setStrength(20);
  erosion.apply(&heightmap);

  // Save everything

  heightmap.saveAsPNG(outputPath);

  CHeightmapRenderer renderer;

  renderer.setRenderShadows(globalSettings.renderShadows);

  if (renderConfigPath) {
    //renderer.loadPreset(renderConfigPath);
  }

  if (colorPath) {
    renderer.renderToPNG(&heightmap, colorPath);
  }

  printf("Done.\n");

  return 0;

}



