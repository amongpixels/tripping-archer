/*
 * CHeightmapRenderer.cpp
 *
 *  Created on: 25 Mar 2013
 *      Author: dragomir
 */

#include "CHeightmapRenderer.h"

namespace archer
{
  
  CHeightmapRenderer::CHeightmapRenderer() {

    this->lightDirection.set(0.5f, 0.8f, 0.5f);
    
    this->colors.addStop(-1.0f, 51, 40, 20); // Deep rock
    this->colors.addStop(-0.8f, 100, 85, 33); // Shallow rock
    this->colors.addStop(-0.40f, 150, 136, 91); // Early river bank
    this->colors.addStop(-0.15f, 95, 118, 27); // Grass around the river bank
    //this->colors.addStop(0.1f, 110, 113, 56); // Mountain grass
    this->colors.addStop(0.3f, 162, 152, 117); // Mountains
    this->colors.addStop(1.0f, 201, 193, 146); // Top of mountains

//    this->colors.addStop(-1.0f, 68, 56, 17); // Deep rock
//    this->colors.addStop(-0.8f, 100, 85, 33); // Shallow rock
//    this->colors.addStop(-0.1f, 150, 136, 91);
//    this->colors.addStop(0.1f, 95, 118, 27);
//    this->colors.addStop(0.25f, 110, 113, 56); // Mountain grass
//    this->colors.addStop(0.7f, 162, 152, 117); // Mountains
//    this->colors.addStop(1.0f, 201, 193, 146); // Top of mountains

    //this->colors.saveAsPNG("gradient.png", 512, 40);

    this->renderLight = true;
    this->renderShadows = false;

  }
  
  void CHeightmapRenderer::renderToPNG(CHeightmap * h, char * path) {

    printf("Rendering heightmap... ");

    this->lightDirection = this->lightDirection.normalize();

    h->calculateNormals();

    CImg <unsigned char> colorMap (h->getWidth(), h->getHeight(), 1, 3, 0);
    CImg <float> shadowMap (h->getWidth(), h->getHeight(), 1, 1, 0);

    CNoise noise;

    for (int x = 0 ; x < h->getWidth() ; x++) {
      for (int y = 0 ; y < h->getHeight() ; y++) {

        float heightValue = h->getValue(x, y);
        float lightIntensity = 1.0f;

        color3f c = this->colors.getColor(heightValue);

        if (this->renderLight) {

          lightIntensity = max(0.0f, min(1.0f, (float)(cml::dot(h->getNormal(x, y), this->lightDirection) + noise.getNoise(x,y) * 0.05f)));
          lightIntensity *= lightIntensity;

          if (this->renderShadows) {

            // Some simple raytracing to calculate the shadows
            vector3f step = lightDirection * 0.5f;
            float heightScale = h->getHeightScale() * 7.0f;
            //step.set(0.5f, 0.11f, 0.5f);
            vector3f currentPoint (x, h->getValue(x, y) * heightScale, y);

            bool inShadow = false;

            //printf("Height scale is %f", heightScale);

            // Cast the ray and let it fly while within the boundaries
            while (floor(currentPoint[0]) > -1 && floor(currentPoint[0]) < h->getWidth() &&
                floor(currentPoint[2]) > -1 && floor(currentPoint[2]) < h->getHeight() &&
                currentPoint[1] < h->getMaxHeight() * heightScale) {

              if (h->getValue(floor(currentPoint[0]), floor(currentPoint[2])) * heightScale > currentPoint[1]) {
                inShadow = true;
                break;
              }

              currentPoint += step;

            }

            float shadow = 1.0f;
            if (inShadow) {
              shadow = 0.6f;
            }

            shadowMap.draw_point(x, y, &shadow);
          }
        }


        int color [] = {
            lightIntensity * c[0] * 255,
            lightIntensity * c[1] * 255,
            lightIntensity * c[2] * 255
        };

        colorMap.draw_point(x, y, color);

      }
    }

    if (this->renderShadows) {
      shadowMap.blur(2.0f);
      colorMap.mul(shadowMap);
    }

    colorMap.save_png(path);

    printf("done.\n");
  }

  void CHeightmapRenderer::loadPreset(char* path) {

    FILE * cfg = fopen(path, "r");

    assert(cfg);

    fscanf(cfg, "%f %f %f\n", &this->lightDirection[0], &this->lightDirection[1], &this->lightDirection[2]);

    this->colors.reset();

    float stop;
    int r, g, b;

    while (fscanf(cfg, "%f %d %d %d\n", &stop, &r, &g, &b) != EOF) {
      this->colors.addStop(stop, r, g, b);
    }

  }
  
  void CHeightmapRenderer::setRenderShadows(bool b) {
    this->renderShadows = b;
  }
  
  void CHeightmapRenderer::setRenderLight(bool b) {
    this->renderLight = b;
  }

  CHeightmapRenderer::~CHeightmapRenderer() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
