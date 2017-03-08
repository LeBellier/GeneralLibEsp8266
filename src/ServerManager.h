/*
 * ServerManager.h
 *
 *  Created on: 8 mars 2017
 *      Author: Bruno
 */

#ifndef LIBRARIES_GENERALLIBESP8266_SERVERMANAGER_H_
#define LIBRARIES_GENERALLIBESP8266_SERVERMANAGER_H_

#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "esp8266FTPServer/ESP8266FtpServer.h"
#include "esp8266TelnetServer/ESP8266TelnetServer.h"
#include <ArduinoOTA.h>
#include <FS.h>
#include "Logger.h"

class ServerManager {
public:
	ServerManager(Logger*);
	virtual ~ServerManager();

	void initDnsHttpFtpOtaTelnetServers(char* dnsName, char* ftpUser,
			char* ftpPasseWord, char*otaHostName, char*otaPasseWord);
	void updateServers();

	void handleRequestFile();bool loadFromSpiffs(String path);

	MDNSResponder mdns;
	ESP8266WebServer httpServer; // port default=80
	FtpServer ftpSrv; //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial
	ESP8266TelnetServer telnetServeur;

	Logger* logger;
};

#endif /* LIBRARIES_GENERALLIBESP8266_SERVERMANAGER_H_ */
