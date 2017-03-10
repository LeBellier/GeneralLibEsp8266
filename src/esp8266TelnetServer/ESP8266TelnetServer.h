/*
 * ESP8266TelnetServer.h
 *
 *  Created on: 1 mars 2017
 *      Author: Bruno
 *
 *  This class is a Telnet server which handle the first client connected until its disconnexion.
 *
 *  Char read go directely to Serial, TODO: add methodes to manage these characters.
 */

#ifndef _ESP8266TELNETSERVER_H_
#define _ESP8266TELNETSERVER_H_

#include <ESP8266WiFi.h>
#include <Aspect.h>

class ESP8266TelnetServer: public Print {

public:
	ESP8266TelnetServer();
	virtual ~ESP8266TelnetServer();
	void begin();
	void handleClient();bool hasConnectedClient();
	void stopClient();

	size_t write(uint8_t) override;
	size_t write(const uint8_t *, size_t) override;

private:
	WiFiClient serverClient;

};

#endif /* _ESP8266TELNETSERVER_H_ */
