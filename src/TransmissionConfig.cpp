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
        {SCS_VAL, "Oran.SCS"},
        {MAX_NRB, "Oran.MaxNrb"},
        {NRB_PER_PACKET, "Oran.NrbPerPacket"},
        {PAYLOAD_FILENAME, "Oran.Payload"},
    };

void TransmissionConfig::initializeConfigData(unordered_map<string, string> configData)
{
    lineRateInGigaBits = stod(configData[objectPropertyToFilePropertyMapper[LINE_RATE]]);

    captureSizeInMs = stod(configData[objectPropertyToFilePropertyMapper[CAPTURE_SIZE]]);

    minIFGs = stoi(configData[objectPropertyToFilePropertyMapper[MIN_IFGS]]);

    destAddress = remove0xFromAddress(configData[objectPropertyToFilePropertyMapper[DEST_ADDRESS]]);

    srcAddress = remove0xFromAddress(configData[objectPropertyToFilePropertyMapper[SRC_ADDRESS]]);

    maxPacketSizeInBytes = stoi(configData[objectPropertyToFilePropertyMapper[MAX_PACKET_SIZE]]);
}

void TransmissionConfig::printProperties()
{
    cout << "CONFIG DATA:" << endl;
    cout << "Line Rate (GigaBits): " << lineRateInGigaBits << endl;
    cout << "Capture Size (ms): " << captureSizeInMs << endl;
    cout << "Minimum IFGs: " << minIFGs << endl;
    cout << "Destination Address: " << destAddress << endl;
    cout << "Source Address: " << srcAddress << endl;
    cout << "Maximum Packet Size (Bytes): " << maxPacketSizeInBytes << endl;
}

string TransmissionConfig::remove0xFromAddress(string address)
{
    auto pos = address.find('x');
    if (pos != string::npos)
    {
        return address.substr(pos + 1);
    }

    return address;
}

TransmissionConfigBurst::TransmissionConfigBurst() {}

void TransmissionConfigBurst::initializeConfigData(unordered_map<string, string> configData)
{
    TransmissionConfig::initializeConfigData(configData);

    burstSize = stoi(configData[objectPropertyToFilePropertyMapper[BURST_SIZE]]);
    burstPeriodicityInMicroseconds = stod(configData[objectPropertyToFilePropertyMapper[BURST_PERIODICITY]]);
}

void TransmissionConfigBurst::printProperties()
{
    TransmissionConfig::printProperties();

    cout << "Burst Size: " << burstSize << endl;
    cout << "Burst Periodicity (Microseconds): " << burstPeriodicityInMicroseconds << endl;
    cout << endl;
}

TransmissionConfigOran::TransmissionConfigOran() {}

void TransmissionConfigOran::initializeConfigData(unordered_map<string, string> configData)
{
    TransmissionConfig::initializeConfigData(configData);

    scs = stoi(configData[objectPropertyToFilePropertyMapper[SCS_VAL]]);

    maxNrb = stoi(configData[objectPropertyToFilePropertyMapper[MAX_NRB]]);
    if (maxNrb == 0)
        maxNrb = 273;

    nrbPerPacket = stoi(configData[objectPropertyToFilePropertyMapper[NRB_PER_PACKET]]);
    if (nrbPerPacket == 0)
        nrbPerPacket = 273;

    payloadFilename = configData[objectPropertyToFilePropertyMapper[PAYLOAD_FILENAME]];
}

void TransmissionConfigOran::printProperties()
{
    TransmissionConfig::printProperties();

    cout << "SCS: " << scs << endl;
    cout << "Max NRB: " << maxNrb << endl;
    cout << "NRB per Packet: " << nrbPerPacket << endl;
    cout << "Payload Filename: " << payloadFilename << endl;
    cout << endl;
}
