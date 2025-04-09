import WeatherStationLogo from "./ui/weather-station-nav";
import EnvironmentLineGraph from "./ui/line-graph";
import LoadFailedCard from "./ui/load-failed-card";
import WeatherCard from "./ui/weather-card";
import { DateTime } from "luxon";
import { existsSync } from "fs"
import MaintenanceCard from "./ui/maintenance-card";
import { GetNWSData } from "./lib/fetch-nws";

function checkMaitnenceFile(): boolean {
  // Maintenance file will have the following path:
  // Don't care about the content, just if it exists
  let maintenancePath = "./under-maintenance.txt"
  return existsSync(maintenancePath)
}

async function getEnvironmentData() {

  let res = null;

  try {
    res = await fetch("http://weather.jacobsimeone.net/api/envdata", { cache: "no-store" })
  } catch (error) {
    return null
  }

  if (!res.ok || res == null) {
    return null
  }

  const data = await res.json()

  // Return an array with relative times + temp/pres/humid data

  let time = []
  let temp_f = []
  let pressure_kpa = []
  let humidity_prcnt = []
  let gas_kohms = []
  let light_an = []
  let rain_an = []
  for (let i = 0; i < data.weatherData.length; i++) {
    let element = data.weatherData[i]



    // Timestamp returned in <TIMESTAMP>-<SAMPLE> format - split into just timestamp (ms)
    let nowUtc = DateTime.fromMillis(parseInt(element.timestamp_unix_ms))
    let nowLocal = nowUtc.setZone("America/Chicago")

    time.push(nowLocal.toISO())
    temp_f.push(Math.round((element.temp_c * 9 / 5 + 32) * 10) / 10)
    pressure_kpa.push(element.pressure_kpa)
    humidity_prcnt.push(element.humid_prcnt)
    gas_kohms.push(element.gas_kohms)
    light_an.push(element.light_an)
    rain_an.push(element.rain_an)
  }

  time = time.reverse()
  temp_f = temp_f.reverse()
  pressure_kpa = pressure_kpa.reverse()
  humidity_prcnt = humidity_prcnt.reverse()
  gas_kohms = gas_kohms.reverse()
  light_an = light_an.reverse()
  rain_an = rain_an.reverse()

  return { times: time, temp_f: temp_f, pressure_kpa: pressure_kpa, humidity_prcnt: humidity_prcnt, gas_an: gas_kohms, light_an: light_an, rain_an: rain_an }
}

function Charts({ time, temperature, humidity, pressure, gas, lightLevel, rain }: { time: (string | null)[], temperature: number[], humidity: number[], pressure: number[], gas: number[], lightLevel: number[], rain: number[] }) {

  return (
    <div className="grid grid-cols-1 mt-3 p-4">
      <EnvironmentLineGraph xlabels={time} label="°F" title="Temperature" color="rgb(255, 0, 0)" backgroundColor="rgba(255, 0, 0, 0.5" d={temperature} />
      <EnvironmentLineGraph xlabels={time} label="% Relative" title="Humidity" color="rgb(0, 255, 0)" backgroundColor="rgba(0, 255, 0, 0.5)" d={humidity} />
      <EnvironmentLineGraph xlabels={time} label="kPa" title="Pressure" color="rgb(0, 0, 255)" backgroundColor="rgba(0, 0, 255, 0.5)" d={pressure} />
      <EnvironmentLineGraph xlabels={time} label="kOhms" title="Gas Sensor (kOhms)" color="rgb(150, 80, 240)" backgroundColor="rgba(150, 80, 240, 0.5)" d={gas} />
      <EnvironmentLineGraph xlabels={time} label="Analog" title="Light Intensity" color="rgb(0, 255, 255)" backgroundColor="rgba(0, 255, 255, 0.5)" d={lightLevel} />
      <EnvironmentLineGraph xlabels={time} label="Analog" title="Rain Sensor" color="rgb(250, 185, 80)" backgroundColor="rgba(250, 185, 80, 0.5)" d={rain} />
    </div>
  )
}

