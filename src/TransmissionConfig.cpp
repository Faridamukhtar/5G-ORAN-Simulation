#include <unordered_map>
#include <iostream>

#include "TransmissionConfig.h"

using namespace std;

unordered_map<ConfigProperties, string> objectPropertyToFilePropertyMapper =
    {
        {LINE_RATE, "Eth.LineRate"},
        {CAPTURE_SIZE, "Eth.CaptureSizeMs"},
        {MIN_IFGS, "Eth.MinNumOfIFGsPerPacket"},
        {DEST_ADDRESS, "Eth.DestAddress"},
        {SRC_ADDRESS, "Eth.SourceAddress"},
        {MAX_PACKET_SIZE, "Eth.MaxPacketSize"},
        {BURST_SIZE, "Eth.BurstSize"},
        {BURST_PERIODICITY, "Eth.BurstPeriodicity_us"},
};

TransmissionConfigBurst::TransmissionConfigBurst() {}

void TransmissionConfigBurst::initializeConfigData(unordered_map<string, string> configData)
{
    lineRateInGigaBits = stod(configData[objectPropertyToFilePropertyMapper[LINE_RATE]]);

    captureSizeInMs = stod(configData[objectPropertyToFilePropertyMapper[CAPTURE_SIZE]]);

    minIFGs = stoi(configData[objectPropertyToFilePropertyMapper[MIN_IFGS]]);

    destAddress = remove0xFromAddress(configData[objectPropertyToFilePropertyMapper[DEST_ADDRESS]]);

    srcAddress = remove0xFromAddress(configData[objectPropertyToFilePropertyMapper[SRC_ADDRESS]]);
    maxPacketSizeInBytes = stoi(configData[objectPropertyToFilePropertyMapper[MAX_PACKET_SIZE]]);
    burstSize = stoi(configData[objectPropertyToFilePropertyMapper[BURST_SIZE]]);
    burstPeriodicityInMicroseconds = stod(configData[objectPropertyToFilePropertyMapper[BURST_PERIODICITY]]);
}

string TransmissionConfigBurst::remove0xFromAddress(string address)
{
    auto pos = address.find('x');
    if (pos != string::npos) {
        return address.substr(pos + 1);
    }

    return address;
}

void TransmissionConfigBurst::printProperties()
{
    cout << "CONFIG DATA:" << endl;
    cout << "Line Rate (GigaBits): " << lineRateInGigaBits << endl;
    cout << "Capture Size (ms): " << captureSizeInMs << endl;
    cout << "Minimum IFGs: " << minIFGs << endl;
    cout << "Destination Address: " << destAddress << endl;
    cout << "Source Address: " << srcAddress << endl;
    cout << "Maximum Packet Size (Bytes): " << maxPacketSizeInBytes << endl;
    cout << "Burst Size: " << burstSize << endl;
    cout << "Burst Periodicity (Microseconds): " << burstPeriodicityInMicroseconds << endl;
    cout << endl;
}