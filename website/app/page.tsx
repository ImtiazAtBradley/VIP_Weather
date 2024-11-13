import WeatherStationLogo from "./ui/weather-station-logo";
import EnvironmentLineGraph from "./ui/line-graph";
import LoadFailedCard from "./ui/load-failed-card";
import WeatherCard from "./ui/weather-card";

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
  let timestamp : string[]= []
  let temp_f = []
  let pressure_kpa = []
  let humidity_prcnt = []
  let light_level = []
  for (let i = 0; i < data.weatherData.length; i++) {
    let element = data.weatherData[i]

    // Timestamp returned in <TIMESTAMP>-<SAMPLE> format - split into just timestamp (ms)
    let now = new Date(parseInt(element.timestamp.split("-")[0]))
    let hours = now.getHours();
    let suffix = hours > 12 ? "PM" : "AM"

    // Set timestamp string
    timestamp[i] = now.toString();

    // Convert to normal people time
    if (hours > 12) {
      hours = hours - 12;
    }

    let graphTimeStr = `${hours}:${now.getMinutes()} ${suffix}`

    time.push(graphTimeStr)
    temp_f.push(Math.round((element.temp_c * 9 / 5 + 32) * 10) / 10)
    pressure_kpa.push(element.pressure_kpa)
    humidity_prcnt.push(element.humid_prcnt)
    light_level.push(element.light_level)
  }

  time = time.reverse()
  temp_f = temp_f.reverse()
  pressure_kpa = pressure_kpa.reverse()
  humidity_prcnt = humidity_prcnt.reverse()
  light_level = light_level.reverse()

  return { times: time, timestamps: timestamp, temp_f: temp_f, pressure_kpa: pressure_kpa, humidity_prcnt: humidity_prcnt, light_level: light_level }
}

function Charts({ time, temperature, humidity, pressure, lightLevel }: { time: string[], temperature: number[], humidity: number[], pressure: number[], lightLevel: string[] }) {

  let lightLevelBinary: number[] = []
  for (let i = 0; i < lightLevel.length; i++) {
    lightLevelBinary.push(lightLevel[i] == "SUNNY" ? 1 : 0)
  }

  return (
    <div className="grid md:grid-cols-2 grid-cols-1 mt-3 p-4">
      <EnvironmentLineGraph xlabels={time} label="°F" title="Temperature" color="rgb(255, 0, 0)" backgroundColor="rgba(255, 0, 0, 0.5" d={temperature} />
      <EnvironmentLineGraph xlabels={time} label="% Relative" title="Humidity" color="rgb(0, 255, 0)" backgroundColor="rgba(0, 255, 0, 0.5)" d={humidity} />
      <EnvironmentLineGraph xlabels={time} label="kPa" title="Pressure" color="rgb(0, 0, 255)" backgroundColor="rgba(0, 0, 255, 0.5)" d={pressure} />
      <EnvironmentLineGraph xlabels={time} label="Binary Intensity" title="Light Intensity" color="rgb(0, 255, 255)" backgroundColor="rgba(0, 255, 255, 0.5)" d={lightLevelBinary} />
    </div>
  )
}

function TheBeccWeatherStation() {
  return (<>
    <div className="flex-col justify-center">
      <h1 className="mt-10 md:text-5xl text-3xl text-center">The BECC Weather Station</h1>
      <div className="py-5 px-10">
        <p>The BECC (Business and Engineering Convergence Center) weather station is a student-built environmental measurement device that reports its current and past data through an API, which this website is using. The weather station is located at Bradley University, in or near the BECC building on the north side of Bradley University's campus. For more information, see our <a href="/about" className="hover:text-red-500 text-red-700">about</a> section, and also be sure to visit our repository on <a className="hover:text-red-500 text-red-700" href="https://github.com/ImtiazAtBradley/VIP_Weather" target="_blank">GitHub</a>.</p>
      </div>
    </div>
  </>)
}

export default async function Page() {

  const weatherData = await getEnvironmentData()

  if (weatherData == null) {
    return (
      <div className="flex justify-center">
        <LoadFailedCard />
      </div>
    )
  }

  let currentTemp = weatherData.temp_f.slice(-1)[0]
  let currentPressure = weatherData.pressure_kpa.slice(-1)[0]
  let currentHumidity = weatherData.humidity_prcnt.slice(-1)[0]
  let currentLightLevel = weatherData.light_level.slice(-1)[0]
  let lastUpdated = weatherData.timestamps.slice(-1)[0]

  return (
    <>
      <div className="mt-5" role="alert">
        <div className="bg-orange-500 text-white font-bold rounded-t px-4 py-2">
          Info!
        </div>
        <div className="border border-t-0 border-orange-400 rounded-b bg-orange-100 px-4 py-3 text-orange-700">
          <p>Please note that the data you are seeing is INSIDE data, from the BECC 4261 Controls Lab, where the weather station is currently located. This website will post an announcement when outside data is now being shown!</p>
        </div>
      </div>
      <div className="flex justify-center">
        <WeatherCard temp_f={currentTemp} humid={currentHumidity} pressure_kpa={currentPressure} light_level={currentLightLevel} />
      </div>

      {TheBeccWeatherStation()}

      <div className="flex mt-5 justify-center md:text-5xl text-3xl">
        <h1 className="text-center">Historical Environment Data</h1>
      </div>
      <Charts time={weatherData.times} temperature={weatherData.temp_f} humidity={weatherData.humidity_prcnt} pressure={weatherData.pressure_kpa} lightLevel={weatherData.light_level} />
      <div className="px-10">
        <p className="text-sm opacity-75">This data was last updated at: {lastUpdated}</p>
        <br></br>
        <p>For more data, access our API, which you can find out more about in our <a href="/documentation" className="text-red-700 hover:text-red-500">documentation</a>.</p>
      </div>
    </>
  );
}
