import '@/app/ui/global.css';
import WeatherStationLogo from "./ui/weather-station-logo";
import { RedHatDisplay } from './ui/fonts';

export default function RootLayout({
  children,
}: {
  children: React.ReactNode;
}) {
  return (
    <html lang="en">
      <body>
        <div className='flex flex-col h-screen'>
        <main className={`flex flex-col md:p-6 mb-auto p-2 ${RedHatDisplay.className}`}>
          <div className="flex itmes-center md:h-28 h-20 rounded-lg bg-red-500 p-4">
            <WeatherStationLogo />
          </div>
          {children}
        </main>
        <footer>
          <div className=" bg-red-600">
            <div className="max-w-2xl mx-auto text-white py-10">
              <div className="text-center">
                <h3 className="text-3xl">A Vertically Integrated Projects Design</h3>
                <p className='text-red text-red-300'>Designed and realized by the ECE 398 weather station group</p>
              </div>
              <div className="mt-12 flex flex-col md:flex-row md:justify-between items-center text-sm text-white">
                <p className="order-2 md:order-1 mt-8 md:mt-0"> Bradley University Electrical And Computer Enginnering Dpt. - 2024 </p>
                <div className="order-1 md:order-2">
                  <a href="/about"><span className="px-2">About</span></a>
                  <a href="/contact">
                    <span className="px-2 border-l">Contact</span>
                  </a>
                </div>
              </div>
            </div>
          </div>
        </footer>

        </div>
      </body>
    </html>
  );
}
