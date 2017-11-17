#ifndef __DATA_DATASTRUCT_COMBAT_H__
#define __DATA_DATASTRUCT_COMBAT_H__

#pragma once

#include "struct_base.h"
#include <string>

///
/// Data Format
/// uint32			'u'
/// int32			'i'
/// float			'f'
/// string			's'
/// char			'c'
/// ignore			'x'
/// index string	'k'
///

// set byte alignment
#pragma pack(push,1)

///
/// CombatWindow
///


///
/// CombatWindowComment: Editor Use Only
///
//struct CombatWindowComment
//{
//public:
//	CombatWindowComment();
//
//	DECLARE_EXTRA_SIZE_ZERO
//
//	static const bool HasIndex() {
//		return true;
//	}
//	const uint32 GetKey() const {
//		return window_id;
//	}
//	static const char* GetKeyName() {
//		return "window_id";
//	}
//	static const char* GetTableName() {
//		return "c_combat_window_comment";
//	}
//	static const char* GetFormat() {
//		return 
//			"u"		// window_id
//			"s";	// comment
//	}
//
//	uint32	window_id;
//	std::string	comment;
//};

///
/// CameraShakeArgs
///

///
/// CombatScheme
///
struct CombatSchemePair
{
public:
    CombatSchemePair();

	DECLARE_EXTRA_SIZE_ZERO

    static const bool HasIndex() {
		return true;
	}
	const uint32 GetKey() const {
		return id;
	}
	static const char* GetKeyName() {
		return "id";
	}
	static const char* GetTableName() {
		return "combat_scheme";
	}
	static const char* GetFormat() {
		return
			"u"		// id
            "s";     // comment
	}

	uint32	id;
    std::string comment;
};

///
/// CombatWindowComment: Editor Use Only
///
//struct CombatSchemeComment
//{
//public:
//	CombatSchemeComment();
//
//	DECLARE_EXTRA_SIZE_ZERO
//
//	static const bool HasIndex() {
//		return true;
//	}
//	const uint32 GetKey() const {
//		return scheme_id;
//	}
//	static const char* GetKeyName() {
//		return "scheme_id";
//	}
//	static const char* GetTableName() {
//		return "c_combat_scheme_comment";
//	}
//	static const char* GetFormat() {
//		return 
//			"u"		// scheme_id
//			"s";	// comment
//	}
//
//	uint32	scheme_id;
//	std::string	comment;
//};

#pragma pack(pop)

#endif // __DATA_DATASTRUCT_COMBAT_H__
