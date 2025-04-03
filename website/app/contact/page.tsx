export default function Page() {
    return (
        <>
            <div className="flex flex-col mt-12 h-screen">
                <p className="text-2xl font-bold text-center">Contact administrators with any questions, comments, or concerns regarding the weather station, or ECE 398:</p>
                <div className="flex py-10 md:flex-row flex-col justify-center">
                    <div className="flex flex-col p-10 mx-12 md:mb-0 mb-12 border shadow-lg rounded">
                        <span className="text-2xl font-bold">Jacob Simeone</span>
                        <span className="text-xs tracking-tight">Student Lead Developer of the BECC Weather Station</span>
                        <a className="text-lg mt-5 text-red-400 underline" href="mailto:jsimeone@mail.bradley.edu">jsimeone@mail.bradley.edu</a>
                    </div>
                    <div className="flex flex-col p-10 mx-12 border shadow-lg rounded">
                        <span className="text-2xl font-bold">Dr. Imtiaz</span>
                        <span className="text-xs tracking-tight">Faculty Advisor for ECE 398 Vertically Integrated Projects</span>
                        <a  className="text-lg mt-5 text-red-400 underline" href="mailto:mimtiaz@fsmail.bradley.edu">mimtiaz@fsmail.bradley.edu</a>
                    </div>
                </div>
            </div>
        </>
    )
}