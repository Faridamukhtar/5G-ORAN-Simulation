#pragma once
#include <string>

using namespace std;
class Payload
{
    string payload; //TODO: TEMP val - remove this when extending packets config in phase 2
public:
    Payload(int maxPacketSize);
};

// TODO (Milestone-2): extend payload to match ORAN/eCPRI packets config