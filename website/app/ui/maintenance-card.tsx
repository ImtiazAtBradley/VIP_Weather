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
    <div className="mt-3 mb-3 p-10">
        <div className="flex flex-col items-center">
            <span className="flex text-6xl font-semibold text-slate-600">Weather Station is Currently Undergoing Maintenance</span>
            <FaScrewdriverWrench className="text-slate-800 w-36 h-36 my-10" />
            <span className="flex mt-5">We'll be back up as soon as possible!</span>
            <span className="flex font-bold"> For questions, comments, or concerns, contact administrators at: jsimeone@mail.bradley.edu.</span>
            <span className="text-xl mt-5">Maintenance Log:</span>
            <MaintenanceLogCard/>
        </div>
    </div>
    )
}
