import { RedHatDisplay } from '@/app/ui/fonts';

export default function Page() {
    return (
        <>
            <div className={`mt-12 ${RedHatDisplay.className}`}>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold">
                        What is The BECC Weather Station?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
                            The BECC Weather Station is a collaborative project within the Electrical & Computer Engineering Vertically Integrated Projects class (ECE 398), designed by students to deploy outdoors
                            on Bradley's campus, in order to inform other students on campus of the current weather, or even predict it in the future!
                        </p>
                    </div>
                </div>
                <div className="text-center shadow-lg rounded border border-gray-200 mt-5 p-5">
                    <div className="text-4xl font-bold mt-10">
                        What is Vertically Integrated Projects?
                    </div>
                    <div className="p-6">
                        <p className="text-lg">
                            Vertically Integrated Projects is a class within Bradley's Electrical Engineering department responsible for the development and maintenence of the BECC Weather Station. Vertically Integrated Projects (or VIP)
                            is a class where students are encouraged to use and share knowledge amongst themselves to create "products" like the weather station to gain professional experience with hands-on projects.
                        </p>
                    </div>
                </div>

            </div>
        </>
    )
}