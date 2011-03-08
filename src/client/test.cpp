#include	"geometry.h"

#include    "OiProxy.h"
#include    <cstdlib>
#include    <vector>
#include	<algorithm>
#include	<iterator>
#include	<sstream>
#include	<complex>
#include	"gnuplot_i.hpp"

#include    <cstdio>

using namespace std;

void wait_for_key();
void delete_plot(Gnuplot*);

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

    int length(0);
    
    int numberOfMeasurements = proxy.getNumberOfMeasurements();    
    vector<const double*> pvalues(numberOfMeasurements);
    vector<const double*> pfreq(numberOfMeasurements);
    vector<Gnuplot*> plots;

    for (i = 0; i < numberOfMeasurements; ++i)
    {
        //pvalues[i] = proxy.getSingularValues(i, nrows, ncols);
        pvalues[i] = proxy.getSpectralDensity(i, nrows, ncols);

        pfreq[i] = proxy.getFrequencies(length);

        if (pvalues[i] != 0)
        {
            try 
            {
                std::stringstream ss;
                ss << "Measurement: " << (i+1); 
                Gnuplot* gplot = new Gnuplot(ss.str());
                gplot->set_title(ss.str());
                
                ss.str(""); ss.clear();
                
                gplot->set_grid();
                gplot->set_ylogscale();
                gplot->set_style("steps");
                for (int j = 0; j < ncols; ++j)
                {
                    ss << "singular values " << j; 
                    gplot->plot_xy(pfreq[i], pfreq[i]+length, pvalues[i]+j*nrows, pvalues[i]+(j+1)*nrows, ss.str() );                   
                    ss.str(""); ss.clear();
                }

                plots.push_back(gplot);
                
            }
            catch(GnuplotException& e)
            {
                std::cout << e.what() << "\n";
                std::for_each(plots.begin(), plots.end(), delete_plot);     
            }

        }

    }

    wait_for_key();
    std::for_each(plots.begin(), plots.end(), delete_plot); 
    

    /*
     *double natFreq = 33.8;
     *int nchannels(0), nsvd(0);
     *const complex<double>* pmodes = proxy.getModes(natFreq, 0, nchannels, nsvd);
     *
     *vector< complex<double> > modesList(nchannels*nsvd);
     *std::copy(pmodes, pmodes+nchannels*nsvd, &modesList[0]);
     */


	return 0;
}

void wait_for_key()
{
    std::cout << "\n" << "Press enter to continue...\n";
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
}

void delete_plot(Gnuplot* plot)
{
    if (plot != 0)
        delete plot;
}
