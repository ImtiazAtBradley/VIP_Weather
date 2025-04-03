import { RedHatDisplay } from '@/app/ui/fonts';

export default function Page() {
    return (
        <>
            <div className={`flex flex-col mt-12 ${RedHatDisplay.className}`}>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold">
                        What is the BECC Weather Station?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
                            The BECC Weather Station is a collaborative project within the Electrical & Computer Engineering Vertically Integrated Project class (ECE 398). The station is designed by students and deployed outdoors
                            on Bradley's campus in order to inform students of the current local weather.
                        </p>
                    </div>
                </div>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold mt-10">
                        What is Vertically Integrated Project?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
                            Vertically Integrated Projects is a class within Bradley's Electrical Engineering department responsible for the development and maintenence of the BECC Weather Station. Vertically Integrated Projects (or VIP)
                            is a class where students are encouraged to use and share knowledge amongst themselves to create "products" like the weather station to gain professional experience with hands-on projects.
                        </p>
                    </div>
                </div>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5 mb-10">
                    <div className="text-4xl font-bold mt-10">
                        Who Built this Thing?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
				The weather station was built by a team of talented ECE 398 VIP students in conjunction with Electrical Engineering staff. A special thanks goes out to professor Imtiaz, our advisor for the course. Primary student developers include: Kyle K. Jacob S. Emily A. Owen J. Chris H. Logan V. Freddie M. Ryuto N. Grace T. Riley P. Matthew V.
                        </p>
                    </div>
                </div>

                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5 mb-10">
                    <div className="text-4xl font-bold mt-10">
                        Want to Build Your Own Station?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
				Everything that you would need/want to know can be found above by clicking the documentation tab. This will provide you with the framwork of the weather station!
                        </p>
                    </div>
                </div>
            </div>
        </>
    )
}
