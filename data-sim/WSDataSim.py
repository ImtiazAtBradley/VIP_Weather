from datetime import datetime
import sys
from math import floor
import requests
import random
import time

# Random walk class
class RandomWalk:
    def __init__(self, initial: float = 0, weight: float = 1) -> None:
        # Value of random walk signal
        self.val = initial
        # Weight of new random inputs
        self.weight = weight
    
    def get_next(self) -> float:
        self.val = self.val + (self.weight * random.uniform(-1.0, 1.0))
        return self.val

# API Interraciton class
class WeatherStationAPI:
    def __init__(self, url: str, key: str) -> None:
        self.url = url
        self.key = key
    
    def post_data(self, temp: float, pres: float, humid: float, gas: float, light: float, water: float) -> bool:
        # TODO: Fill in data
        fakeWeather = {
            'timestamp_unix_ms' : get_unix_time_ms(),
            'temp_c' : temp,
            'humid_prcnt' : humid,
            'pressure_kpa' : pres,
            'gas_kohms' : gas,
            'rain_an' : water,
            'light_an' : light
        }
        print(F"Sending data: {fakeWeather}")
        try:
            requests.post(url=self.url, headers={'Authorization' : self.key}, json=fakeWeather)
        except:
            return False

        return True


# Get current unix timestamp in ms
def get_unix_time_ms():
    return floor(datetime.now().timestamp() * 1000)

def get_random_float(min, max):
    return random.uniform(min, max)

def main():
    if len(sys.argv) < 2 or type(sys.argv[1]) != str:
        print("Must provide api key as argument")
        return

    wsapi = WeatherStationAPI("http://localhost:27500/api/envdata", sys.argv[1])
    temp = RandomWalk(20, 3)
    pres = RandomWalk(90, 0.5)
    humid = RandomWalk(80, 1)
    gas = RandomWalk(4095, 1)
    light = RandomWalk(2024, 0.5)
    water = RandomWalk(2025, 0.5)

    while True:
        ret = wsapi.post_data(temp.get_next(), pres.get_next(), humid.get_next(), gas.get_next(), light.get_next(), water.get_next())
        if not ret:
            print("Failed to send data to api, aborting")
            return
        time.sleep(5)

if __name__ == "__main__":
    main()