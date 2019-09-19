# Simple-MQTT-Relais
Simple Relais with ESP-01, connected with MQTT

## Hardware
I use the quite bad Relais here: 
As you can see, I had to solder some lines to enable proper boot of the ESP-01. (Both Pins HIGH)
![img](doc/DSC_0719.JPG)


## Software
Arduino standard Library PubSubClient, probably something similar to [this](https://github.com/knolleary/pubsubclient) one.
Also, use PlatformIO instead of this cancer-ide that comes with arduino.
