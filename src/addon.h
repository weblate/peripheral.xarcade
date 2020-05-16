/*
 *  Copyright (C) 2016-2020 Garrett Brown
 *  Copyright (C) 2016-2020 Team Kodi
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
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
