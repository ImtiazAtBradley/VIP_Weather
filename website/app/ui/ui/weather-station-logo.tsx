import { TiWeatherWindyCloudy } from "react-icons/ti";

export default function WeatherStationLogo() {
    return (
        <div className="flex w-full px-4 items-center justify-between text-white">
            <a href="/">
                <div className="flex">
                    <WeatherStationIcon />
                    <p className="md:text-4xl text-xl m-3">BECC Weather Station</p>
                </div>
            </a>
        </div>
    );
}

function WeatherStationIcon() {
    return (
        <TiWeatherWindyCloudy className="md:h-16 md:w-16 h-10 w-10" />
    )
}