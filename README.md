# X-Arcade Tankstick driver for Kodi

![X-Arcade Tankstick](peripheral.xarcade/resources/icon.png)

This is a peripheral add-on for Kodi that enables input from the X-Arcade Tankstick. I found [Xarcade2Jstick](https://github.com/petrockblog/Xarcade2Jstick) to be a helpful reference. Thank you Xarcade2Jstick author!

## Fixing permissions

In general the `/dev/input/event*` devices can only be opened by root. This is to prevent keystroke logging security attacks.

To give non-root users read access to the X-Arcade Tankstick, place [80-xarcade.rules](rules/80-xarcade.rules) in `/etc/udev/rules.d/`.
