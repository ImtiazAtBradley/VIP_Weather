export type NWSData = {
    temp : string,
    pres : string,
    humid: string,
    message: string
}

async function GetNWSData() : Promise<NWSData | null> {
    let res = null
    const headers = {
        "User-Agent" : "weather.jacobsimeone.net, jsimeone0105@gmail.com"
    }

    // Attempt to fetch data from NWS
    try {
        res = await fetch("https://api.weather.gov/stations/KPIA/observations/latest", { headers });
    } catch (error) {
        return null;
    }

    if (!res.ok || res == null)
    {
        return null
    }

    const data = await res.json()

    let nwsData: NWSData = {
        temp : (data.properties.temperature.value * 9/5 + 32).toFixed(2), // degrees F
        pres : (data.properties.barometricPressure.value / 1000).toFixed(2), // pascals
        humid : (data.properties.relativeHumidity.value).toFixed(2), // percent
        message : data.properties.textDescription
    }

    return nwsData
}

export {
    GetNWSData
}