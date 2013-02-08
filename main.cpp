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

//#include "internal/CImageBuffer.h"
#include "internal/CNoise.h"
#include "internal/CSimplexNoise.h"
#include "internal/CFault.h"
#include "internal/CParticleDeposition.h"

int main(int argc, char **argv) {

  //CImg<unsigned char> perlinImage (256, 256, 1, 3, 0);

  //perlinImage.draw_point()
  //printf("huj");

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

  printf("CHUJ CI W DUPE");

  CFault faultFilter;
  CParticleDeposition depositionFilter;
  CHeightmap heightmap ( 200, 200 );

  //faultFilter.apply(&heightmap);

  depositionFilter.apply(&heightmap);

  heightmap.saveAsPNG("cipka.png");




  return 0;


}



