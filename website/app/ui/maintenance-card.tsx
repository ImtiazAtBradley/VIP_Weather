import { FaScrewdriverWrench } from "react-icons/fa6";
import { readFileSync, existsSync } from "fs";

const maintenanceLogPath = "./maintenance.log"

function getMaitnenceLog() : string[]
{
    let logs : string[] = []
    let str : string = "";
    try {
        if (existsSync(maintenanceLogPath))
        {
            str = readFileSync(maintenanceLogPath, 'ascii')
            str.replaceAll("\r", "")
            logs = str.split("\n")
        }
    } catch (error) {
        console.log("Failed to read from maintenance log file")
    }
    
    return logs
}

function MaintenanceLogCard() {
    let logEntries : any[] = []
    let rawLogs : string[] = getMaitnenceLog();

    rawLogs.forEach(element => {
        logEntries.push(<li>{element}</li>)
    });

    return (
        <div>
            <ul className="list-disc font-mono ">
                {logEntries}
            </ul>
        </div>
    )
}

export default function MaintenanceCard() {
    return (
    <div className="mt-3 mb-3 p-10 xl:w-1/2">
        <div className="flex flex-col items-center">
            <span className="flex text-6xl font-semibold text-slate-600">Weather Station is Currently Undergoing Maintenance</span>
            <FaScrewdriverWrench className="text-slate-800 w-36 h-36 my-10" />
        </div>
        <div className="flex flex-col">
            <h1 className="text-2xl font-semibold">What is maintenance mode?</h1>
            <p className="text-xl py-5">
                This website is "maintenance mode" when the BU Weather Station is being serviced, or a critical error
                has occurred in the data being collected. For instance, this site chould be in maintenance mode when the 
                battery needs changed, or when garbage data was detected by administrators and is being collected.
                <br/><br/>
                The time until the station is back up can vary be severity by the issue. Unfortunately, the weather station
                is still under active development and the uptime can be unreliable until a full release is done. We
                appreciate your patience and support!
            </p>
            <h1 className="text-2xl font-semibold">Is data still being collected?</h1>
            <p className="text-xl py-5">
                Typically no, and if data <i>is</i> still being collected and available through the API, then it should
                not be trusted. We are actively working on a more streamlined solution for the API for cases like this.
            </p>
            <h1 className="text-2xl font-semibold">Is the API still available?</h1>
            <p className="text-xl py-5">
                For now, yes. Although: data fetched from the API while the station is in this state should not be
                trusted. Currently, the API has no way of notifying the user of bad data, or that the website is in
                maintenance mode (because the website and API are two different systems) but in time, we should be 
                providing a solution that will notify users when data is temporarily unavilable through the API.
            </p>
        </div>
        <div className="flex flex-col items-center">
            <span className="flex mt-5">We'll be back up as soon as possible!</span>
            <span className="flex font-bold"> For questions, comments, or concerns, contact administrators at: jsimeone@mail.bradley.edu.</span>
            <span className="text-xl mt-5">Maintenance Log:</span>
            <MaintenanceLogCard/>
        </div>
    </div>
    )
}
