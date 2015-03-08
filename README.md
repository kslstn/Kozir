#Cozir CO2 Sensor Arduino Library without delay#

This library is based on the library authored by [DirtGambit](http://forum.arduino.cc/index.php?action=profile;u=47469). You can find it [on my GitHub](https://github.com/kslstn/cozir) too.

That original uses delay(), because the sensor needs around 100 ms to answer a request. In this library, the request for a measurement and getting the response is split up in separate functions. This way, you can run time sensitive stuff in your loop, like fading LEDs, without hickups. Have a look at the simple example to see what functions to call.

This library only supports requesting temperature, humidity and CO2 data. The original can be used for much more, like setting the sensor to polling mode, a requirement to use this library.