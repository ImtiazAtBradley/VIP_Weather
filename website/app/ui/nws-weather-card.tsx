import { WiDaySunny } from "react-icons/wi"
import { HiMoon } from "react-icons/hi";
import {NWSData} from "@/app/lib/fetch-nws"

export default function NWSWeatherCard({nwsData} : {nwsData: NWSData}) {

    return (
        <div className="mt-3 mb-3 p-10">
            <div className="flex md:flex-row flex-col items-center">
                <WeatherIcon light_level="SUNNY" />
                <div className="justify-evenly">
                    <span className="text-6xl font-semibold text-slate-600">{nwsData.temp}&deg;F</span>
                    <div className="flex flex-col text-slate-400 text-sm">
                        <div className="flex flex-row justify-between">
                            <span>Humidity:</span>
                            <span>{nwsData.humid}%</span>
                        </div>
                        <div className="flex flex-row justify-between">
                            <span>Pressure:</span>
                            <span>{nwsData.pres}kPa</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    )
}

function WeatherIcon({ light_level }: { light_level: string }) {
    if (light_level == "SUNNY") {
        return (
            <WiDaySunny className="h-36 w-36 text-yellow-300" />
        )
    } else {
        return (
            <HiMoon className="h-32 w-32 text-gray-700" />
        )
    }
}