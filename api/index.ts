import Redis from "ioredis";
import express from "express";

// DEFINITIONS =================================================================

const JSON_CONTENT_TYPE = "application/json"

// TYPES =======================================================================

type WeatherData = {
    timestamp: string,
    temp_c: string,
    humid_prcnt: string,
    pressure_kpa: string,
    is_raining: string,
    light_level: string,
};

// LIB =========================================================================

async function getWeatherData(): Promise<WeatherData[]> {
    // TODO: Eventually, we may want to get data from stations with different IDs
    const items = await redis.xrevrange("weather-station:0", "+", "-", "COUNT", 100)

    let weatherData: Array<WeatherData> = []

    for (var i = 0; i < items.length; i++) {
        weatherData[i] = {
            timestamp: items[i][0],
            temp_c: items[i][1][1],
            humid_prcnt: items[i][1][3],
            pressure_kpa: items[i][1][5],
            is_raining: items[i][1][7],
            light_level: items[i][1][9],
        }
    }

    return weatherData
}

// data -> JSON object containing the new weather station data.
async function postWeatherData(weatherRecord: WeatherData): Promise<boolean> {

    try {
        // Try to add stuff to redis database
        redis.xadd(
            "weather-station:0", "MAXLEN", "~", "720",
            "temp_c", `${weatherRecord.temp_c}`,
            "humid_prcnt", `${weatherRecord.humid_prcnt}`,
            "pressure_kpa", `${weatherRecord.pressure_kpa}`,
            "is_raining", `${weatherRecord.is_raining}`,
            "light_level", `${weatherRecord.light_level}`
        )
    } catch (error) {
        return false;
    }

    return true;
}

// API APP =====================================================================

const redis = new Redis({
    port: 6379,
    host: "localhost",
    retryStrategy(times) {
        const delay = Math.min(times * 50, 2000)
        return delay
    },
})
const app = express()
const port = 27500

redis.on('error', function (e) {
    console.log(`REDIS CLIENT ERROR: ${e}`)
})

app.get('/api/envdata', async (req, res) => {
    let data = { weatherData: await getWeatherData() }

    res.json(data)
})

app.post('/api/envdata', async (req, res) => {
    if (req.headers["content-type"] != JSON_CONTENT_TYPE)
    {
        res.json({"reason" : "bad content-type. API only accepts \"application/json\""})
        res.status(400)
    }

    res.status(403);
})

app.listen(port, () => {
    console.log(`Bradley Weather Station API listening on port ${port}`)
})
