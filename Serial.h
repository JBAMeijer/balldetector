#pragma once
#include "ComProtocol.h"
#include <termios.h>
#include <iostream>
#include <cstdint>

struct SerialSettings: public ComProtocolType
{
	int32_t baudrate = -1;
	int8_t databits = -1;
	int8_t stopbits = -1;
	int8_t enableParity = -1;

	void operator=(const SerialSettings& newSettings)
	{
	/*
		if (newSettings.baudrate != -1)		this->baudrate = newSettings.baudrate;
		if (newSettings.databits != -1)		this->databits = newSettings.databits;
		if (newSettings.stopbits != -1)		this->stopbits = newSettings.stopbits;
		if (newSettings.enableParity != -1) this->enableParity = newSettings.enableParity;
	*/
		this->baudrate = newSettings.baudrate;
		this->databits = newSettings.databits;
		this->stopbits = newSettings.stopbits;
		this->enableParity = newSettings.enableParity;
	}
};
			
class Serial: public ComProtocol
{
public:
	Serial();

	int open() override;
	void close() override;

    std::string read() override;
	void write(const std::string&) override;

	int ioctl(ComProtocolType) override;
private:
	int setBaudrate(termios&);
	int setDataBits(termios&);
	int setStopBits(termios&);
	int setParity(termios&);

private:
	int fd;
	SerialSettings serialSettings;
};

