#pragma once
#ifndef __XML_WRAPPER_TYPEDEFS_H__
#define __XML_WRAPPER_TYPEDEFS_H__

#include "xml-wrapper.h"

#include <string>
#include <locale>
#include <vector>
#include <map>
#include <unordered_map>

#ifdef _DEBUG
#define dprintf(...) printf(__VA_ARGS__);
#else
#define dprintf(...) ;
#endif

namespace xml {
	typedef std::string string;
	template<class T>
	using vector = std::vector<T>;
	template<class K, class I>
	using map = std::map<K, I>;
	template<class K, class I>
	using hashmap = std::unordered_map<K, I>;

	typedef struct {
		string name;
		string value;
	} attribute;
}

#endif // !__XML_WRAPPER_TYPEDEFS_H__
