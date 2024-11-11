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
			<title>BECC Weather - ECE398</title>
			<body>
				<div className='flex flex-col h-full'>
					<main className={`flex flex-col md:p-6 mb-auto p-2 ${RedHatDisplay.className}`}>
						<div className="flex itmes-center md:h-28 h-20 rounded-lg bg-red-500 p-4">
							<WeatherStationLogo />
						</div>
						{children}
					</main>

					<footer className="flex flex-col w-screen">
						<div className=" bg-red-600">
							<div className="max-w-2xl mx-auto text-white py-10 px-4 md:px-0">
								<div className="text-center">
									<h3 className="text-3xl">A Vertically Integrated Projects Design</h3>
									<p className='text-red text-red-300'>Designed and realized by the ECE 398 weather station group</p>
									<p className='text-red text-red-300'>Electrical & Computer Engineering Department 2024</p>
									<p className='text-red text-red-300'>Bradley University</p>
								</div>
								<div className="mt-12 flex flex-col items-center text-sm text-white">
									<div className="order-1 md:order-2">
										<a href="/about"><span className="px-2">About</span></a>
										<a href="/contact">
											<span className="px-2 border-l">Contact</span>
										</a>
										<a href="/documentation">
											<span className="px-2 border-l">Documentation</span>
										</a>
										<a href="https://github.com/ImtiazAtBradley/VIP_Weather" target='_blank'>
											<span className="px-2 border-l">GitHub</span>
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
