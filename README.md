# Zentralheizung-Webserver

Stand: 6.10.2019

6.10.2019
Ein- und Ausschaltezeiten auf 15 Minuten geändert.

1.9.2019
Webserver zur Steuerung einer Zentralheizung mit Wasserkreislauf
Der Code hat eine feste IP-Konfig:
IP 192.168.0.71
Gateway 192.168.0.253
Subnet 255.255.255.0

An das Netzwerk ist der WEbserver angebunden mit einem W5500 Ethernet-Shield.
Elektronisch wird der Arduino und der W5500 wie folgt angebunden:
Arduino - - - W5500
3,3V        3,3V
oder
5,0V        5,0V

GND         GND\
13          SCLK\
12          MISO\
11          MOSI\
10          SCSn\
Reset       Reset\

Das Projekt ist entstanden aus dem Projekt von Salvatore Fancello welches auf der Webseite http://progettiarduino.com zu finden ist.
