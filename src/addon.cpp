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

#define PERIPHERAL_ADDON_JOYSTICKS

#include "utils/CommonMacros.h"
#include "xarcade/XArcadeDefines.h"
#include "xarcade/XArcadeDevice.h"
#include "xarcade/XArcadeScanner.h"
#include "xarcade/XArcadeTypes.h"
#include "xarcade/XArcadeUtils.h"

#include "xbmc_addon_dll.h"
#include "kodi_peripheral_dll.h"
#include "kodi_peripheral_utils.hpp"
#include "libXBMC_addon.h"

#include <algorithm>

namespace XARCADE
{
  ADDON::CHelper_libXBMC_addon* FRONTEND = nullptr;

  DeviceVector DEVICES;
}

using namespace XARCADE;

extern "C"
{

ADDON_STATUS ADDON_Create(void* callbacks, void* props)
{
  if (callbacks != nullptr)
  {
    FRONTEND = new ADDON::CHelper_libXBMC_addon;
    if (!FRONTEND || !FRONTEND->RegisterMe(callbacks))
    {
      FRONTEND = nullptr;
      return ADDON_STATUS_PERMANENT_FAILURE;
    }

    CXArcadeScanner::Get().Initailize(FRONTEND);
  }

  return ADDON_GetStatus();
}

void ADDON_Stop()
{
}

void ADDON_Destroy()
{
  CXArcadeScanner::Get().Deinitailize();

  DEVICES.clear();

  SAFE_DELETE(FRONTEND);
}

ADDON_STATUS ADDON_GetStatus()
{
  if (FRONTEND == nullptr)
    return ADDON_STATUS_UNKNOWN;

  return ADDON_STATUS_OK;
}

bool ADDON_HasSettings()
{
  return false;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting*** sSet)
{
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char* settingName, const void* settingValue)
{
  return ADDON_STATUS_OK;
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char* flag, const char* sender, const char* message, const void* data)
{
}

const char* GetPeripheralAPIVersion(void)
{
  return PERIPHERAL_API_VERSION;
}

const char* GetMinimumPeripheralAPIVersion(void)
{
  return PERIPHERAL_MIN_API_VERSION;
}

PERIPHERAL_ERROR GetAddonCapabilities(PERIPHERAL_CAPABILITIES* pCapabilities)
{
  if (pCapabilities == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  pCapabilities->provides_joysticks = true;

  return PERIPHERAL_NO_ERROR;
}

bool SupportsDevice(const char* device_name)
{
  return CXArcadeUtils::IsXArcadeDevice(device_name ? device_name : "");
}

PERIPHERAL_ERROR PerformDeviceScan(unsigned int* peripheral_count, PERIPHERAL_INFO** scan_results)
{
  if (peripheral_count == nullptr || scan_results == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  // Close disconnected devices
  DEVICES.erase(std::remove_if(DEVICES.begin(), DEVICES.end(),
    [](const DevicePtr& device)
    {
      return !device->IsOpen();
    }), DEVICES.end());

  // Open new devices
  DeviceVector newDevices = CXArcadeScanner::Get().GetNewDevices();
  for (auto& device : newDevices)
  {
    if (device->Open())
      DEVICES.emplace_back(std::move(device));
  }

  // Get peripheral info
  JoystickVector joysticks;
  for (auto& device : DEVICES)
    device->GetJoystickInfo(joysticks);

  // Upcast array pointers
  std::vector<ADDON::Peripheral*> peripherals;
  for (auto& joystick : joysticks)
    peripherals.push_back(joystick.get());

  *peripheral_count = peripherals.size();
  ADDON::Peripherals::ToStructs(peripherals, scan_results);

  return PERIPHERAL_NO_ERROR;
}

void FreeScanResults(unsigned int peripheral_count, PERIPHERAL_INFO* scan_results)
{
  ADDON::Peripherals::FreeStructs(peripheral_count, scan_results);
}

PERIPHERAL_ERROR GetEvents(unsigned int* event_count, PERIPHERAL_EVENT** events)
{
  if (event_count == nullptr || events == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  std::vector<ADDON::PeripheralEvent> peripheralEvents;

  for (auto& device : DEVICES)
    device->GetEvents(peripheralEvents);

  *event_count = peripheralEvents.size();
  ADDON::PeripheralEvents::ToStructs(peripheralEvents, events);

  return PERIPHERAL_NO_ERROR;
}

void FreeEvents(unsigned int event_count, PERIPHERAL_EVENT* events)
{
  ADDON::PeripheralEvents::FreeStructs(event_count, events);
}

bool SendEvent(const PERIPHERAL_EVENT* event)
{
  return false;
}

PERIPHERAL_ERROR GetJoystickInfo(unsigned int index, JOYSTICK_INFO* info)
{
  if (info == nullptr)
    return PERIPHERAL_ERROR_INVALID_PARAMETERS;

  JoystickPtr joystick;

  for (auto& device : DEVICES)
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
    joystick->ADDON::Joystick::ToStruct(*info);
    return PERIPHERAL_NO_ERROR;
  }

  return PERIPHERAL_ERROR_NOT_CONNECTED;
}

void FreeJoystickInfo(JOYSTICK_INFO* info)
{
  if (!info)
    return;

  ADDON::Joystick::FreeStruct(*info);
}

PERIPHERAL_ERROR GetFeatures(const JOYSTICK_INFO* joystick, const char* controller_id,
                             unsigned int* feature_count, JOYSTICK_FEATURE** features)
{
  return PERIPHERAL_ERROR_FAILED;
}

void FreeFeatures(unsigned int feature_count, JOYSTICK_FEATURE* features)
{
}

PERIPHERAL_ERROR MapFeatures(const JOYSTICK_INFO* joystick, const char* controller_id,
                             unsigned int feature_count, JOYSTICK_FEATURE* features)
{
  return PERIPHERAL_ERROR_FAILED;
}

void ResetButtonMap(const JOYSTICK_INFO* joystick, const char* controller_id)
{
}

void PowerOffJoystick(unsigned int index)
{
}

} // extern "C"
