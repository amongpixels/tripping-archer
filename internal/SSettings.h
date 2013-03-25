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
  };

  static SSettings globalSettings = { true, true };

}


#endif /* SSETTINGS_H_ */
