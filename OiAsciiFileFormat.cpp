#include "OiAsciiFileFormat.h"

namespace Oi {

    void AsciiFileFormat::parse(const string& file)
    {

    }

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

    bool AsciiFileFormat::existData()
    {
        return false;
    }

    const arma::mat& AsciiFileFormat::getNodes()
    {
        return nodes_;
    }

    const arma::umat& AsciiFileFormat::getLines()
    {
        return lines_;
    }

    const arma::umat& AsciiFileFormat::getSurfaces()
    {
        return surfaces_;
    }

    const arma::mat& AsciiFileFormat::getData()
    {
        return data_;
    }

    double AsciiFileFormat::getSamplingT()
    {
        return samplingInterval_;
    }

    int AsciiFileFormat::getNumberOfSamples()
    {
        return numberOfSamples_;
    }

} // namespace Oi
