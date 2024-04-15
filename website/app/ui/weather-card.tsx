import { WiDaySunny } from "react-icons/wi"

export default function WeatherCard({temp_c, humid, pressure_kpa} : {temp_c: number, humid: number, pressure_kpa: number}) {
    return (
        <div className="mt-3 mb-3 p-10">
            <div className="flex md:flex-row flex-col items-center">
                <WiDaySunny className="h-36 w-36 text-yellow-300" />
                <div className="justify-evenly">
                        <span className="text-6xl font-semibold text-slate-600">{Math.round((temp_c * 9/5) + 32)}&deg;F</span>
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
