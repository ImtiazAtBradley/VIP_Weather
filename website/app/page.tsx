import WeatherStationLogo from "./ui/weather-station-logo";
import { EnvironmentLineGraph } from "./ui/line-graph";

export default function Page() {
  return (
    <main className="flex flex-col p-6 h-screen">
      <div className="flex itmes-center h-28 rounded-lg bg-red-500 p-4">
        <WeatherStationLogo />
      </div>
      <div className="flex flex-col mt-3 p-4 h-full">
        <div className="flex h-full">
          <EnvironmentLineGraph />
        </div>
        <div className="flex h-full">
          <EnvironmentLineGraph />
        </div>
        <div className="flex h-full">
          <EnvironmentLineGraph />
        </div>
      </div>
    </main>
  );
}