export default function Alert({color, title, msg} : {color: string, title: string, msg: string}) {
    return (
        <div className="mt-5" role="alert">
        <div className={`bg-${color}-500 text-white font-bold rounded-t px-4 py-2`}>
            {title}
        </div>
        <div className={`border border-t-0 border-${color}-400 rounded-b bg-${color}-100 px-4 py-3 text-${color}-700`}>
            <p>{msg}</p>
        </div>
        </div>
    )
}