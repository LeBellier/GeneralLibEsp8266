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
#include "Logger.h"

class ESP8266TelnetServer: public Print {

public:
	ESP8266TelnetServer();
	virtual ~ESP8266TelnetServer();
	void began(Logger*);
	void handle(Logger*);
	void print(String);
	void println(String);
	void disconnect(Logger*);
	size_t write(uint8_t) override;

private:
	WiFiClient serverClient;

};

#endif /* _ESP8266TELNETSERVER_H_ */
