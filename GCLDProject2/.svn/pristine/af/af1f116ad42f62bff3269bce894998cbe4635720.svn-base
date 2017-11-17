#ifndef ModMarker_h__
#define ModMarker_h__

#include <set>

template< typename T>
class ModMarker : private std::set< T>
{
public:
    typedef std::set< T> ModifyIDs;
    typedef typename std::set< T>::const_iterator ModifyIDsCIter;

    bool HasModified() const
    {
        return !std::set<T>::empty();
    }

    void SetUnmodified()
    {
        std::set<T>::clear();
    }
    void SetModifiedID(const T& id)
    {
        std::set<T>::insert( id);
    }
    void SetIDUnModified(const T& id)
    {
        std::set<T>::erase( id);
    }
    bool IsIDModified(const T& id) const
    {
        return std::set<T>::find( id) != std::set<T>::end();
    }

    ModifyIDsCIter ModIdBegin() const { return std::set<T>::begin();}
    ModifyIDsCIter ModIdEnd() const { return std::set<T>::end();}
};


#endif // ModMarker_h__
