#include "OiProxy.h"
#include <iostream>
#include <cstdlib>
#include <vector>

#include <unistd.h>
#include <stdio.h>

using namespace std;


int main(int argc, const char** argv)
{
    // don't forget to set LD_LIBRARY_PATH to directory where libOiVibrations.so libary resides
    // LD_LIBRARY_PATH=/home/john/sharedlibs
    // export LD_LIBRARY_PATH

    cout << "\n";
	if (argc < 2)
	{
		cout << "\n";
        cout << "Requires one or more command arguments.\n";
        cout << " Universal file name : FileName.uff \n";
		exit(1);
	}

    Oi::Proxy proxy;
    bool bSucceess = proxy.init(argc, argv);
    if (bSucceess)
        cout << "Initialization succeeded!\n";
    else
        cout << "Initialization failed!\n";
    
    cout << "\n";
    int i(0), j(0);
    int nrows(0), ncols(0);
    const double* nodes = proxy.getNodes(nrows, ncols); 

    if (nodes != NULL)
    {
        cout << " ---- Nodes ----\n";
        for (i = 0; i < nrows; ++i)
        {
            for (j = 0; j < ncols; ++j)
                cout << nodes[j*nrows + i] << " ";
            cout << "\n";    
        }
        cout << endl;
    }

    const unsigned int* lines = proxy.getLines(nrows, ncols);
    if (lines != NULL)
    {
        cout << " ---- Lines -----\n";

        for (i = 0; i < nrows; ++i)
        {
            for (j = 0; j < ncols; ++j)
                cout << lines[j*nrows + i] << " ";
            cout << "\n";   
        }
        cout << endl;
    }
    
    const unsigned int* surfaces = proxy.getSurfaces(nrows, ncols); 
    if (surfaces != NULL)
    {
        cout << " ---- Surfaces ----\n";
 
        for (i = 0; i < nrows; ++i)
        {
            for (j = 0; j < ncols; ++j)
                cout << surfaces[j*nrows + i] << " ";
            cout << "\n";   
        }
        cout << endl;
        cout << endl;
    }

	return 0;
}
