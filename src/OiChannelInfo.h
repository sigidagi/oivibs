// =====================================================================================
// 
//       Filename:  OiChannelInfo.h
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  2011-02-06 12:25:33
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


#ifndef  OICHANNELINFO_INC
#define  OICHANNELINFO_INC


#include	<utility>

namespace Oi {

class ChannelInfo 
{
    private:
        string name_;
        unsigned int node_;
        int directionValue_;
        int directionAxis_;
        unsigned int nsamples_;
        double sampling_;

    public:
        string name() const { return name_; }
        void name(const string& name) { name_ = name; }

        unsigned int node() const { return node_; }
        void node(unsigned int node) { node_ = node; }
        
        int directionValue() const { return directionValue_; }
        int directionAxis() const { return directionAxis_; }
        void direction(int dir)
        {
            if (dir == 1)
            {
                directionAxis_ = 0; directionValue_ = 1;
            }
            else if (dir == -1)
            {
                directionAxis_ = 0; directionValue_ = -1;
            }
            else if (dir == 2)
            {
                directionAxis_ = 1; directionValue_ =  1;
            }
            else if (dir == -2)
            {
                directionAxis_ = 1; directionValue_ = -1;
            }
            else if (dir == 3)
            {
                directionAxis_ = 2; directionValue_ = 1;
            }
            else if (dir == -3)
            {
                directionAxis_ = 2; directionValue_ = -1;
            }
            else
            {
                directionAxis_ = -1; directionValue_ = 0;
            }
                
        }

        unsigned int nsamples() const { return nsamples_; }
        void nsamples(unsigned int nsamples) { nsamples_ = nsamples; }

        double sampling() const { return sampling_; }
        void sampling(double sampling) { sampling_ = sampling; }
};

// 
/*
 *bool isChannelsEqual(const ChannelInfo& first, const ChannelInfo& other)
 *{
 *    if (first.node() == other.node() 
 *        && first.directionAxis() == other.directionAxis()
 *        && first.directionValue() == other.directionValue())
 *    {
 *        return true;
 *    }
 *    else
 *        return false;
 *}
 */

/*
 *bool operator==(const ChannelInfo& lhs, const ChannelInfo& rhs) const
 *{
 *    if (lhs.node() == rhs.node() 
 *        && lhs.directionAxis() == rhs.directionAxis()
 *        && lhs.directionValue() == rhs.directionValue())
 *    {
 *        return true;
 *    }
 *    else
 *        return false;
 *
 *}
 *
 *bool operator!=(const ChannelInfo& lhs, const ChannelInfo& rhs) const
 *{
 *    return (*);
 *}
 */

} // namespace Oi


#endif   // ----- #ifndef OICHANNELINFO_INC  -----
