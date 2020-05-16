/*
 *  Copyright (C) 2016-2020 Garrett Brown
 *  Copyright (C) 2016-2020 Team Kodi
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

#define PERIPHERAL_ADDON_JOYSTICKS

#include "addon.h"

#include "utils/CommonMacros.h"
#include "xarcade/XArcadeDefines.h"
#include "xarcade/XArcadeDevice.h"
#include "xarcade/XArcadeScanner.h"
#include "xarcade/XArcadeTypes.h"
#include "xarcade/XArcadeUtils.h"

#include <kodi/addon-instance/PeripheralUtils.h>

#include <algorithm>

using namespace XARCADE;

CPeripheralXArcade::CPeripheralXArcade() :
  m_scanner(new CXArcadeScanner)
{
}

ADDON_STATUS CPeripheralXArcade::Create()
{
  return GetStatus();
}

CPeripheralXArcade::~CPeripheralXArcade() = default;

ADDON_STATUS CPeripheralXArcade::GetStatus()
{
  return ADDON_STATUS_OK;
}

ADDON_STATUS CPeripheralXArcade::SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue)
{
  return ADDON_STATUS_OK;
}

void CPeripheralXArcade::GetCapabilities(PERIPHERAL_CAPABILITIES &capabilities)
{
  capabilities.provides_joysticks = true;
  capabilities.provides_joystick_rumble = false;
  capabilities.provides_joystick_power_off = false;
  capabilities.provides_buttonmaps = false;
}

PERIPHERAL_ERROR CPeripheralXArcade::PerformDeviceScan(unsigned int* peripheral_count, PERIPHERAL_INFO** scan_results)
{
  if (peripheral_count == nullptr || scan_results == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  // Close disconnected devices
  m_devices.erase(std::remove_if(m_devices.begin(), m_devices.end(),
    [](const DevicePtr& device)
    {
      return !device->IsOpen();
    }), m_devices.end());

  // Open new devices
  DeviceVector newDevices = m_scanner->GetDevices();
  for (auto& device : newDevices)
  {
    if (device->Open())
      m_devices.emplace_back(std::move(device));
  }

  // Get peripheral info
  JoystickVector joysticks;
  for (auto& device : m_devices)
    device->GetJoystickInfo(joysticks);

  // Upcast array pointers
  std::vector<kodi::addon::Peripheral*> peripherals;
  for (auto& joystick : joysticks)
    peripherals.push_back(joystick.get());

  *peripheral_count = peripherals.size();
  kodi::addon::Peripherals::ToStructs(peripherals, scan_results);

  return PERIPHERAL_NO_ERROR;
}

void CPeripheralXArcade::FreeScanResults(unsigned int peripheral_count, PERIPHERAL_INFO* scan_results)
{
  kodi::addon::Peripherals::FreeStructs(peripheral_count, scan_results);
}

PERIPHERAL_ERROR CPeripheralXArcade::GetEvents(unsigned int* event_count, PERIPHERAL_EVENT** events)
{
  if (event_count == nullptr || events == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  std::vector<kodi::addon::PeripheralEvent> peripheralEvents;

  for (auto& device : m_devices)
    device->GetEvents(peripheralEvents);

  *event_count = peripheralEvents.size();
  kodi::addon::PeripheralEvents::ToStructs(peripheralEvents, events);

  return PERIPHERAL_NO_ERROR;
}

void CPeripheralXArcade::FreeEvents(unsigned int event_count, PERIPHERAL_EVENT* events)
{
  kodi::addon::PeripheralEvents::FreeStructs(event_count, events);
}

bool CPeripheralXArcade::SendEvent(const PERIPHERAL_EVENT* event)
{
  return false;
}

PERIPHERAL_ERROR CPeripheralXArcade::GetJoystickInfo(unsigned int index, JOYSTICK_INFO* info)
{
  if (info == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  JoystickPtr joystick;

  for (auto& device : m_devices)
  {
    if (device->GetPeripheralIndex(0) == index ||
        device->GetPeripheralIndex(1) == index)
    {
      joystick = device->GetJoystick(index);
      break;
    }
  }

  if (joystick)
  {
    joystick->kodi::addon::Joystick::ToStruct(*info);
    return PERIPHERAL_NO_ERROR;
  }

  return PERIPHERAL_ERROR_NOT_CONNECTED;
}

void CPeripheralXArcade::FreeJoystickInfo(JOYSTICK_INFO* info)
{
  if (!info)
    return;

  kodi::addon::Joystick::FreeStruct(*info);
}

ADDONCREATOR(CPeripheralXArcade) // Don't touch this!
