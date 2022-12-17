# smart-meter-wifi-publisher
Yet another solution to read smart meter registers and publish the results over Wi-Fi using MQTT.

## What?
The goal is to fetch smart meter values and send them to a web-server over Wi-Fi. These values can then be used as you wish, allowing for a better understanding of how much electrical energy is being consumed at any given time. 

## Why?
Because it empowers users to make better decisions on energy consumption/saving.

- "How much does it cost to cook using this electrical cooker?" 
- "Could I use something else that's cheaper than this heater?"
- "Would I benefit from upgrading my kitchen to use other energy source?"

These are the sort of questions one can answer by using such a solution.

## How?
This particular implementation is suited to [Portuguese e-redes smart meters](https://www.e-redes.pt/sites/eredes/files/2020-07/DEF-C44-509.pdf). The communication is made though a RJ-12 [RS-485](https://en.wikipedia.org/wiki/RS-485) serial interface, present on the device itself. 

![RJ-12 interface](/assets/emi.png?raw=true "RJ-12 serial inter")

It starts by querying specific registers on the meter, using MODBUS as communication protocol. This information is then published in JSON form over the ethernet using [MQTT](https://mqtt.org/).

Finally, this information can be collected and analyzed in any IoT system, such as [Home Assistant](https://www.home-assistant.io/).

![Home Assistant example](/assets/ha.png?raw=true "Home Assistant example")

## What hardware is needed?
The software is hardware independent, as long as it supports [RS-485](https://en.wikipedia.org/wiki/RS-485) serial and Wi-Fi communication.

However, for this project we used the famous ESP8266 WiFi Module and a RS-485 to TTL serial converter.

<div align="center">
    <img src="/assets/rs485-to-ttl.png">
    <br>
    RS-485
</div>
<div align="center">
    <img src="/assets/esp8266.png">
    <br>
    ESP8266
</div>

The connections were done as follows.
![Connections](/assets/connections.png?raw=true "Connections")

## How is this project organized?
To compile the project, users should install the necessary dependencies.

- `ESP8266WiFi.h` - used to setup the Wi-Fi Connection.
- `ArduinoJson.h` - used to generate the JSON objects to send over the network.
- `ArduinoMqttClient.h` - used to connect to the MQTT broker.

It is also necessary to setup `secrets.h` with the correct settings. A sample file is provided.

## References
--------------------------------------
- [e-redes smart power meters](https://www.e-redes.pt/sites/eredes/files/2020-07/DEF-C44-509.pdf)
- [MQTT](https://mqtt.org/)
- [Home Assistant](https://www.home-assistant.io/)
- [MODBUS protocol](https://en.wikipedia.org/wiki/Modbus)
- [Example project](https://github.com/nikito7/edpbox)