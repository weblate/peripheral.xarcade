/*
 *  Copyright (C) 2016-2021 Garrett Brown
 *  Copyright (C) 2016-2021 Team Kodi
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#include "XArcadeUtils.h"

using namespace XARCADE;

bool CXArcadeUtils::IsXArcadeDevice(const std::string& deviceName)
{
  return deviceName == "XGaming X-Arcade" ||
         deviceName == "XGaming USBAdapter";
}
