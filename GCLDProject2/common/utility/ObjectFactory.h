#ifndef MP_OBJECT_FACTORY_H
#define MP_OBJECT_FACTORY_H
#include <boost/function.hpp>
#include <map>

/// ����ģʽ����ʵ��.
/// ����: ���ɵĶ������Ϊͨ��Ĭ�Ϲ��캯��������.
/// ��Ȼ��Ҳ������չ���ģ������֧�ָ�������Ĺ��캯��.
template<typename IdType, typename ObjectType>
class ObjectFactory
{
public:
	/// ��ʾĬ�Ϲ��캯���ĺ�������.
	typedef boost::function< ObjectType* () > CreatorType;

	/// ���캯����Ӧ�ĺ�������Ĺ�������. 
	typedef std::map<IdType, CreatorType> ObjectCreator_map;

	/// ע���������Ĺ��캯����Ϣ.
	void RegisterObjectCreator(const IdType& id, const CreatorType &creator)
	{
		objectCreatorMap_[id] = creator;
	}

	/// ͨ��Ĭ�Ϲ��캯���ڶ��ϴ���һ���µĶ���ʵ��. ʹ��new����.
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