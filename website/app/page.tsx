import WeatherStationLogo from "./ui/weather-station-logo";
import { EnvironmentLineGraph } from "./ui/line-graph";

export default function Page() {
  return (
    <main className="flex flex-col p-6 h-screen">
      <div className="flex itmes-center md:h-28 h-20 rounded-lg bg-red-500 p-4">
        <WeatherStationLogo />
      </div>
      <div className="flex flex-col mt-3 p-4 md:h-full">
        <div className="flex md:h-full h-60">
          <EnvironmentLineGraph />
        </div>
        <div className="flex md:h-full h-60">
          <EnvironmentLineGraph />
        </div>
        <div className="flex md:h-full h-60">
          <EnvironmentLineGraph />
        </div>
      </div>
    </main>
  );
}