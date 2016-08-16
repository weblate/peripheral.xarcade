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
#pragma once

#include "XArcadeTypes.h"

#include <vector>

namespace ADDON
{
  class CHelper_libXBMC_addon;
  class PeripheralEvent;
}

namespace XARCADE
{
  class CXArcadeDevice
  {
  public:
    CXArcadeDevice(int fd, unsigned int index, ADDON::CHelper_libXBMC_addon* frontend);
    ~CXArcadeDevice();

    bool Open();
    bool IsOpen() const { return m_bOpen; }
    void Close();

    void GetJoystickInfo(JoystickVector& joysticks);
    JoystickPtr GetJoystick(unsigned int index);
    unsigned int GetPeripheralIndex(unsigned int playerIndex);

    void GetEvents(std::vector<ADDON::PeripheralEvent>& events);

  private:
    struct KeyToButtonMap
    {
      unsigned int key;
      unsigned int playerIndex;
      unsigned int buttonIndex;
      unsigned int gamepadButton;
    };

    static const std::vector<KeyToButtonMap> m_keyMap;

    // Construction parameters
    int m_fd;
    const unsigned int m_index;
    ADDON::CHelper_libXBMC_addon* const m_frontend;

    bool m_bOpen;
  };
}
