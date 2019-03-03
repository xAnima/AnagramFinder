#include "stdafx.h"


CAnagramFinder::CAnagramFinder()
{
	//Reset locale to support UTF-8
	setlocale(LC_ALL, "");
}


CAnagramFinder::~CAnagramFinder()
{
}

void CAnagramFinder::Initialize(char *szArgs[])
{
	//Parse source word
	ParseReference(szArgs[2]);

	//Parse dictionary and compare
	FindReferences(szArgs[1]);
}

void CAnagramFinder::PrintResults(timespec* pTimeSpec)
{
	printf("%.3f,",(double)pTimeSpec->tv_nsec / 1000.0f);
	if (m_vecResult.size() != 0)
	{
		for (uint i = 0; i < m_vecResult.size(); i++)
			printf("%ls%c", to_wstring(m_vecResult[i]).c_str(), i < m_vecResult.size() - 1 ? ',' : '\n');
	}
	else
		printf("No anagrams were found!\n");
}

void CAnagramFinder::ParseReference(char * szRefWord)
{
	//Comparsion will be done on wide characters convereted from UTF-8
	std::wstring wstrRefWord = to_wstring(szRefWord);
	m_uiSourceSize = (uint)wstrRefWord.size();

	for (uint i = 0; i < m_uiSourceSize; i++)
	{
		//Upper case character, so that all of the cases will be addressed and create a map with referencing letter
		wstrRefWord[i] = std::towupper(wstrRefWord[i]);
		m_mapSource[wstrRefWord[i]]++;
	}
}

//Find all of the possible anagrams from the dictionary file
void CAnagramFinder::FindReferences(char * szDictPath)
{
	FILE* pFile = fopen(szDictPath, "rb+");

	if (!pFile)
	{
		printf("Failed to open the dictionary file!\n");
		return;
	}
	char cCharacter;
	char szRefBuffer[MAX_WORD_LENGTH];
	bool bFinished = false;
	uint uiLen = 0;

	while (!bFinished)
	{
		//Read characters one by one into word reference buffer
		if (fread(&cCharacter, sizeof(char), 1, pFile))
		{
			if (cCharacter != '\r')
			{
				szRefBuffer[uiLen] = cCharacter;
				uiLen++;
				szRefBuffer[uiLen] = '\0';
			}
			else
			{
				//Read the \n character, as of windows uses CR LF line separation
				fread(&cCharacter, sizeof(char), 1, pFile);

				//Check only words, that are of the similar size
				if (uiLen == m_uiSourceSize)
				{
					//Convertation will be from CP1257 sourced dictionary file to UTF-8
					iconv_t icnvObj = iconv_open("UTF-8", "CP1257");
					if (icnvObj == (iconv_t)-1)
					{
						printf("iconv_open failed!");
						return;
					}

					char *szInputBuffer = &szRefBuffer[0];
					size_t sizeInLeft = uiLen;

					char szRefOutput[MAX_WORD_LENGTH];
					char *szOutputBuffer = &szRefOutput[0];
					size_t sizeOutLeft = MAX_WORD_LENGTH - 1;

					do {
						if (iconv(icnvObj, &szInputBuffer, &sizeInLeft, &szOutputBuffer, &sizeOutLeft) == (size_t)-1) {
							printf("Failed on %s > %d - %d!\n", szRefOutput, sizeInLeft, sizeOutLeft);
							return;
						}
					} while (sizeInLeft > 0 && sizeOutLeft > 0);
					*szOutputBuffer = 0;

					std::string strTemp(szRefOutput);
					std::wstring wstrRefWord = to_wstring(strTemp);
					std::map<wchar_t, int> mapReference;
					for (uint i = 0; i < wstrRefWord.size(); i++)
					{
						wstrRefWord[i] = std::towupper(wstrRefWord[i]);
						mapReference[wstrRefWord[i]]++;
					}

					//Compare maps for equality
					if (CompareMap(m_mapSource, mapReference))
						m_vecResult.push_back(szRefOutput);

					iconv_close(icnvObj);
				}

				//Clear loop variables
				memset(szRefBuffer, 0, uiLen);
				uiLen = 0;
			}
		}
		else
			bFinished = true;
	}

	fclose(pFile);
}