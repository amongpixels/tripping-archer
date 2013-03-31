/*
 * CHeightmapRenderer.h
 *
 *  Created on: 25 Mar 2013
 *      Author: dragomir
 */

#ifndef CHEIGHTMAPRENDERER_H_
#define CHEIGHTMAPRENDERER_H_

#include <cml/cml.h>

#include "CHeightmap.h"
#include "CGradient.h"

using namespace cml;

namespace archer
{
  
  class CHeightmapRenderer
  {
    private:

      bool renderShadows;
      bool renderLight;

      vector3f lightDirection;

      CGradient colors;

    public:
      CHeightmapRenderer();
      void setRenderShadows(bool b);
      void setRenderLight(bool b);
      void renderToPNG(CHeightmap * h, char * path);
      void loadPreset(char * path);
      virtual ~CHeightmapRenderer();
  };

} /* namespace archer */
#endif /* CHEIGHTMAPRENDERER_H_ */
