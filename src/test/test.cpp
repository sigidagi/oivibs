#include <pthread.h> 
#include <iostream>
#include <cstdlib>
#include <vector>

#include <unistd.h>
#include <stdio.h>
#include <dlfcn.h>

using namespace std;

void dlcheck(void *p) {
    if(!p) {
        printf("[MAIN] ERROR: %s\n", dlerror());
        exit(1);
    }
}


int main(int argc, char** argv)
{
    // don't forget to set LD_LIBRARY_PATH to directory where OiVibrations.ndll libary resides
    // LD_LIBRARY_PATH=/home/john/sharedlibs
    // export LD_LIBRARY_PATH
    void* hndl = dlopen("/home/sigis/prg/cplus/OiVibs/OiVibrations/build/src/libOiVibrations.so", RTLD_LAZY);
    dlcheck(hndl);    

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

    typedef void* (*fnc_t)();    
    fnc_t lib_create = (fnc_t)dlsym(hndl, "create");
    
    void* proxy = ((fnc_t)lib_create())();

    dlclose(hndl);

	return 0;
}
