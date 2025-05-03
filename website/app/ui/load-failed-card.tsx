export default function LoadFailedCard() {
    return (
        <div className="mt-3 mb-3 p-10">
            <div className="flex flex-col items-center">
                <span className="flex text-6xl font-semibold text-slate-600">Failed to load weather data :(</span>
                <span className="flex mt-5">This error occurs when this server cannot reach the api which serves weather station data, or there is currently no data available.</span>
                <span className="flex font-bold"> If this problem persists longer than an hour or two, contact administrators at: jsimeone@mail.bradley.edu </span>
            </div>
        </div>
    )
}
