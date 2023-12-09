import requests
import random
import time

url = 'http://ec2-18-116-36-90.us-east-2.compute.amazonaws.com:3002/api/particle_data'
for i in range(20):
    myobj = {"event":"hr",
            "HR":"%s"%random.randint(70, 100),
            "SPO2":"%s"%random.randint(96, 100),
            "coreid":"e00fce680993c866c8f5c213"}
    x = requests.post(url, json = myobj)
    time.sleep(1)