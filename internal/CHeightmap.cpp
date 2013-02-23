/*
 * CHeightmap.cpp
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#include "CHeightmap.h"

namespace archer
{
  void CHeightmap::init (int w, int h) {

    this->width = w;
    this->height = h;

    this->values.resize(this->width, std::vector <float> (this->height, 0.0f));

    this->maxValue = 0.0f;
    this->heightScale = 2.0f;

    this->hasBeenModified = false;

  }

  int CHeightmap::getWidth() {
    return this->width;
  }

  int CHeightmap::getHeight() {
    return this->height;
  }

  float CHeightmap::getValue(int x, int y) {

    assert(x >= 0 && x < this->width);
    assert(y >= 0 && y < this->height);

    return this->values[x][y];
  }

  void CHeightmap::setValue(int x, int y, float v) {
    this->values[x][y] = v;

    if (v > this->maxValue) {
      this->maxValue = v;
    }

    this->hasBeenModified = true;
  }

  void CHeightmap::setHeightScale(float f) {
    this->heightScale = f;
  }

  void CHeightmap::saveAsPNG(char * path) {

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

  CHeightmap::CHeightmap() {
    this->init(128, 128);
  }

  void CHeightmap::calculateNormals() {

    this->normals.clear();
    this->normals.resize(this->width, std::vector <vector3f> (this->height));

    // temp space for all the grid vectors which we will average to get point normals
    vector3f * gridNormals = new vector3f [this->width * this->height];

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
        int gridIndex = x + y * this->width;

        gridNormals[gridIndex] = cml::normalize(cml::cross(a - b, a - c)) * -1.0f;

        if (flip) {
          gridNormals[gridIndex] *= -1.0f;
        }

        //printf("calcualted a normal and its length is: %f\n", gridNormals[gridIndex].length());
      }
    }

    // now that we've got normals per face we need to average them to get normals per vertex
    for (int x = 0 ; x < this->width ; x++) {
      for (int y = 0 ; y < this->height ; y++) {

        std::vector<vector3f *> normalsToAverage;

        normalsToAverage.push_back(&gridNormals[x + y * this->width]);

        //printf("chuj c w dupe %f\n", (*normalsToAverage[normalsToAverage.size()-1]).length());

        if (x > 0) {
          normalsToAverage.push_back(&gridNormals[(x - 1) + y * this->width]);
        }

        if (y > 0) {
          normalsToAverage.push_back(&gridNormals[x + (y - 1) * this->width]);
        }

        if (x > 0 && y > 0) {
          normalsToAverage.push_back(&gridNormals[(x - 1) + (y - 1) * this->width]);
        }

        this->normals[x][y].zero();

        for (std::vector<vector3f *>::iterator n = normalsToAverage.begin() ; n < normalsToAverage.end() ; n++) {
          this->normals[x][y] += *(*n);
          //printf("averaging normal length %f\n", (*n)->length());
        }

        this->normals[x][y] *= (1.0f / normalsToAverage.size());

        //printf("normals to average size %d %f\n", normalsToAverage.size(), this->normals[x][y].length());

        unsigned char color [] = {
            (int) ((this->normals[x][y][0] + 1.0f) * 0.5f * 255),
            (int) ((this->normals[x][y][1] + 1.0f) * 0.5f * 255),
            (int) ((this->normals[x][y][2] + 1.0f) * 0.5f * 255) };

        normalsImg.draw_point(x, y, color);

      }
    }

    normalsImg.save_png("normalne.png");

  }

  void CHeightmap::saveColorMapAsPNG(char* path) {

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

    //colorMap.blur(1.0f);
    colorMap.save_png(path);
  }

  CHeightmap & CHeightmap::operator += (CHeightmap & h) {
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

  CHeightmap & CHeightmap::operator *= (const float & f) {
    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        this->values[x][y] *= f;
      }
    }

    return *this;
  }

  void CHeightmap::zero() {
    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        this->values[x][y] = 0.0f;
      }
    }

    this->maxValue = 0.0f;
  }

  void CHeightmap::normalize() {
    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        this->values[x][y] = this->values[x][y] / this->maxValue;
      }
    }

    this->maxValue = 1.0f;
  }

  void CHeightmap::test() {

    float maxDistance = sqrt(this->width * this->width + this->height * this->height) * 0.3f;

    vector2f middle (this->width * 0.5, this->height * 0.5);

    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        vector2f toMiddle (middle[0] - x, middle[1] - y);
        this->setValue(x, y, pow(1.0f - min(1.0f, toMiddle.length() / maxDistance), 4));
      }
    }

  }

  vector3f CHeightmap::getNormal(int x, int y) {

    assert(x > -1 && x < this->width && y > -1 && y < this->height);

    if (this->hasBeenModified) {
      this->calculateNormals();
    }

    return this->normals[x][y];

  }
  
  void CHeightmap::loadFromPNG(char* path) {

    CImg <unsigned char> image = CImg<unsigned char>(path);

    this->init(image.width(), image.height());

    for (int x = 0 ; x < image.width() ; x++) {
      for (int y = 0 ; y < image.height() ; y++) {

      }
    }

  }

  CHeightmap::CHeightmap(int w, int h) {
    this->init(w, h);
  }
}
