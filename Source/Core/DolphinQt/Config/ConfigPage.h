#pragma once

class ConfigPage
{
public:
	virtual void LoadFromConfig() = 0;
	virtual void Refresh() = 0;
	virtual ~ConfigPage() {}
};