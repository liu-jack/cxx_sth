#ifndef MP_OBJECT_FACTORY_H
#define MP_OBJECT_FACTORY_H
#include <boost/function.hpp>
#include <map>

/// 工厂模式泛型实现.
/// 限制: 生成的对象必须为通过默认构造函数来构造.
/// 当然你也可以扩展这个模板让它支持更多参数的构造函数.
template<typename IdType, typename ObjectType>
class ObjectFactory
{
public:
	/// 表示默认构造函数的函数对象.
	typedef boost::function< ObjectType* () > CreatorType;

	/// 构造函数对应的函数对象的关联容器. 
	typedef std::map<IdType, CreatorType> ObjectCreator_map;

	/// 注册子类对象的构造函数信息.
	void RegisterObjectCreator(const IdType& id, const CreatorType &creator)
	{
		objectCreatorMap_[id] = creator;
	}

	/// 通过默认构造函数在堆上创建一个新的对象实例. 使用new生成.
	ObjectType * MakeObject(const IdType& id)
	{
		ObjectCreator_map::const_iterator iter = objectCreatorMap_.find(id);
		if (iter == objectCreatorMap_.end())
		{
			return NULL;
		}
		else
		{
			return (iter->second)();
		}
	}

private:
	ObjectCreator_map objectCreatorMap_;
};

#endif