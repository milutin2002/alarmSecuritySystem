import machine
import time
warnFlag=False
def motionDetected(pin):
    global warnFlag
    warnFlag=True
    print("Motion detected")
    
sensor=machine.Pin(17,machine.Pin.IN)
sensor.irq(trigger=machine.Pin.IRQ_RISING,handler=motionDetected)
alarm=machine.Pin(15,machine.Pin.OUT)
while True:
    if warnFlag==True:
        alarm.toggle()
        warnFlag=False
        time.sleep(3)
        alarm.toggle()