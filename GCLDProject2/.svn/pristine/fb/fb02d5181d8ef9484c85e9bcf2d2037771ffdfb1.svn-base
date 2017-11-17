#ifndef _SERVER_FRAMEWORK_SINGLETON_H_
#define _SERVER_FRAMEWORK_SINGLETON_H_

template <class T>
class SimpleSingleton
{
protected:
	SimpleSingleton(){}
	virtual ~SimpleSingleton(){}
	SimpleSingleton(const SimpleSingleton&);

public:
	static T& Instance()
	{
		static T s_instance;
		return s_instance;
	}
};

#endif // _SERVER_FRAMEWORK_SINGLETON_H_
