#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>

Serial::Serial() {}

int Serial::open()
{
	serialSettings.databits = 8;
	serialSettings.enableParity = 1;
	serialSettings.stopbits = 1;
	serialSettings.baudrate = 115200;

	if ((fd = serialOpen("/dev/serial0", 115200)) < 0)
	{
		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		return -1;
	}
	
	if (wiringPiSetup() == -1)
	{
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

void Serial::close()
{
	serialClose(fd);
}

std::string Serial::read()
{
	std::string str;
	int dataAvailable = serialDataAvail(fd);
	
	while (dataAvailable)
	{
		int Character = serialGetchar(fd);
		if (Character == -1)
		{
			str.clear();
			str.append(":(ERROR):");
			break;
		}

		std::cout << Character << '\n';

		str.append(std::to_string(Character));
	}

	return str;
}

void Serial::write(const std::string& string)
{
	for (std::string::const_iterator it = string.begin(); it != string.end(); it++)
	{
		serialPutchar(fd, *it);
	}
}

int Serial::ioctl(ComProtocolType type)
{
	SerialSettings& newsettings = static_cast<SerialSettings&>(type);
	serialSettings = newsettings;
	struct termios options;

	tcgetattr(fd, &options);
	// Set Baudrate
	setBaudrate(options);
	// Set DataBits :Not implemented:
	setDataBits(options);
	// Set StopBits :Not implemented:
	setStopBits(options);
	// Set Parity :Not implemented:
	setParity(options);

	return 0;
}

int Serial::setBaudrate(termios &t)
{
	switch (serialSettings.baudrate)
	{
	case 50:
		t.c_ispeed = B50;
		break;
	case 75:
		t.c_ispeed = B75;
		break;
	case 110:
		t.c_ispeed = B110;
		break;
	case 134:
		t.c_ispeed = B134;
		break;
	case 150:
		t.c_ispeed = B150;
		break;
	case 200:
		t.c_ispeed = B200;
		break;
	case 300:
		t.c_ispeed = B300;
		break;
	case 600:
		t.c_ispeed = B600;
		break;
	case 1200:
		t.c_ispeed = B1200;
		break;
	case 1800:
		t.c_ispeed = B1800;
		break;
	case 2400:
		t.c_ispeed = B2400;
		break;
	case 4800:
		t.c_ispeed = B4800;
		break;
	case 9600:
		t.c_ispeed = B9600;
		break;
	case 19200:
		t.c_ispeed = B19200;
		break;
	case 38400:
		t.c_ispeed = B38400;
		break;
	case 115200:
		t.c_ispeed = B115200;
		break;
	default:
		return -1;
		break;
	}
	return 0;
}

int Serial::setDataBits(termios &t)
{
	return 0;
}

int Serial::setStopBits(termios &t)
{
	return 0;
}

int Serial::setParity(termios &t)
{
	return 0;
}


