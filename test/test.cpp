#include <iostream>
#include <cstdlib>
#include <vector>

#include <unistd.h>
#include <armadillo>
#include <stdio.h>
#include <dlfcn.h>
#include <neko_vm.h>


using namespace std;

int main(int argc, char* argv[])
{
    // don't forget to set LD_LIBRARY_PATH to directory where OiVibrations.ndll libary resides
    // LD_LIBRARY_PATH=/home/john/sharedlibs
    // export LD_LIBRARY_PATH
    void* hndl = dlopen("OiVibrations.ndll", RTLD_LAZY);
    if (hndl == NULL)
    {
        cerr << dlerror() << endl;
        exit(-1);
    }

    cout << "\n";
	if (argc < 2)
	{
		cout << "\n";
		cout << "Requires one command argument - Universal file name : FileName.uff \n";
		return false;
	}

	// If first argument is full path plus file name, go to that directory and extract only file name.
	vector<string> vstrFiles;
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

		vstrFiles.push_back(strInFile);
	}

    neko_global_init();
    value arr = alloc_array((int)vstrFiles.size());    
    for (i = 0; i < (int)vstrFiles.size(); ++i)
        val_array_ptr(arr)[i] = alloc_string(vstrFiles[i].c_str());

    
    typedef value (*fnc_init_ptr)(value);
    fnc_init_ptr init = (fnc_init_ptr)(dlsym(hndl, "init__1"));
    value v = ((fnc_init_ptr)init(arr))(arr); 

    if( val_is_bool(v) )
        printf("Init success : %s",val_bool(v)?"true":"false");
    else
       exit(-1); 
 
    typedef value (*fnc_ptr_0)();
    fnc_ptr_0 lines = (fnc_ptr_0)(dlsym(hndl, "getLines__0"));
    value vlines = ((fnc_ptr_0)lines())();
    float x, y, z;

    if (val_is_array(vlines))
    {
        int lsize = val_array_size(vlines);
        for (i = 0; i < lsize; ++i)
        {
            value obj = val_array_ptr(vlines)[i];
            if (!val_is_object(obj))
                continue;

            x = val_float(val_field(obj, val_id("x")));            
            y = val_float(val_field(obj, val_id("y")));            
            z = val_float(val_field(obj, val_id("z")));            
            printf("%f, %f, %f\n", x, y, z);
        }
    }

    fnc_ptr_0 nodes = (fnc_ptr_0)dlsym(hndl, "getNumberOfNodes__0");
    value vnodes = ((fnc_ptr_0)nodes())();
    if (val_is_int(vnodes))
    {
        printf("number of nodes: %i\n", val_int(vnodes));
    }

    neko_global_free();
    dlclose(hndl);

	return 0;
}
