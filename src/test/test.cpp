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

    int i;
    int nodeLength = 0;
    double** nodes = proxy.getNodes(nodeLength); 

    if (nodes != NULL)
    {
        cout << " ---- Nodes ----\n";
        for (i = 0; i < nodeLength; ++i)
            cout << nodes[i][0] << " " << nodes[i][1] << " " << nodes[i][2] << "\n";
    
        cout << endl;

        free2D(nodes, nodeLength);
    }

    int lineLength = 0;
    unsigned int** lines = proxy.getLines(lineLength);
    if (lines != NULL)
    {
        cout << " ---- Lines -----\n";
        for (i = 0; i < lineLength; ++i)
            cout << lines[i][0] << " " << lines[i][1] << "\n";
        
        cout << endl;
        free2D(lines, lineLength);
    }
    
    int surfaceLength = 0;
    unsigned int** surfaces = proxy.getSurfaces(surfaceLength); 
    if (surfaces != NULL)
    {
        cout << " ---- Surfaces ----\n";
        for (i = 0; i < surfaceLength; ++i)
            cout << surfaces[i][0] << " " << surfaces[i][1] << " " << surfaces[i][2] << "\n";
    
        cout << endl;

        free2D(surfaces, surfaceLength);
    }

    

	return 0;
}
