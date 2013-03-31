/*
 * SSettings.h
 *
 *  Created on: 24 Mar 2013
 *      Author: dragomir
 */

#ifndef SSETTINGS_H_
#define SSETTINGS_H_

namespace archer
{
  struct SSettings {
      bool useVoronoi;
      bool renderShadows;
      unsigned char terrainType;
  };

  static SSettings globalSettings = { true, false, 2 };

}


#endif /* SSETTINGS_H_ */
