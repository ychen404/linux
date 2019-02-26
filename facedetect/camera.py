#!/usr/bin/python

from picamera import PiCamera
import sys

filepath = str(sys.argv[1])

camera = PiCamera()
camera.resolution = (608, 608)
camera.capture(filepath)
