import Redis from "ioredis";
import express from "express";
import bodyParser from "body-parser";
import fs from "fs";
import crypto from "crypto";

// DEFINITIONS =================================================================

const JSON_CONTENT_TYPE = "application/json"

// TYPES =======================================================================

type WeatherData = {
    timestamp_unix_ms: string,
    temp_c: string,
    humid_prcnt: string,
    pressure_kpa: string,
    gas_kohms: string,
    rain_an: string,
    light_an: string,
};

// LIB =========================================================================

async function getWeatherData(): Promise<WeatherData[]> {
    // TODO: Eventually, we may want to get data from stations with different IDs
    const items = await redis.xrevrange("weather-station:0", "+", "-", "COUNT", 336)

    let weatherData: Array<WeatherData> = []

    for (var i = 0; i < items.length; i++) {
	let tstamp = items[i][0]
	tstamp = tstamp.slice(0, tstamp.lastIndexOf('-'))
        weatherData[i] = {
            timestamp_unix_ms: tstamp,
            temp_c: items[i][1][1],
            humid_prcnt: items[i][1][3],
            pressure_kpa: items[i][1][5],
            gas_kohms: items[i][1][7], // TODO: Get this from data base somehow
            rain_an: items[i][1][9],
            light_an: items[i][1][11],
        }
    }

    return weatherData
}

// data -> JSON object containing the new weather station data.
async function postWeatherData(weatherRecord: WeatherData): Promise<boolean> {

    try {
        // Try to add stuff to redis database
        redis.xadd(
            "weather-station:0", "MAXLEN", "~", "720", "*",
            "temp_c", `${weatherRecord.temp_c}`,
            "humid_prcnt", `${weatherRecord.humid_prcnt}`,
            "pressure_kpa", `${weatherRecord.pressure_kpa}`,
            "gas_kohms", `${weatherRecord.gas_kohms}`,
            "rain_an", `${weatherRecord.rain_an}`,
            "light_an", `${weatherRecord.light_an}`
        )
    } catch (error) {
        return false;
    }

    return true;
}

function isJsonWeatherRecord(json : object) : boolean
{
    return json.hasOwnProperty('timestamp_unix_ms') 
        && json.hasOwnProperty('temp_c') 
        && json.hasOwnProperty('humid_prcnt') 
        && json.hasOwnProperty('pressure_kpa') 
        && json.hasOwnProperty('gas_kohms')
        && json.hasOwnProperty('rain_an') 
        && json.hasOwnProperty('light_an')
}

function loadKeys(path : string) : string[] | null
{
    try {
        let fileData = fs.readFileSync(path)
        let keys = fileData.toString().split("\n")
        if (keys.length == 0)
        {
            return null
        }
        return keys
    } catch (err) {
        console.log(err)
        return null
    }
}

// GLOBALS =====================================================================

const app = express()
const port = 27500
const keysPath : string = "./api.keys"

// API APP =====================================================================

const redis = new Redis({
    port: 6379,
    host: "localhost",
    retryStrategy(times) {
        const delay = Math.min(times * 50, 2000)
        return delay
    },
})

app.use(bodyParser.json())

redis.on('error', function (e) {
    console.log(`REDIS CLIENT ERROR: ${e}`)
})

app.get('/api/envdata', async (req, res) => {
    let data = { weatherData: await getWeatherData() }

    res.json(data)
})

app.post('/api/envdata', async (req, res) => {

    // Authenticate user
    let token = req.header("Authorization")
    if (token == undefined)
    {
        console.log("User tried accessing without Authorization header")
        res.status(401).send()
        return
    }

    token = crypto.createHash("sha256").update(token).digest("hex")
    let keyFound = false
    {
        loadKeys(keysPath).forEach((val, indx) => {
            if (val === token)
            {
                keyFound = true
            }
        })
    }

    if (!keyFound)
    {
        console.log("Unauthorized user tried to access API")
        res.status(401).send()
        return
    }

    console.log("user authenticated")

    // Process data
    console.log(`Got data: ${JSON.stringify(req.body)}`)
    
    let data = req.body
    let isWeatherRecord = isJsonWeatherRecord(data)
    let reason = "OK"

    if (isWeatherRecord)
    {
        postWeatherData(data);
        res.status(200);
    }
    else
    {
        res.status(400)
        reason = "Invalid JSON data"
    }

    res.json({reason : reason})
})

app.listen(port, () => {
    console.log(`Bradley Weather Station API listening on port ${port}`)
})
