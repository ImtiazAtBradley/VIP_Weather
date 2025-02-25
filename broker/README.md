# Overview

The broker program is designed to go along with the weather station website, and the various devices. The broker queries a radio attached to a device on a serial port, and then parses & passes received messages on to a redis database, as indicated by the user. 

NOTE: This software will only work with POSIX compliant systems (linux/MacOs). Only tested for Ubuntu

# Requirements

- [ ] Receives and parses messages received through serial port via radio. (Abstract through port api)
- [ ] Publishes received data to redis database
- [ ] User can input redis server destination
- [ ] Logs failures to stdout/stderr
