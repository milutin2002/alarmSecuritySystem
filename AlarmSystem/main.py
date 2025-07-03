import machine
import time
warnFlag=False
def motionDetected(pin):
    global warnFlag
    warnFlag=True
    print("Motion detected")
    
led=machine.Pin(13,machine.Pin.OUT)
sensor=machine.Pin(17,machine.Pin.IN)
sensor.irq(trigger=machine.Pin.IRQ_RISING,handler=motionDetected)
alarm=machine.Pin(15,machine.Pin.OUT)
while True:
    if warnFlag==True:
        alarm.toggle()
        led.on()
        warnFlag=False
        time.sleep(1)
        alarm.toggle()
        led.off()