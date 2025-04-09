# Data Sim

This directory contains a Python file which simulates weather data sent to the api. Use for testing of api/website/database stack. 

On the back-end, the simulator generates some random walk signals with configurable weights that update at a set interval for all time unless stopped, or fails to connect to API. 

**Some Notes:**

* Target URL is set in python code
* You can change the weights to get data to change by bigger steps in code
* You can change the delay by changing code
* To change the URL, you will have to edit the code as well
* You MUST pass the api key generated for you by administrators (although that might be you if you are running this locally)

# Running

Install requirements with: 

```
pip install -r requirements.txt
```

Then, you can use the program. Make sure the server you are targeting is running.

```
python3 WSDataSim.py your-api-key
```
