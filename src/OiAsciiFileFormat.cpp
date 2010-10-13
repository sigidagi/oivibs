#include "OiRoot.h"
#include "OiAsciiFileFormat.h"
#include "OiUtil.h"
#include <sstream>
#include <unistd.h>


namespace Oi {

    AsciiFileFormat::AsciiFileFormat(Root* owner) 
            : FileFormatInterface(owner), existData_(false)
    {

    }

    void AsciiFileFormat::parse(const string& file)
    {
        string path = Oi::stripToPath(file);
        chdir(path.c_str());

        string fileName = Oi::stripToFileName(file);

        bool status = records_.load(file, arma::raw_ascii);            
        if (status == true)
        {
            existData_ = true;
        }

    }

/*
 *    bool AsciiFileFormat::searchSamplingInterval(const string& fileName)
 *    {
 *
 *        fileStream_.open(fileName.c_str(), std::ios::in);
 *        if (fileStream_.is_open())
 *        { 
 *            // return stream to the beginning.
 *            fileStream_.seekg(0, std::ios::beg);
 *
 *            string line;
 *            std::stringstream ss;
 *            double dT = 0.0;
 *
 *            while (!fileStream_.eof())
 *            {
 *                getline(fileStream_, line);
 *                if (fileStream_.eof())
 *                    break;
 *
 *                ss << line;
 *                if (line == "T\r" && !fileStream_.eof())
 *                {
 *                    getline(fileStream_, line);
 *                    ss.str("");
 *                    ss.clear();
 *                    ss << line;
 *                    ss >> dT;
 *                    samplingInterval_ = dT;
 *                     
 *                    break;
 *                }
 *                
 *            }
 *
 *            fileStream_.close();
 *       
 *            if (dT == 0.0)
 *                return false;
 *        }
 *   
 *        return true;
 *    }
 */

    bool AsciiFileFormat::existNodes()
    {
        return false;
    }

    bool AsciiFileFormat::existLines()
    {
        return false;
    }

    bool AsciiFileFormat::existSurfaces()
    {
        return false;
    }

    bool AsciiFileFormat::existRecords()
    {
        return existData_;
    }

    arma::mat& AsciiFileFormat::getNodes()
    {
        return nodes_;
    }

    arma::umat& AsciiFileFormat::getLines()
    {
        return lines_;
    }

    arma::umat& AsciiFileFormat::getSurfaces()
    {
        return surfaces_;
    }

    arma::mat& AsciiFileFormat::getRecords()
    {
        return records_;
    }

    double AsciiFileFormat::getSamplingInterval()
    {
        return 1.0;
    }

    int AsciiFileFormat::getNumberOfSamples()
    {
        return numberOfSamples_;
    }

} // namespace Oi
