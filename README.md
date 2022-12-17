# smart-meter-wifi-publisher
Yet another solution to read smart meter registers and publish the results over Wi-Fi using MQTT.

## What?
The goal of this project is to fetch smart meter (electrical) register values and to send them to a web-server over Wi-Fi. These values can then be used as you wish, allowing for a better understanding of how much electrical energy is being consumed at any given time. 

## Why?
Because it empowers users to make better decisions on saving energy and thus, save resources. 
- How much does it cost to cook using that electrical cooker you have? 
- Could you use something else that's cheaper?
- Would I benefit from upgrading my kitchen to use other energy source?
These are the sort of questions one can answer by using such a solution.

## How?
This particular implementation is suited to Portuguese e-redes smart meters. The communication is made though a RJ-12 serial interface present on the device itself. 

![RJ-12 interface](/assets/emi.png?raw=true "RJ-12 serial inter")

The communication protocol used in this interface is MODBUS with some specificities.


## What hardware is needed?





## References
--------------------------------------
- [e-redes smart power meters](https://www.e-redes.pt/sites/eredes/files/2020-07/DEF-C44-509.pdf)
- [MODBUS protocol](https://en.wikipedia.org/wiki/Modbus)
- [Example project](https://github.com/nikito7/edpbox)