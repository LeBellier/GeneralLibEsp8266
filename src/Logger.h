/*
 * Logger.h
 *
 *  Created on: 3 mars 2017
 *      Author: Bruno
 */

#ifndef LIBRARIES_GENERALLIBESP8266_SRC_LOGGER_H_
#define LIBRARIES_GENERALLIBESP8266_SRC_LOGGER_H_

#include <Arduino.h>
#include <Print.h>

class Logger {
public:
	Logger();
	virtual ~Logger();

	void send(String);
	void sendln(String);

	void setPrinter(Print*);

private:

	Print* printer = 0;
};

#endif /* LIBRARIES_GENERALLIBESP8266_SRC_LOGGER_H_ */
