/*
 * ServerManager.h
 *
 *  Created on: 8 mars 2017
 *      Author: Bruno
 *
 *  This class manager all servers usefull. TODO: optionnal server, now it's everythings or nothings
 */

#ifndef SERVERMANAGER_H_
#define SERVERMANAGER_H_

#include <ESP8266mDNS.h>
#include <ESP8266FtpServer.h>
#include <ESP8266WebServer.h>
#include "esp8266TelnetServer/ESP8266TelnetServer.h"
#include <ArduinoOTA.h>
#include <FS.h>

class ServerManager {
public:
	ServerManager();
	virtual ~ServerManager();

	void initDnsHttpFtpOtaTelnetServers(char* dnsName, char* ftpUser,
			char* ftpPasseWord, char*otaHostName, char*otaPasseWord);
	void updateServers();

	// methodes to handle http requests on file in SPIFFS
	void handleRequestFile();bool loadFromSpiffs(String path);

	void printDebug(String);
	void printlnDebug(String);

	ESP8266WebServer httpServer; // port default=80
private:
	MDNSResponder mdns;
	FtpServer ftpSrv; //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
	ESP8266TelnetServer telnetServeur;

};

#endif /* SERVERMANAGER_H_ */
