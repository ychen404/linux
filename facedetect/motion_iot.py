#!/usr/bin/python

import subprocess
import time
import sys

i=0;

while True:
    
	i+=1
	if i%3 is 0: 
        	print "[emulating motion-sensor] Intruder detected"
            	on = time.time()
	   	time.sleep(0.1)
            	subprocess.call(['sudo','./chrt','-i', '98', 'python', 'camera.py'])
	    	time.sleep(0.1)
            	subprocess.call(['sudo', './chrt', '-i','98', './facedetect', 'data/lena.jpg'])
            	time.sleep(1)    
	else:
		print "[emulating motion-sensor] No intruder"
		time.sleep(1)     
