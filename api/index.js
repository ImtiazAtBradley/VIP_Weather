const Redis = require("ioredis")
const express = require("express")

const redis = new Redis({
	port: 6379,
	host: "localhost",
	retryStrategy(times){
		const delay = Math.min(times * 50, 2000)
		return delay
	},
})
const app = express()
const port = 5000

redis.on('error', function (e) {
	console.log(`REDIS CLIENT ERROR: ${e}`)
})

async function getWeatherData(){
	// TODO: Eventually, we may want to get data from stations with different IDs
	const items = await redis.xrevrange("weather-station:0", "+", "-", "COUNT", 50)

	let obj = {
		weatherData: []
	}

	for (var i = 0; i < items.length; i++){
		obj.weatherData[i] = {
			timestamp: items[i][0],
			temp_c: items[i][1][1],
			humid_prcnt: items[i][1][3],
			pressure_kpa: items[i][1][5],
			is_raining: items[i][1][7],
			light_level: items[i][1][9],
		}
	}

	return obj
}

app.get('/api/envdata', async (req, res) => {
	let data = await getWeatherData()

	res.json(data)
})

app.listen(port, () => {
	console.log(`Example app is listening on port ${port}`)
})
