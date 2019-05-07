#ifndef __RND_GEN_CORE_TYPES_H__
#define __RND_GEN_CORE_TYPES_H__

#include "core.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#ifdef _DEBUG
#define dprintf(...) printf(__VA_ARGS__);
#else
#define dprintf(...) ;
#endif

namespace rnd_gen {
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

#endif // !__RND_GEN_CORE_TYPES_H__