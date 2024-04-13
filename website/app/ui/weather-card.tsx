import { WiDaySunny } from "react-icons/wi"

export default function WeatherCard() {
    return (
        <div className="mt-3 mb-3 p-10">
            <div className="flex flex-row items-center">
                <WiDaySunny className="h-36 w-36 text-yellow-300" />
                <div className="justify-evenly">
                    <span className="text-6xl font-semibold text-slate-600">25.6&deg;F</span>
                    <div className="flex flex-col text-slate-400">
                        <div className="flex flex-row justify-between">
                            <span>Humidity:</span>
                            <span>35%</span>
                        </div>
                        <div className="flex flex-row justify-between">
                            <span>Pressure:</span>
                            <span>35 kPa</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    )
}