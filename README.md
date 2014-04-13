SensorNet
=========

Contains all the code used during Space Apps Challenge 2014

This project was made up of two Arduino based projects using DHT22(rh03) sensors together with 6 Dallas One-wire DS18b20, which one of them was waterproof. The hardware and compondents used in this project were cheaply obtained from the internet, borrowed from other members of build brighton and other items from other projects.  

SensorNet was put together upon a proto board. It also had a AT-Mega 328 UNO IC mounted upon a proto sheild together with the required power circuit and other compondents to make a breadboard arduino for under £5.
The display was provided by a 2 digit seven segament display driven by 2 shift regesters, one for each digit. It had terminal conections for the one-wire sensors and for power; operating voltage was 5v, provided by 7805 voltage regulator so that you could use a mains adapter or a batteries to power the hub.
There where aslo nRF24L01 transrecivers which proived two way data transfer between the CoolIT upload project.

CoolIT Upload Module was made up of a single DHT22(RH03) sensor mounted upon a breadboard and using an Arduino Uno together with a ethernet sheild to provide a networked data connection to the SensorNet and upload the data to xivley.
This data can be viewed here https://xively.com/feeds/1166752800

The 7 segment displays used in this project are common anode meaning that the led's share the postive connection;
This requiremet needs a zero (take it LOW) on the shift registers output which will turn on the required Led segment.


