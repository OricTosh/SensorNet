SensorNet
=========

Contains all the code used during Space Apps Challenge 2014

An Arduino based project using a duel 7 Segment Display with a Dallas DS18B20 temperture sensor using 2 shift registers.
Also has 6 one wire temp sensors attached and uploads the data to Xively.

The 7 segment displays used in this project are common anode meaning that the led's share the postive connection;
This requiremet needs a zero (take it LOW) on the shift registers output which will light up the Led.


