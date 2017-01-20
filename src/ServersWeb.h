#ifndef SERVERSWEB_H
#define SERVERSWEB_H

// Prototypage
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266FtpServer.h>
#include <Aspect.h>
#include <FS.h>

void initDnsHttpFtpServers();
void updateServers();

void handleRequestOnFile();
bool loadFromSpiffs(String path);

MDNSResponder mdns;
ESP8266WebServer httpServer; // port default=80
FtpServer ftpSrv; //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial

// Declaration

void initDnsHttpFtpServers() {
	//init spiffs (spi file system)
	if (!SPIFFS.begin()) {
		DEBUG_INIT_PRINTLN("Can't open de File system");
	}
	//init DNS
	if (mdns.begin(dnsName, WiFi.localIP())) {
		DEBUG_INIT_PRINTLN("MDNS responder started");
		DEBUG_INIT_PRINT("You can use the name: http://");
		DEBUG_INIT_PRINT(dnsName);
		DEBUG_INIT_PRINTLN(".local/");
	}
	//init Http Server
	httpServer.onNotFound(handleRequestOnFile);
	httpServer.begin();
	DEBUG_INIT_PRINTLN("HTTP server started");

	//init Ftp Server
	ftpSrv.begin(ftpUser, ftpPasseWord); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
	DEBUG_INIT_PRINT("FTP server started; MdP:");
	DEBUG_INIT_PRINT(ftpUser);
	DEBUG_INIT_PRINT(", User:");
	DEBUG_INIT_PRINTLN(ftpPasseWord);

}
void updateServers() {
	mdns.update();
	ftpSrv.handleFTP();
	httpServer.handleClient();
}

void handleRequestOnFile() {
	String uriAsked = httpServer.uri();
	DEBUG_PRINTLN(uriAsked);

	//check the request
	if (!loadFromSpiffs(uriAsked)) { // no file at the uri found
		String message = "File Not Detected  ";
		message += "URI: ";
		message += uriAsked;
		message += "\n  Method: ";
		message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
		message += "\n  Arguments: ";
		message += httpServer.args();
		for (uint8_t i = 0; i < httpServer.args(); i++) {
			message += "\n NAME:" + httpServer.argName(i) + " VALUE:"
					+ httpServer.arg(i);
		}
		httpServer.send(404, "text/plain", message);
		DEBUG_INIT_PRINTLN(message);
	}
}

bool loadFromSpiffs(String path) {

	String dataType = "text/plain";
	if (path.endsWith("/"))
		path += "index.html";

	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));

	if (path.endsWith(".html"))
		dataType = "text/html";
	else if (path.endsWith(".css"))
		dataType = "text/css";
	else if (path.endsWith(".js"))
		dataType = "application/javascript";
	else if (path.endsWith(".png"))
		dataType = "image/png";
	else if (path.endsWith(".gif"))
		dataType = "image/gif";
	else if (path.endsWith(".jpg"))
		dataType = "image/jpeg";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".xml"))
		dataType = "text/xml";
	else if (path.endsWith(".pdf"))
		dataType = "application/pdf";
	else if (path.endsWith(".zip"))
		dataType = "application/zip";

	File dataFile = SPIFFS.open(path.c_str(), "r");
	if (!dataFile) {
		return false;
	}

	if (httpServer.hasArg("download"))
		dataType = "application/octet-stream";
	if (httpServer.streamFile(dataFile, dataType) != dataFile.size()) {
		DEBUG_PRINTLN("Sent less data than expected!");
		return false;
	}
	dataFile.close();
	return true;
}
#endif