function TheBeccWeatherStation() {
  return (<>
    <div className="flex-col justify-center">
      <h1 className="mt-10 md:text-5xl text-3xl text-center">The BECC Weather Station</h1>
      <div className="py-5 px-10 text-xl">
        <p>The BECC (Business and Engineering Convergence Center) weather station is a student-built environmental measurement device that reports its current and past data through an API, which this website is using. The weather station is located at Bradley University, in or near the BECC building on the north side of Bradley University's campus. For more information, see our <a href="/about" className="hover:text-red-500 text-red-700">about</a> section, and also be sure to visit our repository on <a className="hover:text-red-500 text-red-700" href="https://github.com/ImtiazAtBradley/VIP_Weather" target="_blank">GitHub</a>. This station is extremely new, and data may be inaccurate until we get it tuned up, we appreciate your patience!  <br /><br /> <strong>For any questions, comments, or concerns please <a className="hover:text-red-500 text-red-700" href="/contact">Contact Us</a>.</strong></p>
      </div>
    </div>
  </>)
}

export default async function Page() {

  let nwsData = await GetNWSData();

  // Check if the maintenance file is present, if so, show under maintenance screen
  if (checkMaitnenceFile()) {
    return (
      <div className="flex justify-center">
        <MaintenanceCard nwsData={nwsData}/>
      </div>
    )
  }

  const weatherData = await getEnvironmentData()

  if (weatherData == null || weatherData.times.length <= 0) {
    return (
      <div className="flex justify-center">
        <LoadFailedCard />
      </div>
    )
  }

  let currentTemp = weatherData.temp_f.slice(-1)[0]
  let currentPressure = weatherData.pressure_kpa.slice(-1)[0]
  let currentHumidity = weatherData.humidity_prcnt.slice(-1)[0]
  let currentLightLevel = weatherData.light_an.slice(-1)[0]
  let lastUpdated = weatherData.times.slice(-1)[0]

  return (
    <>
      <div className="mt-5" role="alert">
        <div className="bg-orange-500 text-white font-bold rounded-t px-4 py-2">
          Weather Station Data Reliablility
        </div>
        <div className="border border-t-0 border-orange-400 rounded-b bg-orange-100 px-4 py-3 text-orange-700">
          <p>The BECC Weather Station is deployed in a sub-optimal location and is reporting temperatures hotter than ambient in the presence of high solar intensity.</p>
        </div>
      </div>
      <div className="mt-5" role="alert">
        <div className="bg-green-500 text-white font-bold rounded-t px-4 py-2">
          News!
        </div>
        <div className="border border-t-0 border-green-400 rounded-b bg-green-100 px-4 py-3 text-green-700">
          <p>We've made some improvments around the site, including a <a className="hover:underline text-red-700" href="/news">news</a> page where we can more effectively communicate with users!</p>
        </div>
      </div>
      <div className="flex justify-center">
        <WeatherCard temp_f={currentTemp} humid={currentHumidity} pressure_kpa={currentPressure} light_level={currentLightLevel} />
      </div>

      {TheBeccWeatherStation()}

      <div className="flex mt-5 justify-center md:text-5xl text-3xl">
        <h1 className="text-center">Historical Environment Data</h1>
      </div>
      <Charts time={weatherData.times} temperature={weatherData.temp_f} humidity={weatherData.humidity_prcnt} pressure={weatherData.pressure_kpa} gas={weatherData.gas_an} lightLevel={weatherData.light_an} rain={weatherData.rain_an} />
      <div className="px-10">
        <p className="text-sm opacity-75">This data was last updated at: {lastUpdated}</p>
        <br></br>
        <p>For more data, access our API, which you can find out more about in our <a href="https://litemage.github.io/vip-weather-docs/" className="text-red-700 hover:text-red-500">documentation</a>.</p>
      </div>
    </>
  );
}
