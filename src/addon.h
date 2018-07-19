/*
 *      Copyright (C) 2016-2018 Garrett Brown
 *      Copyright (C) 2016-2018 Team Kodi
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

#include "xarcade/XArcadeTypes.h"

#include <kodi/addon-instance/Peripheral.h>

namespace XARCADE
{
  class CXArcadeScanner;
}

class ATTRIBUTE_HIDDEN CPeripheralXArcade
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstancePeripheral
{
public:
  CPeripheralXArcade();
  ~CPeripheralXArcade() override;

  ADDON_STATUS Create() override;
  ADDON_STATUS GetStatus() override;
  ADDON_STATUS SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue) override;

  void GetCapabilities(PERIPHERAL_CAPABILITIES &capabilities) override;
  PERIPHERAL_ERROR PerformDeviceScan(unsigned int* peripheral_count, PERIPHERAL_INFO** scan_results) override;
  void FreeScanResults(unsigned int peripheral_count, PERIPHERAL_INFO* scan_results) override;
  PERIPHERAL_ERROR GetEvents(unsigned int* event_count, PERIPHERAL_EVENT** events) override;
  void FreeEvents(unsigned int event_count, PERIPHERAL_EVENT* events) override;
  bool SendEvent(const PERIPHERAL_EVENT* event) override;
  PERIPHERAL_ERROR GetJoystickInfo(unsigned int index, JOYSTICK_INFO* info) override;
  void FreeJoystickInfo(JOYSTICK_INFO* info) override;

private:
  XARCADE::DeviceVector m_devices;
  std::unique_ptr<XARCADE::CXArcadeScanner> m_scanner;
};
