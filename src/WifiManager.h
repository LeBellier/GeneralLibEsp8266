#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H
// Prototypage

#include <ESP8266WiFi.h>
#include <ESP8266WiFiScan.h>
#include <Aspect.h>
void initWIFI(uint8_t nbSSID, String* SSIDs, String* passewords); //Connection if it's possible

// Déclaration
void initWIFI(uint8_t nbSSID, String* SSIDs, String* passewords) {
	String ssid;
	String password;

	boolean wifiFounded = false;
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.enableSTA(true);
	WiFi.disconnect();
	delay(100);
	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	if (n != 0) {
		int j = 0;
		while (j < nbSSID && !wifiFounded) {
			ssid = SSIDs[j];
			DEBUG_INIT_PRINT("Correspond-il à ");
			DEBUG_INIT_PRINTLN(ssid);
			int i = 0;
			while (i < n && !wifiFounded) {
				DEBUG_INIT_PRINT("Wifi detecté: ");
				DEBUG_INIT_PRINTLN(((ESP8266WiFiScanClass ) WiFi).SSID(i));

				if (((ESP8266WiFiScanClass) WiFi).SSID(i) == ssid) {
					ssid = SSIDs[j];
					password = passewords[j];
					wifiFounded = true;
				}
				i++;
			}
			j++;
		}

		if (wifiFounded) {
			char *cSsid = &ssid[0];
			char *cPassword = &password[0u];
			WiFi.begin(cSsid, cPassword);
			free(cSsid);
			free(cPassword);

			DEBUG_INIT_PRINTLN("");
			// Wait for connection
			while (WiFi.status() != WL_CONNECTED) {
				delay(500);
				DEBUG_INIT_PRINT(".");
			}
			String sIP = WiFi.localIP().toString();
			DEBUG_INIT_PRINTLN("");
			DEBUG_INIT_PRINT("Connected to ");
			DEBUG_INIT_PRINTLN(ssid);
			DEBUG_INIT_PRINT("IP address: ");
			DEBUG_INIT_PRINTLN(sIP);
		} else {
			const char *ssid = "ESP-MPU";
			//const char *password = "thereisnospoon";

			/* You can remove/add the password parameter if you want the AP to be open/secure. */
			//	WiFi.softAP(ssid, password);
			WiFi.enableAP(true);
			WiFi.softAP(ssid);
			IPAddress ip(192, 168, 4, 1); // this 3 lines for a fix IP-address
			IPAddress gateway(192, 168, 4, 1);
			IPAddress subnet(255, 255, 255, 0);

			WiFi.softAPConfig(ip, gateway, subnet);

		}
	}
}
#endif
