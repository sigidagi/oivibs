#include	"geometry.h"

#include    "OiProxy.h"
#include    <cstdlib>
#include    <vector>
#include	<algorithm>
#include	<iterator>
#include	<sstream>
#include	"gnuplot_i.hpp"

#include    <unistd.h>
#include    <cstdio>
#include	<memory.h>

using namespace std;

void wait_for_key();

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
    {
        cout << "Initialization failed!\n";
        exit(1);
    }
    cout << "\n";

    int i(0);
    int nrows(0), ncols(0);
    const double* pnodes = proxy.getNodes(nrows, ncols); 
    
    vector<Point> nodesCollection;
    nodesCollection.reserve(nrows);

    Point pt1, pt2, pt3; 
    if (pnodes != NULL && ncols == 3)
    {
        for (i = 0; i < nrows; ++i)
        {
            Point pt; 
            pt.x = pnodes[0*nrows + i];
            pt.y = pnodes[1*nrows + i];             
            pt.z = pnodes[2*nrows + i];

            nodesCollection.push_back(pt);
        }
    }

    const unsigned int* plines = proxy.getLines(nrows, ncols);
    
    vector<Line> linesCollection;
    linesCollection.reserve(nrows);

    if (plines != NULL && ncols == 2 && !nodesCollection.empty())
    {
        unsigned int idx1(0), idx2(0);
        for (i = 0; i < nrows; ++i)
        {
            Line line;
            idx1 = plines[0*nrows + i];
            idx2 = plines[1*nrows + i];
            
            // number of node (point) starts from 1, where the indexes of vector<Point> 
            // starts from 0.
            line.point1 = nodesCollection[idx1-1]; 
            line.point2 = nodesCollection[idx2-1]; 

            linesCollection.push_back(line);
        }
    }
    
    const unsigned int* psurfaces = proxy.getSurfaces(nrows, ncols); 
    
    vector<Surface> surfacesCollection;
    surfacesCollection.reserve(nrows);

    if (psurfaces != NULL && ncols == 3)
    {
        unsigned int idx1(0), idx2(0), idx3(0);
        for (i = 0; i < nrows; ++i)
        {
            Surface surface;
            idx1 = psurfaces[0*nrows + i];
            idx2 = psurfaces[1*nrows + i];
            idx3 = psurfaces[2*nrows + i];
             
            surface.point1 = nodesCollection[idx1-1]; 
            surface.point2 = nodesCollection[idx2-1]; 
            surface.point3 = nodesCollection[idx3-1]; 

            surfacesCollection.push_back(surface);
        }
    }
    
/*
 *    std::cout << "------ Nodes ---------\n";
 *    std::ostream_iterator<Point> os_pit( std::cout, "\n" );
 *    std::copy(nodesCollection.begin(), nodesCollection.end(), os_pit);
 *    std::cout << std::endl;
 *
 *    std::cout << "------- Lines --------\n";
 *    std::ostream_iterator<Line> os_it( std::cout, "\n" );
 *    std::copy(linesCollection.begin(), linesCollection.end(), os_it); 
 *    std::cout << std::endl; 
 */
   
    vector<double> singularValues;

    const double* pvalues = 0;
    for ( i = 1; i < argc; ++i)
    {
        string fileName = argv[i];
        pvalues = proxy.getSingularValues(fileName, nrows, ncols);
        if (pvalues != 0)
        {
            try 
            {
                Gnuplot gplot("Singular values");
                gplot.set_title("singular values");
                gplot.set_grid();
                gplot.set_style("steps");
                std::stringstream ss;
                for (int j = 0; j < ncols; ++j)
                {
                    ss << "singular values " << j; 
                    gplot.plot_x(pvalues+j*nrows, pvalues+(j+1)*nrows-1, ss.str() );                   
                    ss.str("");
                    ss.clear();
                }
                
                wait_for_key();
            }
            catch(GnuplotException& e)
            {
                std::cout << e.what() << "\n";
            }

        }
    }
    
	return 0;
}

void wait_for_key()
{
    std::cout << "\n" << "Press enter to continue...\n";
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
}
