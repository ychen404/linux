#!/usr/bin/env python
from time import sleep
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)

GPIO.setup(21, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

try:
    while True:
        sleep(1)
        if GPIO.input(21):
            print "motion detected\n"
        else:
            print "no motion\n"

finally:                  
        GPIO.cleanup()
        print "All cleaned up."
