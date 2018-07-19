/*
 *      Copyright (C) 2016 Garrett Brown
 *      Copyright (C) 2016 Team Kodi
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this Program; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "XArcadeScanner.h"
#include "XArcadeDefines.h"
#include "XArcadeDevice.h"
#include "XArcadeUtils.h"
#include "utils/CommonDefines.h" // for INVALID_FD

#include <kodi/AddonBase.h>

#include <fcntl.h>
#include <glob.h>
#include <linux/input.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace XARCADE;

DeviceVector CXArcadeScanner::GetDevices()
{
  DeviceVector devices;

  glob_t pglob;
  int rc = glob("/dev/input/event*", 0, nullptr, &pglob);
  if (rc != 0)
  {
    kodi::Log(ADDON_LOG_ERROR, "Failed to open event devices");
    return devices;
  }

  for (unsigned int ctr = 0; ctr < pglob.gl_pathc; ++ctr)
  {
    const char* filename = pglob.gl_pathv[ctr];

    int fevdev = open(filename, O_RDONLY | O_NONBLOCK);
    if (fevdev == INVALID_FD)
      continue;

    char name[256] = { };
    ioctl(fevdev, EVIOCGNAME(sizeof(name)), name);

    if (CXArcadeUtils::IsXArcadeDevice(name))
    {
      // Found device
      devices.emplace_back(std::make_shared<CXArcadeDevice>(fevdev, m_nextIndex++));
    }
    else
    {
      close(fevdev);
    }
  }

  globfree(&pglob);

  return devices;
}
