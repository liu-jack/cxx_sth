#include "SimpleSingleton.h"
#include "IScriptHost.h"

class DummyScriptHost : public SimpleSingleton<DummyScriptHost>, public IScriptHost
{
	friend class SimpleSingleton<DummyScriptHost>;

private:
	DummyScriptHost(){}
	~DummyScriptHost(){}
};