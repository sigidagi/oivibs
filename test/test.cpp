#include <iostream>
#include <cstdlib>
#include <vector>

#include <unistd.h>
#include <armadillo>
#include <stdio.h>
#include <dlfcn.h>
#include <neko_vm.h>

#include "../gfft/gfft.h"

using namespace std;
using namespace arma;

int main(int argc, char* argv[])
{
    
    chdir("../");
    void* hndl = dlopen("libOiVibrations.so", RTLD_LAZY);
    if (hndl == NULL)
    {
        cerr << dlerror() << endl;
        exit(-1);
    }

    // ------------------- ACML ------------------------------
    int tt = 1<<10;
    colvec time = linspace<colvec>(0, tt-1, tt) / tt;
    colvec in= 0.7*sin(2*M_PI*50*time) + sin(2*M_PI*120*time);
    in = in + 2*randn(tt);

    
    int nfft = tt/2;
    

/*
 *    cx_colvec out(nfft);
 *
 *    typedef void (*fnc_fftw_ptr)(colvec&, cx_colvec&);
 *    fnc_fftw_ptr pfftw = (fnc_fftw_ptr)(dlsym(hndl, "fftw"));
 *    if (pfftw == NULL)
 *    {
 *        dlclose(hndl);
 *        exit(-1);
 *    }
 *
 *    pfftw(in, out); 
 *    colvec f = 512*linspace<colvec>(0, 1, 513);
 *
 *    f.save("f.dat", raw_ascii);
 *    in.save("signal.dat", raw_ascii);
 *    //
 */
    // ----------------- end ACML -------------------------------
    //
/*
 *   
 *    printf("\n");
 *    printf("ACML example: FFT of a real sequence using dzfft\n");
 *    printf("------------------------------------------------\n");
 *    printf("\n");
 *
 *
 *    typedef void (*fnc_ffta_ptr)(double*, long);
 *    fnc_ffta_ptr pffta = (fnc_ffta_ptr)(dlsym(hndl, "four1"));
 *
 *    if (pffta == NULL)
 *    {
 *        dlclose(hndl);
 *        exit(-1);
 *    }
 *  
 *    int nelem = (in.n_elem)/2;
 *
 *
 *    pffta(in.memptr(), nelem);
 *    cx_colvec out(nelem);
 *    
 *    long pos = 0;
 *    for ( int it = (in.n_elem-1); it > -1; it-=2)
 *    {
 *         real(out(pos)) = in(it-1);
 *         imag(out(pos)) = in(it);
 *         ++pos;
 *    }
 *
 *    colvec outa(nelem);
 *    outa = arma::abs( out % arma::conj(out) );
 *    outa.save("sig1.dat", raw_ascii);
 *
 */

    
    // ------------------------- GFFT example -----------------------


// range of the needed GFFT classes
    const unsigned Min = 1;
    const unsigned Max = 27;

// initialization of the object factory
    Loki::Factory<AbstractFFT<Tp>,unsigned int> gfft_factory;
    FactoryInit<GFFTList<GFFT,Min,Max>::Result>::apply(gfft_factory);


// runtime definition of the data length
    int p = 9;

// create an instance of the GFFT
    AbstractFFT<Tp>* gfft = gfft_factory.CreateObject(p);

// compute FFT
    gfft->fft(in.memptr());

    cx_colvec out(nfft);
    
    long pos = nfft-1;
    for ( int it = 0; it < nfft; ++it)
    {
         real(out(pos)) = in(2*it);
         imag(out(pos)) = in(2*it+1);
         --pos;
    }

    colvec outa(nfft);
    outa = arma::pow( arma::abs(out), 2)/tt;
    outa.save("gfft.dat", raw_ascii);
 
    // --------------------------- end GFFT -----------------------

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
