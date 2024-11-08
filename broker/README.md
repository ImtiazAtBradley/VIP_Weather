# Overview

The broker program is designed to go along with the weather station website, and the various devices. The broker queries a radio attached to a device on a serial port, and then parses & passes received messages on to a redis database, as indicated by the user. 

NOTE: This software will only work with POSIX compliant systems (linux/MacOs). Only tested for Ubuntu

# Changelog:

**V 1.1.0**

- Removed hiredis, broker now publishes to weather station API
- Configurable API key through file
- Configurable URL through arguments
