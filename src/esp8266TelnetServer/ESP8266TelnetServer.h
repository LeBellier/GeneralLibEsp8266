/*
 * ESP8266TelnetServer.h
 *
 *  Created on: 1 mars 2017
 *      Author: Bruno
 */

#ifndef _ESP8266TELNETSERVER_H_
#define _ESP8266TELNETSERVER_H_

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>

class ESP8266TelnetServer: public WiFiServer {
public:
	ESP8266TelnetServer(uint16_t port = 23);
	void begin();
	void handle();
	void send(String msg);

private:
	WiFiClient serverClient;

};

#endif /* _ESP8266TELNETSERVER_H_ */
