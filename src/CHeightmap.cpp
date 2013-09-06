/*
 * CHeightmap.cpp
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#include "CHeightmap.h"

namespace archer
{
  //float CHeightmap::minHeight = -1.0f;
  //float CHeightmap::maxHeight = 1.0f;

  void CHeightmap::init (int w, int h) {

    this->width = w;
    this->height = h;

    this->values.resize(this->width, std::vector <float> (this->height, 0.0f));

    this->maxValue = 0.0f;
    this->heightScale = 10.0f;
    this->maxHeight = 1.0f;

    this->hasBeenModified = false;

  }

  int CHeightmap::getWidth() const {
    return this->width;
  }

  int CHeightmap::getHeight() const {
    return this->height;
  }

  float CHeightmap::getValue(int x, int y) const {

    assert(x >= 0 && x < this->width);
    assert(y >= 0 && y < this->height);

    return this->values[x][y];
  }

  void CHeightmap::setValue(int x, int y, float v) {

    if (x > -1 && x < this->width && y > -1 && y < this->height) {

      v = std::max(-this->maxHeight, std::min(this->maxHeight, v)); // Cap the value between -1.0f and 1.0f
      this->values[x][y] = v;

      if (v > this->maxValue) {
        this->maxValue = v;
      }

      this->hasBeenModified = true;

    }

  }

  void CHeightmap::setHeightScale(float f) {
    this->heightScale = f;
  }

  void CHeightmap::setMaxHeight(float f) {
    this->maxHeight = f;
  }

  void CHeightmap::saveAs(char * path) {

    CImg <unsigned char> img (this->width, this->height, 1, 3, 0);

    //for (vector< vector<float> >::iterator i = this->values.begin() ; i < this->values.end() ; i++) {
    //  for (vector<float>::iterator j = i->begin() ; j < i->end() ; j++) {
    for (unsigned int i = 0 ; i < this->values.size() ; i++) {
      for (unsigned int j = 0 ; j < this->values[i].size() ; j++) {

        float normalized = (this->values[i][j] + this->maxHeight) * 0.5f;// / this->maxValue;

        int color [] = { normalized * 255, normalized * 255, normalized * 255 };
        img.draw_point(i, j, color);

      }
    }

    img.save(path);

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

    //normalsImg.save_png("normalne.png");

  }

//  void CHeightmap::saveColorMapAsPNG(char* path, bool useShadows) {
//
//    this->calculateNormals();
//
//    vector3f lightDirection (0.5f, 0.8f, 0.5f);
//
//    CImg <unsigned char> colorMap (this->width, this->height, 1, 3, 0);
//    CImg <float> shadowMap (this->width, this->height, 1, 1, 0);
//
//    //int color1 [] = { 188, 169, 91 };
//    //int color2 [] = { 31, 195, 35 };
//    //int color3 [] = { 197, 197, 197 };
//
//    CGradient colors;
//    colors.addStop(-1.0f, 68, 56, 17); // Deep rock
//    colors.addStop(-0.8f, 100, 85, 33); // Shallow rock
//    colors.addStop(-0.1f, 150, 136, 91);
//    colors.addStop(0.1f, 95, 118, 27);
//    colors.addStop(0.25f, 110, 113, 56); // Mountain grass
//    colors.addStop(0.7f, 162, 152, 117); // Mountains
//    colors.addStop(1.0f, 201, 193, 146); // Top of mountains
//
//    CNoise noise;
//    //noise.setBounds(0.0, 0.0, 6.0, 6.0);
//
//    //helpers::printVector3f(colors.getColor(0.0f));
//    //helpers::printVector3f(colors.getColor(1.0f));
//
//    colors.saveAsPNG("gradient.png", 512, 30);
//
//    //shadowMap.fill
//
//    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
//      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
//
//        //vector3f a.set((float) x, this->values[x][y], (float) y);
//
//        float lightIntensity = max(0.0f, min(1.0f, (float)(cml::dot(this->normals[x][y], lightDirection) + noise.getNoise(x,y) * 0.05f)));
//
//        lightIntensity *= lightIntensity;
//
//        float heightValue = this->values[x][y]; //(this->values[x][y] + this->maxHeight) * 0.5f;
//
//        //heightValue = std::max(0.0f, std::min(1.0f, (float)(heightValue + noise.getNoise(x * 0.03, y * 0.03) * 0.02f)));
//
//        color3f c = colors.getColor(heightValue);
//
//        // Some simple raytracing to calculate the shadows
//        vector3f step = lightDirection;
//        step.set(0.5f, 0.11f, 0.5f);
//        vector3f currentPoint (x, this->values[x][y] * this->heightScale, y);
//
//        if (useShadows) {
//          bool inShadow = false;
//
//          while (floor(currentPoint[0]) > -1 && floor(currentPoint[0]) < this->width &&
//              floor(currentPoint[2]) > -1 && floor(currentPoint[2]) < this->height) {
//
//            if (this->values[floor(currentPoint[0])][floor(currentPoint[2])] * this->heightScale > currentPoint[1]) {
//              inShadow = true;
//              break;
//            }
//
//            currentPoint += step;
//
//          }
//
//          float shadow = 1.0f;
//          if (inShadow) {
//            shadow = 0.6f;
//          }
//
//          shadowMap.draw_point(x, y, &shadow);
//        }
//
//        int color [] = {
//            lightIntensity * c[0] * 255,
//            lightIntensity * c[1] * 255,
//            lightIntensity * c[2] * 255
//        };
//
//        colorMap.draw_point(x, y, color);
//
//      }
//    }
//
//    if (useShadows) {
//      shadowMap.blur(2.0f);
//      colorMap.mul(shadowMap);
//    }
//
//    colorMap.save_png(path);
//  }

  CHeightmap & CHeightmap::operator += (CHeightmap & h) {
    assert(h.getWidth() == this->width);
    assert(h.getHeight() == this->height);

    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {

        float newValue = this->values[x][y] + h.getValue(x, y);

        this->setValue(x, y, newValue);
        //
        //        if (newValue > this->maxValue) {
        //          this->maxValue = newValue;
        //        }
        //
        //        this->values[x][y] = newValue;
      }
    }

    return *this;
  }

  CHeightmap & CHeightmap::operator -= (CHeightmap & h) {
    assert(h.getWidth() == this->width);
    assert(h.getHeight() == this->height);

    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        this->setValue(x, y, this->values[x][y] - h.getValue(x, y));
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
      this->hasBeenModified = false;
    }

    return this->normals[x][y];

  }
  
  void CHeightmap::loadFromFile(char* path) {

    CImg <unsigned char> image = CImg<unsigned char>(path);

    this->init(image.width(), image.height());

    for (int x = 0 ; x < image.width() ; x++) {
      for (int y = 0 ; y < image.height() ; y++) {
        this->setValue(x, y, ((float)(*image.data(x, y, 0, 0)) / 255.0f) * 2.0f - this->maxHeight);
      }
    }

  }
  
  CHeightmap & CHeightmap::operator *= (const CHeightmap & h) {
    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {

        float newValue = this->getValue(x, y) * h.getValue(x, y);
        this->setValue(x, y, newValue);

      }
    }

    return *this;
  }
  
  const CHeightmap CHeightmap::operator * (const CHeightmap & h) {
    CHeightmap heightmap = *this;
    heightmap *= h;
    return heightmap;
  }
  
  CHeightmap & CHeightmap::operator = (float f) {
    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        this->setValue(x, y, f);
      }
    }

    return *this;
  }
  
  void CHeightmap::flip() {
    for (unsigned int x = 0 ; x < this->values.size() ; x++) {
      for (unsigned int y = 0 ; y < this->values[x].size() ; y++) {
        this->setValue(x, y, -this->getValue(x, y));
      }
    }
  }
  
  float CHeightmap::getHeightScale() {
    return this->heightScale;
  }
  
  float CHeightmap::getMaxHeight() {
    return this->maxHeight;
  }
  
//  void CHeightmap::levelOut(const CPointsSet2i& p) {
//    for (int i = 0 ; i < p.getCount() ; i++) {
//      this->setValue(p.getPoints()[i][0], p.getPoints()[i][1], 0.0f);
//    }
//  }

  CHeightmap::CHeightmap(int w, int h) {
    this->init(w, h);
  }
}
