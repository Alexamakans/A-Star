#pragma once

class ISurface
{
public:
	virtual void Clear() = 0;
	virtual void Present() = 0;
};