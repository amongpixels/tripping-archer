/*
 * CHeightmap.cpp
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#include "CHeightmap.h"

void CHeightmap::init (int w, int h) {

  printf("ROBIE RESOZE");
  this->values.resize(w, vector <float> (h));

  this->width = w;
  this->height = h;
  this->maxValue = 0.0f;

  printf("I PO PTOKACH");
//  for (vector< vector<float> >::iterator i = this->values.begin() ; i < this->values.end() ; i++) {
//    i->resize(h);
//  }

}

int CHeightmap::getWidth() {
  return this->width;
}

int CHeightmap::getHeight() {
  return this->height;
}

float CHeightmap::getValue(int x, int y) {
  return this->values[x][y];
}

void CHeightmap::setValue(int x, int y, float v) {
  this->values[x][y] = v;

  if (v > this->maxValue) {
    this->maxValue = v;
  }
}

void CHeightmap::saveAsPNG(char * path) {

  CImg <unsigned char> img (this->width, this->height, 1, 3, 0);

  //for (vector< vector<float> >::iterator i = this->values.begin() ; i < this->values.end() ; i++) {
  //  for (vector<float>::iterator j = i->begin() ; j < i->end() ; j++) {
  for (int i = 0 ; i < this->values.size() ; i++) {
    for (int j = 0 ; j < this->values[i].size() ; j++) {

      float normalized = (this->values[i][j]) / this->maxValue;

      int color [] = { normalized * 255, normalized * 255, normalized * 255 };
      img.draw_point(i, j, color);

    }
  }

  img.save_png(path);

}

CHeightmap::CHeightmap() {
  this->init(128, 128);
}

CHeightmap::CHeightmap(int w, int h) {
  this->init(w, h);
}
