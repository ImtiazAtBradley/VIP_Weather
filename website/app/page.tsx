import WeatherStationLogo from "./ui/weather-station-logo";
import { RedHatDisplay } from "./ui/fonts";
import EnvironmentLineGraph from "./ui/line-graph";
import LoadFailedCard from "./ui/load-failed-card";
import WeatherCard from "./ui/weather-card";

async function getEnvironmentData() {

  let res = null;

  try {
    res = await fetch("http://localhost/api/envdata", { cache: "no-store" })
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
    temp_f.push(Math.round((element.temp_c * 9/5 + 32) * 10 ) / 10)
    pressure_kpa.push(element.pressure_kpa)
    humidity_prcnt.push(element.humid_prcnt)
  }

  time = time.reverse()
  temp_f = temp_f.reverse()
  pressure_kpa = pressure_kpa.reverse()
  humidity_prcnt = humidity_prcnt.reverse()

  return { times: time, temp_f: temp_f, pressure_kpa: pressure_kpa, humidity_prcnt: humidity_prcnt }
}

function Charts({ time, temperature, humidity, pressure }: { time: number[], temperature: { time: string, val: number }[], humidity: { time: string, val: number }[], pressure: { time: string, val: number }[] }) {
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
      <main className="flex flex-col md:p-6 p-2 h-screen">
        <div className="flex itmes-center md:h-28 h-20 rounded-lg bg-red-500 p-4">
          <WeatherStationLogo />
        </div>
        <div className="flex justify-center">
          <LoadFailedCard />
        </div>
      </main>
    )
  }

  let currentTemp = weatherData.temp_f.slice(-1)[0]
  let currentPressure = weatherData.pressure_kpa.slice(-1)[0]
  let currentHumidity = weatherData.humidity_prcnt.slice(-1)[0]

  return (
    <main className="flex flex-col md:p-6 p-2 h-screen">
      <div className="flex itmes-center md:h-28 h-20 rounded-lg bg-red-500 p-4">
        <WeatherStationLogo />
      </div>
      <div className="mt-5" role="alert">
	  <div className="bg-orange-500 text-white font-bold rounded-t px-4 py-2">
	    Info!
	  </div>
	  <div className="border border-t-0 border-orange-400 rounded-b bg-orange-100 px-4 py-3 text-orange-700">
	    <p>Weather Station is currently indoors, and is reading indoor data. Values updates happen every 5 seconds. Refresh page in order to see new data.</p>
	  </div>
	</div>
      <div className="flex justify-center">
        <WeatherCard temp_f={currentTemp} humid={currentHumidity} pressure_kpa={currentPressure} light_level="SUNNY" />
      </div>
      <div className={`${RedHatDisplay.className} flex mt-5 justify-center md:text-5xl text-3xl`}>
        <h1 className="text-center">Historical Environment Data</h1>
      </div>
      <Charts time={weatherData.times} temperature={weatherData.temp_f} humidity={weatherData.humidity_prcnt} pressure={weatherData.pressure_kpa} />
    </main>
  );
}
