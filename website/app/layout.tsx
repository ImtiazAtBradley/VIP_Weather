import '@/app/ui/global.css';
import WeatherStationNav from "./ui/weather-station-nav";
import { RedHatDisplay } from './ui/fonts';

export default function RootLayout({
	children,
}: {
	children: React.ReactNode;
}) {
	return (
		<html lang="en">
			<title>BECC Weather - ECE398</title>
			<body>
				<div className='flex flex-col'>
					<main className={`flex flex-col md:p-6 mb-auto p-2 ${RedHatDisplay.className}`}>
						<div className="flex itmes-center p-4">
							<WeatherStationNav />
						</div>
						{children}
					</main>

					<footer className="flex flex-col w-screen">
						<div className=" bg-red-600">
							<div className="max-w-2xl mx-auto text-white py-10 px-4 md:px-0">
								<div className="text-center">
									<a target='_blank' href='https://litemage.github.io/bu-vip-website' className="text-3xl hover:underline text-red-50">A Vertically Integrated Projects Design</a>
									<p className='text-red text-red-100'>Report any issues to: <a target='_blank' className='underline hover:text-red-50' href="https://github.com/ImtiazAtBradley/VIP_Weather/issues">our GitHub</a></p>
									<p className='text-red text-red-300'>Designed and realized by the ECE 398 weather station group</p>
									<p className='text-red text-red-300'>Electrical & Computer Engineering Department 2025</p>
									<p className='text-red text-red-300'>Bradley University</p>
								</div>
							</div>
						</div>
					</footer>

				</div>
			</body>
		</html>
	);
}
