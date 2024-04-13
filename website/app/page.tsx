import WeatherStationLogo from "./ui/weather-station-logo";
import { RedHatDisplay } from "./ui/fonts";
import EnvironmentLineGraph from "./ui/line-graph";
import WeatherCard from "./ui/weather-card";

export const temperature = [
  { "time": "1", "val": 34.1 },
  { "time": "2", "val": 36.4 },
  { "time": "3", "val": 39.1 },
  { "time": "4", "val": 41.2 },
  { "time": "5", "val": 45.9 },
  { "time": "6", "val": 50.5 },
  { "time": "7", "val": 53.7 },
  { "time": "8", "val": 60.7 },
  { "time": "9", "val": 56.9 },
  { "time": "10", "val": 45.3 },
]

export const pressure = [
  { "time": "1", "val": 99.1 },
  { "time": "2", "val": 98.4 },
  { "time": "3", "val": 96.1 },
  { "time": "4", "val": 99.2 },
  { "time": "5", "val": 98.9 },
  { "time": "6", "val": 99.5 },
  { "time": "7", "val": 99.7 },
  { "time": "8", "val": 97.7 },
  { "time": "9", "val": 98.9 },
  { "time": "10", "val": 99.3 },
]

export const humidity = [
  { "time": "1", "val": 55.1 },
  { "time": "2", "val": 43.4 },
  { "time": "3", "val": 56.1 },
  { "time": "4", "val": 43.2 },
  { "time": "5", "val": 45.9 },
  { "time": "6", "val": 60.5 },
  { "time": "7", "val": 65.7 },
  { "time": "8", "val": 66.7 },
  { "time": "9", "val": 50.9 },
  { "time": "10", "val": 55.3 },
]

export default function Page() {
  return (
    <main className="flex flex-col p-6 h-screen">
      <div className="flex itmes-center md:h-28 h-20 rounded-lg bg-red-500 p-4">
        <WeatherStationLogo />
      </div>
      <div className="flex justify-center">
        <WeatherCard />
      </div>
      <div className={`${RedHatDisplay.className} flex mt-5 justify-center text-5xl`}>
        <h1>Current Environment Data</h1>
      </div>
      <div className="grid grid-cols-2 mt-3 p-4">
        <EnvironmentLineGraph label="Degrees C" title="Temperature" color="rgb(255, 0, 0)" backgroundColor="rgba(255, 0, 0, 0.5" d={temperature} />
        <EnvironmentLineGraph label="Percent" title="Humidity" color="rgb(0, 255, 0)" backgroundColor="rgba(0, 255, 0, 0.5)" d={humidity} />
        <EnvironmentLineGraph label="kPa" title="Pressure" color="rgb(0, 0, 255)" backgroundColor="rgba(0, 0, 255, 0.5)" d={pressure} />
      </div>
    </main>
  );
}