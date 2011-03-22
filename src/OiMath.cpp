// =====================================================================================
// 
//       Filename:  OiMath.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-03-02 12:05:02
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//
//     This file is part of the OiVibrations C++ library.
//     It is provided without any warranty of fitness
//     for any purpose. You can redistribute this file
//     and/or modify it under the terms of the GNU
//     General Public License (GPL) as published
//     by the Free Software Foundation, either version 3
//     of the License or (at your option) any later version.
//     (see http://www.opensource.org/licenses for more info)
// 
// =====================================================================================


#include	"OiMath.h"
#include	<OiFFTWest.h>
#include    <cmath>
#include    <iostream>
#include    <boost/progress.hpp>


namespace Oi {
 
    void  detrend( Mat<double>& x, int p)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        if (x.n_rows == 1)
            x = arma::trans(x);
       
       // create a matrix b where columns represent polinomial order:
       // second column - linear approximation of data
       // third column - second order polinomial approximation of data.
       // and so on..
       // number of rows size of data. 
        int rows = x.n_rows;
        mat b = (linspace<colvec>(1,rows,rows) * ones<rowvec>(p+1));

        for (int i = 0; i < (int)b.n_cols; ++i)
        {
            b.col(0) = pow(b.col(0), 0);
        }
        
        // subtract approximation curve from original data.
        x = x - b *( arma::pinv(b)*x );
    }

    colvec  hamming(int m)
    {
        colvec ham;
        if ( m <= 0)
           return ham;

        ham.set_size(m);
        ham = 0.54 - 0.46 * cos( 2 * M_PI * linspace<colvec>(0,m-1, m)/ (m-1) );

        return ham;
    }
   
   void inverse( Mat<double>& x)
    {
        if (x.n_cols == 0 || x.n_rows == 0)
            return;

        arma::pinv(x); 
    }
    
    void convmtx(colvec in, int p, mat& out)
    {
        in.set_size( in.n_elem + p-1 );
        for (int i = 0; i < p-1; ++i)
            in(in.n_elem-1 - i) = 0;
        
        rowvec temp(p);
        temp.zeros();
        temp(0) = in(0);

        out = arma::toeplitz(in, temp);
    }
    
    void covar(colvec in, int p, mat& out )
    {
        unsigned int length = in.n_elem;
        colvec temp = ones<colvec>(length, 1);

        in = in - temp * (arma::sum(in)/length); 
        
        mat out1;
        convmtx(in, p, out1);
        convmtx(in, p, out);
        
        out = trans(out1)*out/(length-1); 
    }

    

} // namespace Oi 
