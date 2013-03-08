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
  terrainType = atoi(argv[3]);

  printf("Terrain type %d\n", terrainType);


//  po::options_description desc("Allowed options");
//  desc.add_options()
//      ("input", po::value<string>(&inputPath), "Input image")
//  ;

  //CImg<unsigned char> perlinImage (256, 256, 1, 3, 0);

  //perlinImage.draw_point()

  //CImageBuffer buff (256, 256);
  //CNoise chuj;
  //CSimplexNoise simplexNoise;

  //int color [] = { 255, 0, 255 };

  //buff.putPixel(4, 4, color);

  //chuj.fillShitWithBullshit(&buff);
  //simplexNoise.setOctaves(6);
  //simplexNoise.setBounds(0.0, 0.0, 1.0, 1.0);
  //simplexNoise.setPersistence(0.5);

  //simplexNoise.apply(&perlinImage);

  //perlinImage.save_png("test.png");
  //buff.savePNG("chuj.png");

//  CSimplexNoise simplexFilter;
//  simplexFilter.setOctaves(10);
//  simplexFilter.setBounds(0.0, 0.0, 1.0, 1.0);
//  simplexFilter.setPersistence(0.5);
//  //CFault faultFilter;
//
//  CVoronoi voronoiFilter;
//
//
//
//  CHeightmap heightmap (256, 256);
  //heightmap.zero();
  //heightmap.loadFromPNG("before.png");
  //heightmap.test();

  //simplexFilter.apply(&heightmap);
 // voronoiFilter.apply(&heightmap);

//  CBrownianTree brownianTree ( 256, 256, 4096 * 2 );
//  brownianTree.saveAsPNG("brownian.png");
//
//  CParticleDeposition depositionFilter;
//  depositionFilter.setBoundingPoints(&brownianTree);
//  depositionFilter.setMode(RANDOM);
//  depositionFilter.setVentCenter(brownianTree.getMedianPoint());
//  depositionFilter.setParticlesCount(brownianTree.getCount() * 40);
//
//  depositionFilter.apply(&heightmap);
//  heightmap.normalize();
////
////
////
//  CPerturbation perturbationFilter;
//  perturbationFilter.setMagnitude(0.03f);
//  perturbationFilter.apply(&heightmap);


  CHeightmap baseHeightmap (256, 256);

  CInputProcessor inputProcessor;
  inputProcessor.loadFromImage(inputPath);
  (*inputProcessor.getClusters())[0]->points.createMask(&baseHeightmap);

  baseHeightmap.saveAsPNG("chujciwdupe.png");

  inputProcessor.getSkeleton("skeleton.png");



  //depositionFilter.apply(&heightmap);
  //voronoiFilter.apply(&heightmap);
  //simplexFilter.apply(&baseHeightmap);

//  for (std::vector<SPointCluster *>::iterator i = inputProcessor.getClusters()->begin() ; i < inputProcessor.getClusters()->end() ; i++) {
//    CParticleDeposition depositionFilter;
//
//    depositionFilter.setBoundingPoints((*i)->points.getPoints());
//    depositionFilter.setVentCenter(inputProcessor.getClusterMedian(*(*i)));
//    depositionFilter.setParticlesCount((*i)->points.getCount() * 8);
//
//    depositionFilter.apply(&heightmap);
//  }

  //vector2i chuj (0,0);

  //chuj.set( ((unsigned int)(0 - 1)) % 100, 0);

  //printf("zaraz robie browna %d\n", chuj[0]);


//  heightmap.setHeightScale(5.0f);
//  baseHeightmap.setHeightScale(2.0f);
//
//  baseHeightmap.normalize();
//  heightmap.normalize();
//
//  //baseHeightmap *= 0.5;
//
//  heightmap += baseHeightmap;
//
//
//  faultFilter.apply(&heightmap);



//  heightmap.saveAsPNG("before.png");
//
//
//  CHeightmap eroded = heightmap;
//
//  CThermalErosion thermalErosionFilter;
//  //thermalErosionFilter.apply(&eroded);
//  CHydraulicErosion hydraulicFilter;
//  hydraulicFilter.apply(&eroded);
//
//  eroded.saveAsPNG(outputPath);
//  eroded.saveColorMapAsPNG("color.png");

  printf("DONE\n");

  return 0;


}



