import { WiDaySunny, WiRain } from "react-icons/wi"
import { HiMoon } from "react-icons/hi";

function getLightString(lightAnalog: number): string {
    if (lightAnalog > 2000) {
        return "SUNNY"
    }

    return "DARK"
}

export default function WeatherCard({ temp_f, humid, pressure_kpa, light_level, rain_level }: { temp_f: number, humid: number, pressure_kpa: number, light_level: number, rain_level: number }) {

    let lightString = getLightString(light_level)

    return (
        <div className="mt-3 mb-3 p-10">
            <div className="flex md:flex-row flex-col items-center">
                <WeatherIcon light_level={lightString} rain_level={rain_level} />
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
        </div>
    )
}

function WeatherIcon({ light_level, rain_level }: { light_level: string, rain_level: number }) {
    if (rain_level > 700)
    {
        return (
            <WiRain className="h-36 w-36 text-gray-500" />
        )
    }
    else if (light_level == "SUNNY") {
        return (
            <WiDaySunny className="h-36 w-36 text-yellow-300" />
        )
    } 
    else {
        return (
            <HiMoon className="h-32 w-32 text-gray-700" />
        )
    }
}