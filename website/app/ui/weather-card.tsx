import { WiDayRain, WiDaySunny, WiNightAltRain, WiNightRain } from "react-icons/wi"
import { HiMoon } from "react-icons/hi";

export default function WeatherCard({ temp_f, humid, pressure_kpa, light_level, is_raining }: { temp_f: number, humid: number, pressure_kpa: number, light_level: string, is_raining: boolean }) {

    let displaySunny = light_level == "SUNNY";

    return (
        <div className="mt-3 mb-3 p-10">
            <div className="flex flex-col">
                <div className="flex md:flex-row flex-col items-center">
                    <WeatherIcon light_level={light_level} is_raining={is_raining}/>
                    <div className="justify-evenly">
                        <span className="text-6xl font-semibold text-slate-600">{temp_f}&deg;F</span>
                        <div className="flex flex-col text-slate-400 text-sm">
                            <div className="flex flex-row justify-between">
                                <span>Humidity:</span>
                                <span>{humid}%</span>
                            </div>
                            <div className="flex flex-row justify-between">
                                <span>Pressure:</span>
                                <span>{pressure_kpa}kPa</span>
                            </div>
                        </div>
                    </div>
                </div>
                <div className="flex">
                    <span className="font-semibold text-slate-500">
                        Weather station is currently reporting {light_level.toLowerCase()} and {is_raining == true ? "raining" : "clear"}.
                    </span>
                </div>
            </div>
        </div>
    )
}

function WeatherIcon({ light_level, is_raining }: { light_level: string, is_raining: boolean}) {
    if (is_raining)
    {
        if (light_level == "SUNNY")
        {
            return (
                <WiDayRain className="h-36 w-36 text-gray-500" />
            )
        }
        else
        {
            return (
                <WiNightAltRain className="h-32 w-32 text-gray-700" />
            )
        }
    }
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