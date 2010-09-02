// =====================================================================================
// 
//       Filename:  OiData.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-06-06 21:26:44
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================

#ifndef _OIDATA_H
#define _OIDATA_H

#include "UniversalFormat.h"
#include <armadillo>

using namespace arma;

class OiData
{
private:
    /*
	 *OiData(){};
	 *OiData(OiData const&){};
	 *OiData& operator=(OiData const&){ return *this;};
     */

    cube m_Pxx;
    mat m_SVD;
    colvec m_Freq;

	friend class UniversalFormat;
    void createPSD(cube& psd, mat& postfft);

public:

//	static OiData& Instance();
    bool startProcessing(UniversalFormat* pUFF);

}; // class OiData

#endif 

