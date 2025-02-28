#pragma once
class CFileMgr
{
	SINGLE(CFileMgr);

private:
	void SearchFolder(const wstring& folderPath);
	void ProcessFile(const wstring& filePath);
	wstring GetFileNameWithoutExtension(const wstring& fileName);
public:

public:
	void init(const wstring& folderPath);

};

