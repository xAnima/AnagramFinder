#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <string.h>

#include <locale>
#include <codecvt>
#include <iconv.h>

#define MAX_WORD_LENGTH 255

// Convert UTF-8 string to wide string
inline std::wstring to_wstring(std::string const& strRef) {
	std::wstring_convert<std::codecvt_utf8<wchar_t> > objConv;
	return objConv.from_bytes(strRef);
}

template <typename Map>
bool CompareMap(Map const &lhs, Map const &rhs)
{
	if (lhs.size() == rhs.size())
		return  std::equal(lhs.begin(), lhs.end(), rhs.begin());
	else
		return false;
}

#include "AnagramFinder.h"

