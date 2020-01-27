#pragma once

#include <string>
#include <string_view>

struct ComProtocolType
{
};


class ComProtocol
{
public:
	ComProtocol() {}
	virtual ~ComProtocol() {}
	
	virtual int open() = 0;
	virtual void close() = 0;
	
	virtual std::string read() = 0;
	virtual void write(const std::string&) = 0;
	
	virtual int ioctl(ComProtocolType) = 0;
};

