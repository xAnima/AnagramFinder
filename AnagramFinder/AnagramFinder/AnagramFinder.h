#ifndef __ANAGRAM_FINDER_H
#define __ANAGRAM_FINDER_H

#pragma once

class CAnagramFinder
{
public:
	CAnagramFinder();
	~CAnagramFinder();

	void Initialize(char *szArgs[]);

	void PrintResults(timespec* pSec);
private:
	void ParseReference(char *szRefWord);
	void FindReferences(char *szDictPath);

protected:
	std::map<wchar_t, int> m_mapSource;
	uint m_uiSourceSize;


	std::locale m_localeDefault;
	std::vector<std::string> m_vecResult;
};

#endif //__ANAGRAM_FINDER_H