#include<math.h>
#include "EthernetPacket.h"

EthernetPacket::EthernetPacket(int maxPacketSize, string destAddress, string srcAddress,string etherType = "0800", string preamble = "FB555555555555", string SFD = "D5") : destAddress(destAddress), srcAddress(srcAddress), preamble(preamble), SFD(SFD), etherType(etherType)
{
    payload = new Payload(maxPacketSize);
}

EthernetPacketWrapper::EthernetPacketWrapper(int minNoOfIFGs, int maxPacketSize, string destAddress, string srcAddress, string etherType = "0800", string preamble = "FB555555555555", string SFD="D5"): ethernetPacket(maxPacketSize, destAddress, srcAddress, preamble, SFD, etherType)
{
    int totalPacketWrapperSize = maxPacketSize + minNoOfIFGs;
    int rem = totalPacketWrapperSize % 4;
    if(rem == 0)
    {
        IFGsNum = minNoOfIFGs;
    }
    else
    {
        IFGsNum = minNoOfIFGs + (4 - rem);
    }
}