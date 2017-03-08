/*
 * Logger.cpp
 *
 *  Created on: 3 mars 2017
 *      Author: Bruno
 */

#include <Logger.h>

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::send(String msg) {
	if (!printer) {
		printer->print(msg);
	}
}
void Logger::sendln(String msg) {
	if (!printer) {
		printer->println(msg);
	}
}

void Logger::setPrinter(Print*prter) {
	printer = prter;
	sendln("Je suis le logger, coucou");
}
