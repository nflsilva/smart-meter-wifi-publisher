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
This particular implementation is suited to [Portuguese e-redes smart meters](https://www.e-redes.pt/sites/eredes/files/2020-07/DEF-C44-509.pdf). The communication is made though a RJ-12 serial interface present on the device itself. 

![RJ-12 interface](/assets/emi.png?raw=true "RJ-12 serial inter")

It starts by querying specific registers on the meter, using MODBUS as communication protocol. This information is then published in JSON form over the ethernet using [MQTT](https://mqtt.org/).

Finally, this information can be collected and analyzed on any of the user's favorite system, such as [Home Assistant](https://www.home-assistant.io/).

![Home Assistant example](/assets/ha.png?raw=true "Home Assistant example)

## What hardware is needed?





## References
--------------------------------------
- [e-redes smart power meters](https://www.e-redes.pt/sites/eredes/files/2020-07/DEF-C44-509.pdf)
- [MQTT](https://mqtt.org/)
- [Home Assistant](https://www.home-assistant.io/)
- [MODBUS protocol](https://en.wikipedia.org/wiki/Modbus)
- [Example project](https://github.com/nikito7/edpbox)