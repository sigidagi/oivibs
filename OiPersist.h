// =====================================================================================
// 
//       Filename:  OiPersist.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2010-09-29 18:00:35
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Sigitas Dagilis (), sigidagi@gmail.com
//        Company:  sigidagis
// 
// =====================================================================================


#ifndef  OIPERSIST_INC
#define  OIPERSIST_INC

#include	<string>

using std::string;

namespace Oi
{
    class PersistInterface
    {
        public:
            virtual ~PersistInterface(){}
        public:
            virtual void save(const string& name) = 0;
            virtual void load(const string& name) = 0;
    };

} // namespace Oi

#endif   // ----- #ifndef OIPERSIST_INC  -----
