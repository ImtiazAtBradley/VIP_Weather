export default function Page()
{
    return (
        <>
            <div className="p-5 px-10 text-xl">
                <h1 className="text-3xl pb-5">Server & Services Usage Disclaimer</h1>
                <p>The ECE 398 Weather Station project is a data collection project of meteorological and historical data. To enhance science, experimental products may be accessible on this server and care must be taken when using such products as they are intended for research use. This server is available 24 hours a day, seven days a week unless otherwise noted. Timely delivery of data and products from this server through the Internet is not guaranteed.</p>
                <br/>
                <p>Note the above applies for both the Weather Station web server and API</p>
            </div>
            <div className="p-5 px-10 text-xl">
                <h1 className="text-3xl pb-5">How You Can Use Our Data</h1>
                <p>The information on the Bradley Weather Station web page is in the public domain, unless specifically noted otherwise, and may be used without charge for any lawful purpose so long as you do not: 1. claim it is your own 2. use it in a manner that implies an endorsement or affiliation with Bradley University 3. modify its content and then present it as official Bradley University material. Also DO NOT present information of your own in a way that makes it appear to be official Bradley University Information.</p>
            </div>
            <div className="p-5 px-10 text-xl">
                <h1 className="text-3xl pb-5">Who Has Access to Weather Station Data?</h1>
                <p>The student lead on this project, Jacob Simeone, as well as the student advisor, Dr. Mohammad Imtiaz, have access to the data listed below, both of whose contact is listed on the <a className="hover:text-red-500 text-red-700" href="/contact">contact page</a></p>
            </div>
            <div className="p-5 px-10 text-xl">
                <h1 className="text-3xl pb-5">What Data is Collected?</h1>
                <ul className="list-disc px-5">
                    <li>Temperature immediately inside the box.</li>
                    <li>Humidity immediately inside the box.</li>
                    <li>Pressure immediately inside the box.</li>
                    <li>Light level immediately inside the box.</li>
                    <li>Whether or not there is water on top of the box, present on the sensor.</li>
                    <li>IP Adress (part of log on website, all servers, collect this information) & related information, like browser, hostname, ect. If the user provides it through the HTTP(S) protocols(s)</li>
                </ul>
            </div>
            <div className="p-5 px-10 text-xl">
                <h1 className="text-3xl pb-5">Data Disclaimer</h1>
                <p>The Weather Station does NOT collect any personally identifiable information about its surroundings or its users (we DO NOT have access to your information and with the information we do collect, as specified above, will be used specifically for research purposes and for delivering the weather around Bradley University to users), but only enough to fulfill its role to inform the users about the environment with real-time data, as well as log connections to this website and the accompanying API for administrative purposes.</p>
            </div>
        </>
    )
}