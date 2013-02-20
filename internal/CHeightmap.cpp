/*
 * CHeightmap.cpp
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#include "CHeightmap.h"

void archer::CHeightmap::init (int w, int h) {

  this->width = w;
  this->height = h;

  this->values.resize(this->width, std::vector <float> (this->height));

  this->maxValue = 0.0f;
  this->heightScale = 5.0f;

}

int archer::CHeightmap::getWidth() {
  return this->width;
}

int archer::CHeightmap::getHeight() {
  return this->height;
}

float archer::CHeightmap::getValue(int x, int y) {

  assert(x >= 0 && x < this->width);
  assert(y >= 0 && y < this->height);

  return this->values[x][y];
}

void archer::CHeightmap::setValue(int x, int y, float v) {
  this->values[x][y] = v;

  if (v > this->maxValue) {
    this->maxValue = v;
  }
}

void archer::CHeightmap::setHeightScale(float f) {
  this->heightScale = f;
}

void archer::CHeightmap::saveAsPNG(char * path) {

  CImg <unsigned char> img (this->width, this->height, 1, 3, 0);

  //for (vector< vector<float> >::iterator i = this->values.begin() ; i < this->values.end() ; i++) {
  //  for (vector<float>::iterator j = i->begin() ; j < i->end() ; j++) {
  for (unsigned int i = 0 ; i < this->values.size() ; i++) {
    for (unsigned int j = 0 ; j < this->values[i].size() ; j++) {

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

      vector3f a, b, c;
      bool flip = false;

      a.set((float) x, this->values[x][y] * this->heightScale, (float) y);

      if ((x < this->width - 1) && (y < this->height - 1)) {
        b.set((float) (x + 1), this->values[x + 1][y] * this->heightScale, (float) y);
        c.set((float) x, this->values[x][y + 1] * this->heightScale, float (y + 1));
      }
      else {

        if (x == this->width - 1) {
          b.set((float) (x - 1), this->values[x - 1][y] * this->heightScale, (float) y);
          c.set((float) x, this->values[x][y + 1] * this->heightScale, float (y + 1));
        }
        else if (y == this->height - 1) {
          b.set((float) (x + 1), this->values[x + 1][y] * this->heightScale, (float) y);
          c.set((float) x, this->values[x][y - 1] * this->heightScale, float (y - 1));
        }
        else {
          b.set((float) (x - 1), this->values[x - 1][y] * this->heightScale, (float) y);
          c.set((float) x, this->values[x][y - 1] * this->heightScale, float (y - 1));
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

  vector3f lightDirection (1.0f, 0.5f, 1.0f);

  CImg <unsigned char> colorMap (this->width, this->height, 1, 3, 0);

  int color1 [] = { 188, 169, 91 };
  int color2 [] = { 31, 195, 35 };
  int color3 [] = { 197, 197, 197 };

  for (unsigned int x = 0 ; x < this->values.size() ; x++) {
     for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {

       //vector3f a.set((float) x, this->values[x][y], (float) y);

       float lightIntensity = max(0.0f, min(1.0f, cml::dot(this->normals[x][y], lightDirection)));

       if (lightIntensity < 1.0f) {
         //printf("intensity %f\n", lightIntensity);
       }

       float heightValue = this->values[x][y] / this->maxValue;

//       float pC1 = max(0.0f, -3.0f * heightValue + 1.0f);
//       float pC2 = max(0.0f, -3.0f * (heightValue - 0.3f) + 1.0f);
//       float pC3 = max(0.0f, -3.0f * (heightValue - 0.6f) + 1.0f);
//
//       if (pC1 > 1.0f) { pC1 = 0.0f; }
//       if (pC2 > 1.0f) { pC2 = 0.0f; }
//       if (pC3 > 1.0f) { pC3 = 0.0f; }
//
//       int color [] = {
//           lightIntensity * (color1[0] * pC1 + color2[0] * pC2 + color3[0] * pC3),
//           lightIntensity * (color1[1] * pC1 + color2[1] * pC2 + color3[1] * pC3),
//           lightIntensity * (color1[2] * pC1 + color2[2] * pC2 + color3[2] * pC3)
//       };

       int color [] = {
           lightIntensity * 255,
           lightIntensity * 255,
           lightIntensity * 255
       };

       colorMap.draw_point(x, y, color);

     }
  }

  colorMap.blur(1.0f);
  colorMap.save_png(path);
}

archer::CHeightmap & archer::CHeightmap::operator += (archer::CHeightmap & h) {
  assert(h.getWidth() == this->width);
  assert(h.getHeight() == this->height);

  for (unsigned int x = 0 ; x < this->values.size() ; x++) {
    for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {

      float newValue = this->values[x][y] + h.getValue(x, y);

      if (newValue > this->maxValue) {
        this->maxValue = newValue;
      }

      this->values[x][y] = newValue;
    }
  }

  return *this;
}

archer::CHeightmap & archer::CHeightmap::operator *= (const float & f) {
  for (unsigned int x = 0 ; x < this->values.size() ; x++) {
    for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
      this->values[x][y] *= f;
    }
  }

  return *this;
}

void archer::CHeightmap::zero() {
  for (unsigned int x = 0 ; x < this->values.size() ; x++) {
    for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
      this->values[x][y] = 0.0f;
    }
  }
}

void archer::CHeightmap::normalize() {
  for (unsigned int x = 0 ; x < this->values.size() ; x++) {
    for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
      this->values[x][y] = this->values[x][y] / this->maxValue;
    }
  }

  this->maxValue = 1.0f;
}

archer::CHeightmap::CHeightmap(int w, int h) {
  this->init(w, h);
}
