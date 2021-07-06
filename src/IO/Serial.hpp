#pragma once

#include <stdint.h>

#define SERIAL_COM1 0x3F8

// Initialises serial port for logging. Returns 0 if initialization successful, else -1.
int InitializeSerialPort(uint16_t port);

// Sends byte to serial port.
void SerialOut(uint16_t port, uint8_t byte);