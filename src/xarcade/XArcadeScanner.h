/*
 *  Copyright (C) 2016-2021 Garrett Brown
 *  Copyright (C) 2016-2021 Team Kodi
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#pragma once

#include "XArcadeTypes.h"

namespace ADDON
{
  class CHelper_libXBMC_addon;
}

namespace XARCADE
{
  class CXArcadeScanner
  {
  public:
    CXArcadeScanner() = default;

    DeviceVector GetDevices();

  private:
    unsigned int m_nextIndex = 0;
  };
}
