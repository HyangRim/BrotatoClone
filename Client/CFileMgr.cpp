#include "pch.h"
#include "CFileMgr.h"
#include "CPathMgr.h"
#include "CSoundMgr.h"
#include "Direct2DMgr.h"

CFileMgr::CFileMgr()
{

}

CFileMgr::~CFileMgr()
{

}

void CFileMgr::init(const wstring& folderPath)
{
    // 주어진 폴더 경로에서 탐색 시작
    SearchFolder(folderPath);  
}

void CFileMgr::SearchFolder(const wstring& folderPath)
{
    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // 폴더 내 모든 파일 및 디렉터리를 검색하기 위한 경로
   wstring searchPath = folderPath + L"\\*";

    // 첫 번째 파일/디렉터리 검색
    hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        wprintf(L"Failed to open directory: %s\n", folderPath.c_str());
        return;
    }

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0)
            {
                // 하위 디렉터리를 재귀적으로 탐색
                wstring subFolderPath = folderPath + L"\\" + findData.cFileName;
                SearchFolder(subFolderPath);
            }
        }
        else
        {
            // 파일 경로 생성
            std::wstring fileName = findData.cFileName;
            std::wstring filePath = folderPath + L"\\" + fileName;

            // 파일 처리
            ProcessFile(filePath);
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}

void CFileMgr::ProcessFile(const std::wstring& filePath)
{
    // 파일 확장자 확인
    size_t lastSlashPos = filePath.find_last_of(L"\\/");
    wstring fileName = (lastSlashPos == wstring::npos) ? filePath : filePath.substr(lastSlashPos + 1);
    wstring extension = fileName.substr(fileName.find_last_of(L'.') + 1);

    // PNG 파일 처리
    const wchar_t* contentPath = CPathMgr::GetInstance()->GetContentPath();
    wstring relativePath = CPathMgr::GetInstance()->GetRelativePath(filePath.c_str());
    // 확장자에 따라 처리
    if (extension == L"png")
    {
        wstring tag = GetFileNameWithoutExtension(fileName);

        Direct2DMgr::GetInstance()->LoadAndStoreBitmap(relativePath.c_str(), tag.c_str(), false);

        wprintf(L"Loaded PNG: %s\n", tag.c_str());
    }
    else if (extension == L"mp3" || extension == L"wav")
    {
        // MP3 또는 WAV 파일 처리
        wstring key = GetFileNameWithoutExtension(fileName);

        CSoundMgr::GetInstance()->AddSound(key, relativePath.c_str(), false, false);

        wprintf(L"Loaded sound: %s\n", key.c_str());
    }
}

wstring CFileMgr::GetFileNameWithoutExtension(const wstring& fileName)
{
    size_t dotPos = fileName.find_last_of(L'.');
    if (dotPos != wstring::npos)
    {
        return fileName.substr(0, dotPos); // 확장자를 제외한 파일 이름 반환
    }
    return fileName; // 확장자가 없는 경우 그대로 반환
}