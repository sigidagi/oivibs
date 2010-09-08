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

#include <armadillo>
#include <vector>

using namespace arma;


namespace Oi {

class OiData
{
private:
    /*
	 *OiData(){};
	 *OiData(OiData const&){};
	 *OiData& operator=(OiData const&){ return *this;};
     */

    // channels are represented in columns 
    arma::mat m_matData;

    cube m_Pxx;
    mat m_SVD;
    colvec m_Freq;

	friend class UniversalFormat;
    void createPSD(cube& psd, mat& postfft);

public:

//	static OiData& Instance();
    bool startProcessing();

    arma::Mat<double>& getData();
    

    std::vector<double> vSamplingInterval;
    std::vector<int> vNumberOfSamples;
 
}; // class OiData

} // namespace Oi


#endif 

