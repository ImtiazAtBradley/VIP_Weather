import { TiWeatherWindyCloudy } from "react-icons/ti";
import { RedHatDisplay } from '@/app/ui/fonts';

export default function WeatherStationLogo() {
    return (
        <div
            className={`${RedHatDisplay.className} flex flex-row items-center text-white`}
        >
            <WeatherStationIcon />
            <p className="text-[44px] m-3">BECC Weather Station</p>
        </div>
    );
}

function WeatherStationIcon() {
    return (
        <TiWeatherWindyCloudy className="h-16 w-16" />
    )
}