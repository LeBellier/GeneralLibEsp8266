#ifndef ASPECT_H
#define ASPECT_H

// Uncomment to print debugging info to console attached to ESP8266
//#define DEBUG_ESP
//#define DEBUG_INIT

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DEBUG_ESP
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) { }
#endif

// Setup debug init printing macros.
#ifdef DEBUG_INIT
#define DEBUG_INIT_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_INIT_PRINTLN(...) {DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_INIT_PRINT(...) {}
#define DEBUG_INIT_PRINTLN(...) {}
#endif

#endif
