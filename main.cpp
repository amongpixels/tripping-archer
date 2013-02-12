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

  CFault faultFilter;
  CParticleDeposition depositionFilter;
  CHeightmap heightmap ( 256, 256 );
  CVoronoi voronoiFilter;

  CInputProcessor inputProcessor;
  inputProcessor.loadFromImage(inputPath);

  for (std::vector<SPointCluster *>::iterator i = inputProcessor.getClusters()->begin() ; i < inputProcessor.getClusters()->end() ; i++) {
    CParticleDeposition depositionFilter;

    depositionFilter.setBoundingPoints(&(*i)->points);
    depositionFilter.setVentCenter(inputProcessor.findClusterMedian(*(*i)));
    depositionFilter.setParticlesCount((*i)->points.size() * 5);

    depositionFilter.apply(&heightmap);
  }




  //faultFilter.apply(&heightmap);

  //depositionFilter.apply(&heightmap);
  //voronoiFilter.apply(&heightmap);

  heightmap.saveAsPNG(outputPath);
  heightmap.saveColorMapAsPNG("color.png");

  printf("DONE\n");

  return 0;


}



