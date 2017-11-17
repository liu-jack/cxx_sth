#ifndef HandlerArray_h__
#define HandlerArray_h__
#include <assert.h>
#include <vector>

template< typename NetPackHandler>
class HandlerArray
{
public:
    HandlerArray( int size)
        : m_handlers( size)
    { }

    bool RegistHandler( int OpCode , const NetPackHandler& handle)
    {
        assert( static_cast<size_t>( OpCode  ) < m_handlers.size());
        if ( m_handlers[ OpCode])
        {
            return false;
        }
        else
        {
            m_handlers[ OpCode] = handle; 
            return true;
        }
    }

    inline const NetPackHandler& GetHandler( int Opcode)
    {
        return m_handlers[Opcode];    
    }
private:
    typedef std::vector< NetPackHandler> Handlers;
private:
    Handlers m_handlers;
};

#endif // HandlerArray_h__
