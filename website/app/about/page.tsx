import { RedHatDisplay } from '@/app/ui/fonts';

export default function Page() {
    return (
        <>
            <div className={`flex flex-col mt-12 ${RedHatDisplay.className}`}>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold">
                        What is The BECC Weather Station?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
                            The BECC Weather Station is a collaborative project within the Electrical & Computer Engineering Vertically Integrated Projects class (ECE 398), designed by students to deploy outdoors
                            on Bradley&aposs campus, in order to inform other students on campus of the current weather, or even predict it in the future!
                        </p>
                    </div>
                </div>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold mt-10">
                        What is Vertically Integrated Projects?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
                            Vertically Integrated Projects is a class within Bradley&aposs Electrical Engineering department responsible for the development and maintenence of the BECC Weather Station. Vertically Integrated Projects (or VIP)
                            is a class where students are encouraged to use and share knowledge amongst themselves to create &quotproducts&quot like the weather station to gain professional experience with hands-on projects.
                        </p>
                    </div>
                </div>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold mt-10">
                        Who built this thing?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
				The weather station was built by a team of talented ECE 398 VIP students in conjunction with Electrical Engineering staff. A special thanks goes out to professor Imtiaz, our advisor for the course. Primary student developers include: Kyle K. Jacob S. Emily A. Owen J. Samantha P. Chris H. Logan V. Freddie M. Ryuto N.
                        </p>
                    </div>
                </div>

            </div>
        </>
    )
}
