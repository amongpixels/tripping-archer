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

int main(int argc, char **argv) {

  //CImg<unsigned char> perlinImage(256, 256, 1, 3, 0);

  //perlinImage.draw_point()
  //printf("huj");

  CImageBuffer buff (256, 256);
  CNoise chuj;

  int color [] = { 255, 0, 255 };

  //buff.putPixel(4, 4, color);

  chuj.fillShitWithBullshit(&buff);


  buff.savePNG("chuj.png");


  return 0;


}



