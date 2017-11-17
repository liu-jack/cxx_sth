/*
 Win32 Debug support
*/
#ifndef MMO_ASSERT_H__
#define MMO_ASSERT_H__



#ifdef _DEBUG
	#ifdef WIN32
	#include <crtdbg.h>
	#define WIN32_ASSERT(_Exp) if ( !( _Exp ) ) {if ( _CrtDbgReport(_CRT_ASSERT, __FILE__ , __LINE__, __FUNCTION__ , "" ) == 1 ) { _asm { int 3} } };
	#define ASSERT WIN32_ASSERT
	#define MASSERT(_Exp, _Msg) if ( !( _Exp ) ) {if ( _CrtDbgReport(_CRT_ASSERT, __FILE__ , __LINE__, __FUNCTION__ , _Msg ) == 1 ) { _asm { int 3} } };
	#else
	#include <assert.h>
	#define ASSERT assert 
	#define MASSERT( _Exp, _Msg ) if( !( _Exp ) ) { printf( "%s", (_Msg) ) ; assert( (_Exp) ) ;  }
	/*#define ASSERT(_Exp) if( _Exp ){}
	#define MASSERT( _Exp, _Msg ) if( _Exp ){printf( "%s", (_Msg) );}*/
	#endif
#else
	#include <assert.h>
	#define ASSERT assert
	#define MASSERT( _Exp, _Msg ) if( !( _Exp ) ) { printf( "%s", (_Msg) ) ; assert( (_Exp) ) ;  }
	//#define ASSERT(_Exp) if( _Exp ){}
	//#define MASSERT( _Exp, _Msg ) if( _Exp ){printf( "%s", (_Msg) );}
#endif


#define MMO_ASSERT ASSERT

#endif
