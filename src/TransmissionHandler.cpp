#include "TransmissionHandler.h"

#define IFG_VAL "07"

string concatNStrings(string data, int repetitions)
{
    string finalString = "";
    for (int i=0; i<repetitions; i++)
    {
        finalString.append(data);
    }

    return finalString;
}

TransmissionHandlerBurst::TransmissionHandlerBurst(TransmissionConfigBurst *transmissionConfigBurst)
{
    this->transmissionConfigBurst = transmissionConfigBurst;
    CalculateTransmissionParameters();
    initializeEthernetPackets();
}

TransmissionHandlerBurst::~TransmissionHandlerBurst()
{
    delete ethernetPacket;
}

void TransmissionHandlerBurst::CalculateTransmissionParameters()
{
    calculateSingleByteDurationInMicroseconds();
    calculatePacketSize();
    calculatePacketDuration();
    calculateTotalNumberOfCompleteBursts();
    calculateNumberOfLastBurstPackets();
    calculateNumberOfIFGsAfterLastPacket();
}

void TransmissionHandlerBurst::calculateSingleByteDurationInMicroseconds()
{
    double lineRateInBytes = (pow(10,9) * (transmissionConfigBurst->lineRateInGigaBits))/8;
    singleByteDurationInMicroseconds = (1 / (lineRateInBytes)) * pow(10, 6);
}

void TransmissionHandlerBurst::calculatePacketSize()
{
    int maxPacketSize = transmissionConfigBurst->maxPacketSizeInBytes;
    int minNoOfIFGs = transmissionConfigBurst->minIFGs;

    int totalPacketWrapperSize = maxPacketSize + minNoOfIFGs; // VALID FOR MILESTONE_1:FIXED PACKET SIZE = MAX PACKET SIZE
    int rem = totalPacketWrapperSize % 4;
    IFGsNum = 0;
    if (rem == 0)
    {
        IFGsNum = minNoOfIFGs;
    }
    else
    {
        IFGsNum = minNoOfIFGs + (4 - rem);
    }

    packetAndIFGsSizeInBytes = maxPacketSize + IFGsNum; // TODO: For milestone 2, replace max packet size with actual packet size and do necessay changes (e.g., fragmentation)
}

void TransmissionHandlerBurst::calculatePacketDuration()
{
    packetAndIFGsDurationInMicroseconds = singleByteDurationInMicroseconds * (packetAndIFGsSizeInBytes);
}

void TransmissionHandlerBurst::calculateTotalNumberOfCompleteBursts()
{
    double captureSizeInMicroseconds = (transmissionConfigBurst->captureSizeInMs) * pow(10, 3);
    burstAndPeriodicityDurationInMicroseconds = ((transmissionConfigBurst->burstSize) * packetAndIFGsDurationInMicroseconds) + (transmissionConfigBurst->burstPeriodicityInMicroseconds);
    totalNumberOfCompleteBursts = (captureSizeInMicroseconds) / burstAndPeriodicityDurationInMicroseconds;
    totalNumberOfIFGsBetweenBursts = transmissionConfigBurst->burstPeriodicityInMicroseconds / singleByteDurationInMicroseconds;
    int rem = totalNumberOfIFGsBetweenBursts % 4;
    if (rem != 0)
    {
        totalNumberOfIFGsBetweenBursts += (4 - rem);
    }
}

void TransmissionHandlerBurst::calculateNumberOfLastBurstPackets()
{
    double captureSizeInMicroseconds = (transmissionConfigBurst->captureSizeInMs) * pow(10, 3);
    double totalCompleteBurstTime = totalNumberOfCompleteBursts * burstAndPeriodicityDurationInMicroseconds;

    double remainingTimeAfterCompleteBursts = captureSizeInMicroseconds - totalCompleteBurstTime;
    numberOfLastBurstPackets = remainingTimeAfterCompleteBursts / packetAndIFGsDurationInMicroseconds;
}

void TransmissionHandlerBurst::calculateNumberOfIFGsAfterLastPacket()
{
    double captureSizeInMicroseconds = (transmissionConfigBurst->captureSizeInMs) * pow(10, 3);
    double totalCompleteBurstTime = totalNumberOfCompleteBursts * burstAndPeriodicityDurationInMicroseconds;

    double remainingTimeAfterLastPacket = captureSizeInMicroseconds - (totalCompleteBurstTime + numberOfLastBurstPackets * packetAndIFGsDurationInMicroseconds);
    numberOfIFGsAfterLastPacket = remainingTimeAfterLastPacket / singleByteDurationInMicroseconds;
    int rem = numberOfIFGsAfterLastPacket % 4;
    if (rem != 0)
    {
        numberOfIFGsAfterLastPacket += (4 - rem);
    }
}

void TransmissionHandlerBurst::initializeEthernetPackets()
{
    int maxPacketSize = transmissionConfigBurst->maxPacketSizeInBytes;
    string destAddress = transmissionConfigBurst->destAddress;
    string srcAddress = transmissionConfigBurst->srcAddress;
    ethernetPacket = new EthernetPacket(maxPacketSize, destAddress, srcAddress);
}

void TransmissionHandlerBurst::transmitPackets(FileHandler *fileHandler)
{
    for (int i = 0; i < totalNumberOfCompleteBursts; i++)
    {
        for (int j = 0; j < transmissionConfigBurst->burstSize; j++)
        {
            string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, IFGsNum);
            fileHandler->writeFile(packet);
        }
        
        string IFGsBetweenBursts = concatNStrings(IFG_VAL, totalNumberOfIFGsBetweenBursts);
        fileHandler->writeFile(IFGsBetweenBursts);
    }

    for (int i = 0; i < numberOfLastBurstPackets; i++)
    {
        string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, IFGsNum);
        fileHandler->writeFile(packet);
    }

    string IFGsAfterLastPacket = concatNStrings(IFG_VAL, numberOfIFGsAfterLastPacket);
    fileHandler->writeFile(IFGsAfterLastPacket);
}

void TransmissionHandlerBurst::printTransmissionParams()
{
    cout<< "TRANSMISSION PARAMS" << endl;
    cout << "Single Byte Duration (μs): " << singleByteDurationInMicroseconds << endl;
    cout << "Packet and IFGs Size (Bytes): " << packetAndIFGsSizeInBytes << endl;
    cout << "Number of IFGs: " << IFGsNum << endl;
    cout << "Packet and IFGs Duration (μs): " << packetAndIFGsDurationInMicroseconds << endl;
    cout << "Burst and Periodicity Duration (μs): " << burstAndPeriodicityDurationInMicroseconds << endl;
    cout << "Total Number of Complete Bursts: " << totalNumberOfCompleteBursts << endl;
    cout << "Total Number of IFGs Between Bursts: " << totalNumberOfIFGsBetweenBursts << endl;
    cout << "Number of Last Burst Packets: " << numberOfLastBurstPackets << endl;
    cout << "Number of IFGs After Last Packet: " << numberOfIFGsAfterLastPacket << endl;
    cout << endl;
}