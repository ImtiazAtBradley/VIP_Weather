import { TiWeatherWindyCloudy } from "react-icons/ti";
import { RedHatDisplay } from '@/app/ui/fonts';

export default function WeatherStationLogo() {
    return (
        <div className={`${RedHatDisplay.className} flex flex-row items-center text-white`}>
            <WeatherStationIcon />
            <p className="md:text-4xl text-xl m-3">BECC Weather Station</p>
        </div>
    );
}

function WeatherStationIcon() {
    return (
        <TiWeatherWindyCloudy className="md:h-16 md:w-16 h-10 w-10" />
    )
}