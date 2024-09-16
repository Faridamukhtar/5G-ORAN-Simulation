#include"Payload.h"

Payload::Payload(int maxPacketSize)
{
    int extraBytesNo = 26;
    string dummyData((maxPacketSize-extraBytesNo)*2,'0');
    payload = dummyData;
}

string Payload::getPayload()
{
    return payload;
}
