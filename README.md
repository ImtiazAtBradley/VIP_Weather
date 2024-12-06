# Bradley Weather Station - Overview

The Bradley Weather Station was designed and realized by students enrolled in Vertically Integrated Projects in the Electrical Engineering department. This project aims to provide students on Bradley's campus a way to monitor and log weather data. In the future, the weather station system will become a distributed network of different stations, indoors and out that will report their data to a central data broker. 

# Getting Started

The weather station comes in a few different parts:

- Hardware
- Broker
- API
- Webserver

The hardware must be set up with the proper sensors, and be able to be flashed with the firmware needed to communicate with the rest of the system. 

## Hardware

**Parts Needed:**

| Part | Notes |
| --- | --- |
| ESP32-Devkit-M1 | other developmetn boards may work fine, but care would need to be taken to change pins to proper outputs |
| BME280 | Temperature, Humidity, Pressure sensor. Communicates with the microcontroller over i2c |
| RYLR896 | LoRa radio used for communication with broker. This particular module uses AT commands to be configured, and how it sends data. Search on the internet for AT commands and datasheet |
| Generic Photo Resistor | Any photo-resistor would work like [this](https://www.amazon.com/HiLetgo-Dependent-Photoresistor-Photoconductive-Resistance/dp/B00N1ZJUN4/), but some tweaking may be needed for light level indications. A 10K resistor to ground is used to create a voltage divider between the photoresistor and the pin on the micro measuring the output voltage. |
| Arduino Water Level Sensor | Arduino kits usually ship with analog water level sensors like [this](https://www.amazon.com/DIYables-Detector-Arduino-ESP8266-Raspberry/dp/B0BXKMLB4D/) one, which is what we're using to detect if it's raining or not. Note that this does NOT measure quantity of rain |

**Wiring Table:**

| ESP32 Pin (GPIO) | Device Pin | Notes |
| --- | --- | --- |
| 32 | Light Level Analog Output | Analog light level circuit input. Don't forget the divider!
| 33 | Water Level Analog Output |
| 13 | RYLR896 Reset Pin | Active Low
| 10 | RYLR896 Rx Pin |
| 9 | RYLR896 Tx Pin |
| 21 | BME280 SDA | 
| 22 | BME280 SCL | 

It's optional, but in the first prototype, we also broke out the TX and RX pins on the microcontroller to a connector on the housing. 

**Broker Device**

The computer running the broker needs to have one of the RYLR896 devices connected to it, and the device file accessible to the host running the broker program. 

## Software - Broker

The broker software is responsible for processing the serial data received from the attached radio. The broker program then writes that data to a redis database running locally on the the same machine.

**Requirements:** 

| Requirement | Value | Notes |
| --- | --- | --- |
| Operating System | Ubuntu officially supported | Other POSIX compliant OSs using the Linux kernel could work, but some finagling would be needed |
| Serial Ports | 1 | Need one serial port for communicating with the radio
| Database | System must be running an unsecured, local, redis database which can be accessed at port 6379 |
| HIRedis Library | The system must have the HIRedis library installed and available to running applications

**Running the Broker**

The broker system must have the HIRedis library installed

```bash
# Navigate into a directory you would like to install hiredis to
git clone https://github.com/redis/hiredis.git

mkdir build

cmake -B ./build/

cmake --build ./build/

cd build

make 

make install
```

Building the broker:

```bash
# Navigate to this repository's home directory first
cd broker/broker/

mkdir build

cmake -B ./build/

cmake --build ./build/
```

Running the broker:

```bash
# Replace /dev/XXX with the device file which the radio is attached to
./build/bradley-cast-broker /dev/XXX
```

## Software - API

The API is built with Node.js and is responsible for being the primary data source for any other application that requires data from the weather station(s).

**Requirements:**
- This server must be running on the same system as the redis database, and follows the same access requirements as the broker for the database.
- This server inherits all the other requirements as the broker, because this server must be running on the same system as the broker/redis db

**Running the API**

The system running this API must have npm installed in order to run the server, and the ability to download packages needed by the tool set

NOTE: The redis server must be running before this

```bash
npm install

node index.js
```

## Software - Website

The website does not have to be installed on the same system as the other two pieces of software, and can be installed on another machine, as long as the website and it's clients have access to the webserver running the API, as this is where the data for the website comes from. 

**Putting the website in maintenance mode**

The website has a feature called "maintenance mode" which gives developers an easy way to put the website into a mode that informs users the website is currently down. In order to do this, navigate to the root directory of the website (should be "`website/`" unless you have changed something) and create two files:

- `under-maintenance.txt` -> Create this file in the root website directory to put the website into maintance mode. You do not have to put any content into this file, just the presence is enough.
- `maintenance.log` -> Create this file in the same directory as `under-maintenance.txt`. This file is where maintenance log information that is newline seperated. The contents of this file will be shown to users as a log of past maintenance. The expected format of this file is as shown below:

```
[2024-12-02T10:30:00-06:00] Your description here...
[2024-11-21T14:00:00-06:00] Your description here...
```
> Note that timestamps are in ISO8601 format

**Requirements**
- Can run npm and required packages
- Can access the server running the API software

**Set-Up**

```bash
# Navigate to /website directory
npm install

# !!!!!!!!!!!!!!!!!!!
# DO NOT USE THIS IN PRODUCTION, THIS IS AN EXAMPLE, DOCS WILL NEED UPDATED
# !!!!!!!!!!!!!!!!!!!
npm run dev 
```

**Building the webpage**

Another option is to build the website and run it as recommended in production environments:

```bash
npm install

# Builds the webpage
npm run build

npm run start
``` 

# Protocol

Note that the protocol used to communicate over the radio is NOT the binary protocol described in the wiki, but is a simple ASCII string. 

String: `T00.00|H00.00|P00.00|R<0 or 1>|L<0,1,2>`

There is more data transmitted, but this is what the payload looks like.

> Next semester, we'll use the binary protocol that we developed and described in the GitHub repository.

# Using the Broker

Reference the help of the broker program in order to see usage, but here's a quick overview:

**To start the broker:**

```bash
bradley-cast-broker /dev/ttyS0 # Where ttyS0 is the serial port you're using
```

**To reset data collected by weather station:**

```bash
bradley-cast-broker reset-ws 0 # Where 0 is the ID of the weather station, which it will be for now
```

# Bradley-Cast Protocol

As a group, we developed this binary protocol to encode and parse messages to go to and from weather stations. The radios we are currently using only support transmitting ASCII data. In order to encode our binary protocol, we would have had to add extra code to encode to Base64. Next semester, we'll be switching to our binary protocol for communication. 

For now, the protocol used is a very simple ascii text string in the following format:

`T00.00|H00.00|P00.00|R<0 or 1>|L<0,1,2>`

**Where (Replace '00.00' with values and assume '|' is the delimeter):**

- `T` is temperature in celsius
- `H` is the humidity, in percent
- `P` is pressure, in kPa
- `R` is If it's raining or not
- `L` is light level: 

# Nginx Concigurations

Nginx is expected to be ran in front of any of the servers which could be accessed publicly the following are some configuration files that should be placed in `/etc/nginx/sites-enabled` (or your directory for configurations)

**For the front-end webserver:**
```
# Typically in /etc/nginx/sites-enabled/ directory
# Bradley weather station webserver configuration
# Repo: https://github.com/ImtiazAtBradley/VIP_Weather
server {
  listen 80;
  listen [::]:80;

  location / {
    proxy_pass http://localhost:27500;
  }
  
  location /api/envdata {
    # NOTE: This IP and port could change depending on your implementation. 
    # Point this URL to wherever the api is accessible
    proxy_pass http://192.168.16.101:27500/api/envdata;
  }
}
```

**For the api:**
```
# Typically in /etc/nginx/sites-enabled/ directory
# Bradley weather station api configuration
# Repo: https://github.com/ImtiazAtBradley/VIP_Weather
server {
  listen 80;
  listen [::]:80;

  location /api/envdata {
    proxy_pass http://localhost:27500/api/envdata;
  }
}
```

# Contributors

**Spring 2024**
- Jacob S. 
- Kyle K.
- Christopher H.
- Emily A.
- Owen J.
- Samantha P.
- Dr. Imtiaz - **Advisor**
