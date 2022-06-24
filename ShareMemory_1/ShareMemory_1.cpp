// ShareMemory_1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

wchar_t* c2w(const char* str)
{
    int length = strlen(str) + 1;
    wchar_t* t = (wchar_t*)malloc(sizeof(wchar_t) * length);
    memset(t, 0, length * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, str, strlen(str), t, length);
    return t;
}

//两次运行程序即可实现写入读出同一块内存的内容
int main()
{
    string strMapName("ShareMemory");                // 内存映射对象名称
    string strComData("This is common data!");        // 共享内存中的数据
    LPCWSTR wStrMapName = c2w(strMapName.c_str());     // 转成宽字符 = =
    LPVOID pBuffer;                                    // 共享内存指针

    // 首先试图打开一个命名的内存映射文件对象
    HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, wStrMapName);
    if (NULL == hMap)
    {    // 打开失败，创建之
        hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            strComData.length() + 1,
            wStrMapName);
        // 映射对象的一个视图，得到指向共享内存的指针，设置里面的数据
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        strcpy((char*)pBuffer, strComData.c_str());
        cout << "写入共享内存数据：" << (char*)pBuffer << endl;
    }
    else
    {    // 打开成功，映射对象的一个视图，得到指向共享内存的指针，显示出里面的数据
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        cout << "读取共享内存数据：" << (char*)pBuffer << endl;
    }

    getchar();            // 注意，进程关闭后，所有句柄自动关闭，所以要在这里暂停

    // 解除文件映射，关闭内存映射文件对象句柄
    ::UnmapViewOfFile(pBuffer);
    ::CloseHandle(hMap);
    system("pause");
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
