/*
 * CHeightmap.cpp
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#include "CHeightmap.h"

void archer::CHeightmap::init (int w, int h) {

  this->values.resize(w, std::vector <float> (h));

  this->width = w;
  this->height = h;

  this->maxValue = 0.0f;
  this->heightScale = 10.0f;

//  for (vector< vector<float> >::iterator i = this->values.begin() ; i < this->values.end() ; i++) {
//    i->resize(h);
//  }

}

int archer::CHeightmap::getWidth() {
  return this->width;
}

int archer::CHeightmap::getHeight() {
  return this->height;
}

float archer::CHeightmap::getValue(int x, int y) {
  return this->values[x][y];
}

void archer::CHeightmap::setValue(int x, int y, float v) {
  this->values[x][y] = v;

  if (v > this->maxValue) {
    this->maxValue = v;
  }
}

void archer::CHeightmap::saveAsPNG(char * path) {

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

archer::CHeightmap::CHeightmap() {
  this->init(128, 128);
}

void archer::CHeightmap::calculateNormals() {

  this->normals.clear();
  this->normals.resize(this->width, std::vector <vector3f> (this->height));

  CImg <unsigned char> normalsImg (this->width, this->height, 1, 3, 0);

  for (int x = 0 ; x < this->width ; x++) {
    for (int y = 0 ; y < this->height ; y++) {

      //printf("ELLO\n");

      vector3f a, b, c;
      bool flip = false;

      a.set((float) x, this->values[x][y], (float) y);

      if ((x < this->width - 1) && (y < this->height - 1)) {
        b.set((float) (x + 1), this->values[x + 1][y], (float) y);
        c.set((float) x, this->values[x][y + 1], float (y + 1));
      }
      else {

        if (x == this->width - 1) {
          b.set((float) (x - 1), this->values[x - 1][y], (float) y);
          c.set((float) x, this->values[x][y + 1], float (y + 1));
        }
        else if (y == this->height - 1) {
          b.set((float) (x + 1), this->values[x + 1][y], (float) y);
          c.set((float) x, this->values[x][y - 1], float (y - 1));
        }
        else {
          b.set((float) (x - 1), this->values[x - 1][y], (float) y);
          c.set((float) x, this->values[x][y - 1], float (y - 1));
        }

        flip = true;

      }

      //helpers::printVector3f(a);

      this->normals[x][y] = cml::normalize(cml::cross(a - b, a - c)) * -1.0f;

      if (flip) {
        this->normals[x][y] *= -1.0f;
      }

      unsigned char color [] = {
          (int) (this->normals[x][y][0] * 255),
          (int) (this->normals[x][y][1] * 255),
          (int) (this->normals[x][y][2] * 255) };

      normalsImg.draw_point(x, y, color);

    }
  }

  normalsImg.save_png("normalne.png");

}

void archer::CHeightmap::saveColorMapAsPNG(char* path) {

  this->calculateNormals();

  vector3f lightDirection (1.0f, 1.0f, 1.0f);

  CImg <unsigned char> colorMap (this->width, this->height, 1, 3, 0);

  for (unsigned int x = 0 ; x < this->values.size() ; x++) {
     for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {

       //vector3f a.set((float) x, this->values[x][y], (float) y);

       float lightIntensity = max(0.0f, min(1.0f, cml::dot(this->normals[x][y], lightDirection)));

       if (lightIntensity < 1.0f) {
         //printf("intensity %f\n", lightIntensity);
       }

       int color [] = { lightIntensity * 255, lightIntensity * 255, lightIntensity * 255 };
       colorMap.draw_point(x, y, color);

     }
  }

  colorMap.save_png(path);
}

archer::CHeightmap::CHeightmap(int w, int h) {
  this->init(w, h);
}
