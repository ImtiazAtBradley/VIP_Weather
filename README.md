# VIP Weather - An Overview

A weather station project to inform students on Bradley University campus of current weather conditions. Students shall be able to interract with the station in a way that should inform them of:

- The current temperature, in both celsius & farenheit
- The current atmostpheric pressure
- If it's raining
- If it's sunny or gloomy
- What campus looks like (in order to determine snow or flooding hazards)

# Connecting To The Weather Station

For now, anyone can connect to the weather station web server hosted directly on the ESP32 microcontroller by connecting to the WiFi network hosted by that microcontroller with the following information...

- SSID: `BECC Weather Station`
- Password: `weathernow`

After connecting, the user will have to go to the url `beccweather.com` in order to access the weather web page. 

**NOTE:** This behavior will surely change in the future
