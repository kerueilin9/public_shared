"""import Jetson.GPIO as GPIO
import time
import sys


SPICLK = 11
SPIMISO = 9
SPIMOSI = 10
SPICS = 8
output_pin = 7

photo_ch = 0

LED1PORT = 17
LED2PORT = 22

LED1 = "OFF"
LED2 = "OFF"

def init():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(output_pin, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.setwarnings(False)
    GPIO.cleanup()
    GPIO.setmode(GPIO.BCM)
    
    GPIO.setup(SPIMOSI, GPIO.OUT)
    GPIO.setup(SPIMISO, GPIO.IN)
    GPIO.setup(SPICLK, GPIO.OUT)
    GPIO.setup(SPICS, GPIO.OUT)
    GPIO.setup(LED1PORT, GPIO.OUT)
    GPIO.setup(LED2PORT, GPIO.OUT)
    
def readadc(adcnum, clockpin, mosipin, misopin, cspin):
    if ((adcnum > 7) or (adcnum < 0)):
        return -1
    GPIO.output(cspin, True)
    
    GPIO.output(clockpin, False)
    GPIO.output(cspin, False)
    
    commandout = adcnum
    commandout |= 0x18
    commandout <<= 3
    for i in range(5):
        if (commandout & 0x80):
            GPIO.output(mosipin, True)
        else:
            GPIO.output(mosipin, False)
        commandout <<= 1
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False)
        
    adcout = 0
    
    for i in range(12):
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False)
        adcout <<= 1
        if (GPIO.input(misopin)):
            adcout |= 0x1
            
    GPIO.output(cspin, True)
    
    adcout >>= 1
    return adcout

def main():
	init()
	# adc_value
	adc_value = 0
	#while sys.argv[1] == "true":
    adc_value = readadc(photo_ch, SPICLK, SPIMOSI, SPIMISO, SPICS)
    
    if adc_value > 700:
        LED1 = "ON"
        LED2 = "ON"
    elif adc_value > 100:
        LED1 = "ON"
        LED2 = "OFF"
    else:
        LED1 = "OFF"
        LED2 = "OFF"

    print "adc_value", adc_value
    #print "LED1", LED1
    #print "LED2", LED2
    if LED1 == "ON":
        GPIO.output(LED1PORT, True)
    else:
        GPIO.output(LED1PORT, False)
    if LED2 == "ON":
        GPIO.output(LED2PORT, True)
    else:
        GPIO.output(LED2PORT, False)
    time.sleep(1)




if __name__ == '__main__':
	try:
		#print(str(random.random()))
		main()
	except:
		print("error")"""

import Jetson.GPIO as GPIO
import time
import sys


SPICLK = 11
SPIMISO = 9
SPIMOSI = 10
SPICS = 8
output_pin = 7

photo_ch = 0

LED1PORT = 17
LED2PORT = 22

LED1 = "OFF"
LED2 = "OFF"

def init():
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(output_pin, GPIO.OUT, initial=GPIO.HIGH)
    GPIO.setwarnings(False)
    GPIO.cleanup()
    GPIO.setmode(GPIO.BCM)
    
    GPIO.setup(SPIMOSI, GPIO.OUT)
    GPIO.setup(SPIMISO, GPIO.IN)
    GPIO.setup(SPICLK, GPIO.OUT)
    GPIO.setup(SPICS, GPIO.OUT)
    GPIO.setup(LED1PORT, GPIO.OUT)
    GPIO.setup(LED2PORT, GPIO.OUT)
    
def readadc(adcnum, clockpin, mosipin, misopin, cspin):
    if ((adcnum > 7) or (adcnum < 0)):
        return -1
    GPIO.output(cspin, True)
    
    GPIO.output(clockpin, False)
    GPIO.output(cspin, False)
    
    commandout = adcnum
    commandout |= 0x18
    commandout <<= 3
    for i in range(5):
        if (commandout & 0x80):
            GPIO.output(mosipin, True)
        else:
            GPIO.output(mosipin, False)
        commandout <<= 1
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False)
        
    adcout = 0
    
    for i in range(12):
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False)
        adcout <<= 1
        if (GPIO.input(misopin)):
            adcout |= 0x1
            
    GPIO.output(cspin, True)
    
    adcout >>= 1
    return adcout

def main():
	init()
	# adc_value
	adc_value = 0
	#while sys.argv[1] == "true":
	adc_value = readadc(photo_ch, SPICLK, SPIMOSI, SPIMISO, SPICS)
	
	if adc_value > 700:
		LED1 = "ON"
		LED2 = "ON"
	elif adc_value > 100:
		LED1 = "ON"
		LED2 = "OFF"
	else:
		LED1 = "OFF"
		LED2 = "OFF"

	print adc_value
	#print "LED1", LED1
	#print "LED2", LED2
	if LED1 == "ON":
		GPIO.output(LED1PORT, True)
	else:
		GPIO.output(LED1PORT, False)
	if LED2 == "ON":
		GPIO.output(LED2PORT, True)
	else:
		GPIO.output(LED2PORT, False)
	#time.sleep(1)
	

#import random



if __name__ == '__main__':
	try:
		#print(str(random.random()))
	
		main()
	except:
		print("error")

	

