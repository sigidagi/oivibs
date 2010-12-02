#include "OiProxy.h"
#include <iostream>
#include <cstdlib>
#include <vector>

#include <unistd.h>
#include <stdio.h>

using namespace std;

template<typename T>
void free2D(T** p2Darray, int length)
{
    for (int i = 0; i < length; ++i)
        delete [] p2Darray[i];

    delete [] p2Darray;
    p2Darray = 0;
}


int main(int argc, char** argv)
{
    // don't forget to set LD_LIBRARY_PATH to directory where OiVibrations.ndll libary resides
    // LD_LIBRARY_PATH=/home/john/sharedlibs
    // export LD_LIBRARY_PATH

    cout << "\n";
	if (argc < 2)
	{
		cout << "\n";
		cout << "Requires one command argument - Universal file name : FileName.uff \n";
		exit(1);
	}

    vector<string> fileList;
    int i;

	for (i = 1; i < argc; ++i)
	{
		string strInFile = argv[i];
		string::size_type idx = strInFile.rfind('\\');
		if (idx != string::npos)
		{
			string strPath = strInFile.substr(0, idx);
            
			chdir(strPath.c_str());
			strInFile = strInFile.substr(idx+1);
		}

		fileList.push_back(strInFile);
	}
    
    if (fileList.empty())
        return false;

    Oi::Proxy proxy;
    bool bSucceess = proxy.init(fileList[0]);
    if (bSucceess)
        cout << "Initialization succeeded!\n";
    else
        cout << "Initialization failed!\n";


	return 0;
}
