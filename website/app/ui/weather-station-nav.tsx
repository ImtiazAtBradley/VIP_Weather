import { TiWeatherWindyCloudy } from "react-icons/ti";
import NavLink from "./nav-link"

export default function WeatherStationNav() {
    return (
        <div className="flex flex-col p-3 w-full items-start justify-between text-white rounded-lg bg-red-500 md:items-center ">
            <a href="/">
                <div className="flex">
                    <WeatherStationIcon />
                    <p className="text-xl m-3 hover:underline md:text-4xl ">BECC Weather Station</p>
                </div>
            </a>
            <div className="flex md:px-10 flex-col md:space-x-5 md:flex-row ">
                <NavLink link="/news" text="News" />
                <NavLink link="/about" text="About" />
                <NavLink link="https://litemage.github.io/vip-weather-docs/" text="Documentation" blank={true} />
                <NavLink link="https://github.com/ImtiazAtBradley/VIP_Weather" text="GitHub" blank={true} />
                <NavLink link="/data-collection" text="Data Usage Policy" />
                <NavLink link="/contact" text="Contact" />
            </div>
        </div>
    );
}

function WeatherStationIcon() {
    return (
        <TiWeatherWindyCloudy className="md:h-16 md:w-16 h-10 w-10" />
    )
}