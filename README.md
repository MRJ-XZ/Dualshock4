# Dualshock4
A simple program for dualshock4 to get data from it, to exhibit lightbar demo and to control vibration generators using wired connection.

Dualshock4 is known to the kernel as a **HID device**; like keyboard, mouse, ect... and sends data to the host computer every 4 ms.
This Code is a simple example of reading the transmitted data (buttons status, built-in gyroscope data,...) and keeping it for 
further usages; so it can be implemented in other programs.
> for more information about HID devices:
[introduction to HID](https://docs.kernel.org/hid/index.html)

> for more information about the received data from DS4:
[PS dev wiki](https://www.psdevwiki.com/ps4/DS4-USB)
