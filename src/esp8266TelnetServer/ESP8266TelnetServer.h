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
#include <Arduino.h>

class ESP8266TelnetServer {
public:
	ESP8266TelnetServer();
	void began();
	void handle();
	void send(String msg);

private:
	WiFiClient serverClient;

};

#endif /* _ESP8266TELNETSERVER_H_ */
