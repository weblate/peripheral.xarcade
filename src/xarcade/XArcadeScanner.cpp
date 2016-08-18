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

#include "libXBMC_addon.h"

#include <fcntl.h>
#include <glob.h>
#include <linux/input.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace XARCADE;

CXArcadeScanner::CXArcadeScanner() :
  m_frontend(nullptr),
  m_nextIndex(0)
{
}

CXArcadeScanner& CXArcadeScanner::Get()
{
  static CXArcadeScanner instance;
  return instance;
}

void CXArcadeScanner::Initailize(ADDON::CHelper_libXBMC_addon* frontend)
{
  m_frontend = frontend;
}

void CXArcadeScanner::Deinitailize()
{
  m_frontend = nullptr;
}

DeviceVector CXArcadeScanner::GetNewDevices()
{
  DeviceVector devices;

  if (m_frontend == nullptr)
    return devices;

  glob_t pglob;
  int rc = glob("/dev/input/event*", 0, nullptr, &pglob);
  if (rc != 0)
  {
    m_frontend->Log(ADDON::LOG_ERROR, "Failed to open event devices");
    return devices;
  }

  for (unsigned int ctr = 0; ctr < pglob.gl_pathc; ++ctr)
  {
    const char* filename = pglob.gl_pathv[ctr];

    int fevdev = open(filename, O_RDONLY);
    if (fevdev == INVALID_FD)
      continue;

    char name[256] = { };
    ioctl(fevdev, EVIOCGNAME(sizeof(name)), name);

    if (CXArcadeUtils::IsXArcadeDevice(name))
    {
      // Found device
      DevicePtr device = std::make_shared<CXArcadeDevice>(fevdev, m_nextIndex++, m_frontend);
      if (device->Open())
        devices.emplace_back(std::move(device));
    }
    else
    {
      close(fevdev);
    }
  }

  globfree(&pglob);

  return devices;
}
