#ifndef PAGESWEB_H
#define PAGESWEB_H

// Prototypage
#include <Aspect.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266FtpServer.h>

void initDNS();
void initWebServer();
void initFtpServer();
void updateServers();

void handleNotFound();
bool loadAnimation(String sPath);
bool loadFromSpiffs(String path);

MDNSResponder mdns;
ESP8266WebServer server(80);
FtpServer ftpSrv; //set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial

int showType = 0;
bool animate = false;  // Web Button

// Déclaration
void initDNS() {
	if (mdns.begin(dnsName, WiFi.localIP())) {
		DEBUG_INIT_PRINTLN("MDNS responder started");
		DEBUG_INIT_PRINT("You can use the name: http://");
		DEBUG_INIT_PRINT(dnsName);
		DEBUG_INIT_PRINTLN(".local/");
	}
}
void initWebServer() {
	SPIFFS.begin(); //start spiffs (spi file system)
	server.onNotFound(handleNotFound);
	server.begin();
	DEBUG_INIT_PRINTLN("HTTP server started");
}
void initFtpServer() {
	if (SPIFFS.begin()) {
		DEBUG_INIT_PRINTLN("SPIFFS opened!");
		ftpSrv.begin("esp8266", "esp8266"); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
		DEBUG_INIT_PRINTLN("FTP server started; MdP:esp8266, User:esp8266");
	}
}
void updateServers() {
	mdns.update();
	ftpSrv.handleFTP();
	server.handleClient();
}

void handleNotFound() {
	String uriAsked = server.uri();
	DEBUG_PRINTLN(uriAsked);

	//check the request
	if (!loadFromSpiffs(server.uri()) && !loadAnimation(server.uri())) { // no file at the uri found
		String message = "File Not Detected  ";
		message += "URI: ";
		message += server.uri();
		message += "\n  Method: ";
		message += (server.method() == HTTP_GET) ? "GET" : "POST";
		message += "\n  Arguments: ";
		message += server.args();
		for (uint8_t i = 0; i < server.args(); i++) {
			message += " NAME:" + server.argName(i) + "\n VALUE:"
					+ server.arg(i) + "\n";
		}
		server.send(404, "text/plain", message);
		DEBUG_PRINTLN(message);
	}
}
bool loadAnimation(String sPath) {
	if (sPath.startsWith("/pixel_animation=")) {
		showType = sPath.substring(17).toInt();
		animate = true;
		server.send(200);
		return true;
	} else if (sPath.startsWith("/pixel_random=")) {
		String chiffres = "0123456789ABCDEF";
		sPath = sPath.substring(14);
		int ledNb = chiffres.indexOf(sPath.charAt(0)) * 16
				+ chiffres.indexOf(sPath.charAt(1));
		int r = chiffres.indexOf(sPath.charAt(2)) * 16
				+ chiffres.indexOf(sPath.charAt(3));
		int g = chiffres.indexOf(sPath.charAt(4)) * 16
				+ chiffres.indexOf(sPath.charAt(5));
		int b = chiffres.indexOf(sPath.charAt(6)) * 16
				+ chiffres.indexOf(sPath.charAt(7));
		if (ledNb == 156) {
			strip.setColor(r, g, b);
		} else {
			strip.setMatrixPixelColor(ledNb, r, g, b);
			strip.show();
		}
		String response = "";
		response.concat(sPath.substring(0, 2));
		response.concat('#');
		response.concat(sPath.substring(2, 8));

		DEBUG_PRINT("Show color: ");
		DEBUG_PRINT(r);
		DEBUG_PRINT(" ");
		DEBUG_PRINT(g);
		DEBUG_PRINT(" ");
		DEBUG_PRINT(b);
		DEBUG_PRINT(" Response:");
		DEBUG_PRINT(response);
		server.send(200, "text/plain", response);
		return true;
	} else {
		return false;
	}
}

bool loadFromSpiffs(String path) {

	String dataType = "text/plain";
	if (path.endsWith("/"))
		path += "index.html";

	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));
	else if (path.endsWith(".html"))
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

	if (server.hasArg("download"))
		dataType = "application/octet-stream";
	if (server.streamFile(dataFile, dataType) != dataFile.size()) {
		DEBUG_PRINTLN("Sent less data than expected!");
		return false;
	}
	dataFile.close();
	return true;
}
#endif
