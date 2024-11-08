import WeatherStationLogo from "./ui/weather-station-logo";
import EnvironmentLineGraph from "./ui/line-graph";
import LoadFailedCard from "./ui/load-failed-card";
import WeatherCard from "./ui/weather-card";

async function getEnvironmentData() {

  let res = null;

  try {
    res = await fetch("http://weather.jacobsimeone.net:27500/api/envdata", { cache: "no-store" })
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
  for (let i = 0; i < data.weatherData.length; i++) {
    let element = data.weatherData[i]

    time.push(i + 1)
    temp_f.push(Math.round((element.temp_c * 9 / 5 + 32) * 10) / 10)
    pressure_kpa.push(element.pressure_kpa)
    humidity_prcnt.push(element.humid_prcnt)
  }

  time = time.reverse()
  temp_f = temp_f.reverse()
  pressure_kpa = pressure_kpa.reverse()
  humidity_prcnt = humidity_prcnt.reverse()

  return { times: time, temp_f: temp_f, pressure_kpa: pressure_kpa, humidity_prcnt: humidity_prcnt }
}

function Charts({ time, temperature, humidity, pressure }: { time: number[], temperature: number[], humidity: number[], pressure: number[] }) {
  return (
    <div className="grid md:grid-cols-2 grid-cols-1 mt-3 p-4">
      <EnvironmentLineGraph xlabels={time} label="Degrees F" title="Temperature" color="rgb(255, 0, 0)" backgroundColor="rgba(255, 0, 0, 0.5" d={temperature} />
      <EnvironmentLineGraph xlabels={time} label="Percent" title="Humidity" color="rgb(0, 255, 0)" backgroundColor="rgba(0, 255, 0, 0.5)" d={humidity} />
      <EnvironmentLineGraph xlabels={time} label="kPa" title="Pressure" color="rgb(0, 0, 255)" backgroundColor="rgba(0, 0, 255, 0.5)" d={pressure} />
    </div>
  )
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
        <WeatherCard temp_f={currentTemp} humid={currentHumidity} pressure_kpa={currentPressure} light_level="SUNNY" />
      </div>
      <div className="flex mt-5 justify-center md:text-5xl text-3xl">
        <h1 className="text-center">Historical Environment Data</h1>
      </div>
      <Charts time={weatherData.times} temperature={weatherData.temp_f} humidity={weatherData.humidity_prcnt} pressure={weatherData.pressure_kpa} />
    </>
  );
}
