#include "TransmissionHandler.h"

#define IFG_VAL "07"
#define TOTAL_ORAN_HEADER_SIZE 50

string concatNStrings(string data, int repetitions)
{
    string finalString = "";
    for (int i = 0; i < repetitions; i++)
    {
        finalString.append(data);
    }

    return finalString;
}

TransmissionHandlerBurst::TransmissionHandlerBurst(TransmissionConfigBurst *transmissionConfigBurst)
{
    this->transmissionConfigBurst = transmissionConfigBurst;
    calculateTransmissionParameters();
    setEthernetPacket();
}

TransmissionHandlerBurst::~TransmissionHandlerBurst()
{
    delete ethernetPacket;
}

void TransmissionHandlerBurst::calculateTransmissionParameters()
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
    double lineRateInBytes = (pow(10, 9) * (transmissionConfigBurst->lineRateInGigaBits)) / 8;
    singleByteDurationInMicroseconds = (1 / (lineRateInBytes)) * pow(10, 6);
}

void TransmissionHandlerBurst::calculatePacketSize()
{
    int maxPacketSize = transmissionConfigBurst->maxPacketSizeInBytes;
    int minNoOfIFGs = transmissionConfigBurst->minIFGs;

    int totalPacketWrapperSize = maxPacketSize + minNoOfIFGs;
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

    packetAndIFGsSizeInBytes = maxPacketSize + IFGsNum;
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

void TransmissionHandlerBurst::setEthernetPacket()
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
    cout << "TRANSMISSION PARAMS" << endl;
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

TransmissionHandlerOran::TransmissionHandlerOran(TransmissionConfigOran *transmissionConfigOran)
{
    this->transmissionConfigOran = transmissionConfigOran;
    noOfFragmentsPerPacket = 1;
    noOfFragmentsPerLastPacketInSymbol = 1;
    calculateTransmissionParameters();
}

void TransmissionHandlerOran::calculateFrameStructureData()
{
    int frameRate = 100; // frame generation time = 10ms, the  frame rate = 100 frames/sec

    int maxNrb = transmissionConfigOran->maxNrb;

    noOfPacketsPerSymbol = ceil((double)maxNrb / transmissionConfigOran->nrbPerPacket);
    lastPacketPerSymbolNRBs = maxNrb - (noOfPacketsPerSymbol - 1) * transmissionConfigOran->nrbPerPacket;

    int noOfSlotsPerSubFrame = 1 / ((double)15.0 / transmissionConfigOran->scs);

    noOfSymbols = 14 * noOfSlotsPerSubFrame * 10 * frameRate * transmissionConfigOran->captureSizeInMs * pow(10, -3);

    OranPacket::setSlotsPerSubFrame(noOfSlotsPerSubFrame);
}

void TransmissionHandlerOran::calculateFragmentationRequirements()
{
    int maxNRBsExpectedPerPacket = transmissionConfigOran->nrbPerPacket;
    int maxAllowedNRBsPerPacket = (transmissionConfigOran->maxPacketSizeInBytes - TOTAL_ORAN_HEADER_SIZE) / (12 * 4);

    if (maxNRBsExpectedPerPacket > maxAllowedNRBsPerPacket)
    {
        noOfFragmentsPerPacket = ceil((double)maxNRBsExpectedPerPacket / maxAllowedNRBsPerPacket);
        maxNRBsPerPacket = maxAllowedNRBsPerPacket;
        lastFragmentNRBs = maxNRBsExpectedPerPacket - maxNRBsPerPacket * (noOfFragmentsPerPacket - 1);
    }
    else
    {
        maxNRBsPerPacket = lastFragmentNRBs = maxNRBsExpectedPerPacket;
    }

    // Handle last packet in symbol (handles the ceil that was done in previous packets/symbol calculation)
    if (lastPacketPerSymbolNRBs > maxAllowedNRBsPerPacket)
    {
        noOfFragmentsPerLastPacketInSymbol = ceil((double)lastPacketPerSymbolNRBs / maxAllowedNRBsPerPacket);
        lastFragmentInLastPacketPerSymbolNRBs = lastPacketPerSymbolNRBs - maxAllowedNRBsPerPacket * (noOfFragmentsPerPacket - 1);
    }
    else
    {
        lastFragmentInLastPacketPerSymbolNRBs = lastPacketPerSymbolNRBs;
    }

    OranPacket::setMaxPacketsPerSymbolAfterFragmentation((noOfPacketsPerSymbol - 1) * noOfFragmentsPerPacket + noOfFragmentsPerLastPacketInSymbol);
}

void TransmissionHandlerOran::calculateIFGsPerPacket()
{
    int minNoOfIFGs = transmissionConfigOran->minIFGs;
    int actualPacketSizeInBytes = maxNRBsPerPacket * 12 * 4 + TOTAL_ORAN_HEADER_SIZE;
    int lastPacketInFragmentSizeInBytes = lastFragmentNRBs * 12 * 4 + TOTAL_ORAN_HEADER_SIZE;
    int lastPacketInSymbolSizeInBytes = lastFragmentInLastPacketPerSymbolNRBs * 12 * 4 + TOTAL_ORAN_HEADER_SIZE;

    int totalFullPacketWrapperSize = actualPacketSizeInBytes + minNoOfIFGs;
    int rem = totalFullPacketWrapperSize % 4;
    if (rem == 0)
        noOfIFGsPerFullPacket = minNoOfIFGs;
    else
        noOfIFGsPerFullPacket = minNoOfIFGs + (4 - rem);

    if (noOfFragmentsPerPacket > 1)
    {
        int totalLastPacketInFragmentWrapperSize = lastPacketInFragmentSizeInBytes + minNoOfIFGs;
        rem = totalLastPacketInFragmentWrapperSize % 4;
        if (rem == 0)
            noOfIFGsPerLastPacketInFragment = minNoOfIFGs;
        else
            noOfIFGsPerLastPacketInFragment = minNoOfIFGs + (4 - rem);
    }

    int totalLastPacketInSymbolWrapperSize = lastPacketInSymbolSizeInBytes + minNoOfIFGs;
    rem = totalLastPacketInSymbolWrapperSize % 4;
    if (rem == 0)
        noOfIFGsPerLastPacketInSymbol = minNoOfIFGs;
    else
        noOfIFGsPerLastPacketInSymbol = minNoOfIFGs + (4 - rem);
}

void TransmissionHandlerOran::calculateTransmissionParameters()
{
    calculateFrameStructureData();
    calculateFragmentationRequirements();
    calculateIFGsPerPacket();
}

void TransmissionHandlerOran::setEthernetPacket()
{
}

void TransmissionHandlerOran::transmitPackets(FileHandler *fileHandler)
{
    vector<pair<int, int>> IQSamples;

    FileHandler *fileHandlerRead = new FileHandler(transmissionConfigOran->payloadFilename, READ_FILE);

    for (int i = 0; i < noOfSymbols; i++)
    {
        for (int j = 0; j < noOfPacketsPerSymbol; j++)
        {
            if (j == noOfPacketsPerSymbol - 1)
            {
                for (int x = 0; x < noOfFragmentsPerLastPacketInSymbol - 1; x++)
                {
                    fileHandlerRead->ReadIQFile(IQSamples, maxNRBsPerPacket * 12);
                    ethernetPacket = new EthernetPacketOran(maxNRBsPerPacket, IQSamples, transmissionConfigOran->destAddress, transmissionConfigOran->srcAddress);
                    string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, noOfIFGsPerFullPacket);
                    fileHandler->writeFile(packet);
                    delete ethernetPacket;
                }

                fileHandlerRead->ReadIQFile(IQSamples, lastFragmentInLastPacketPerSymbolNRBs * 12);
                ethernetPacket = new EthernetPacketOran(lastFragmentInLastPacketPerSymbolNRBs, IQSamples, transmissionConfigOran->destAddress, transmissionConfigOran->srcAddress);
                string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, noOfFragmentsPerLastPacketInSymbol);
                fileHandler->writeFile(packet);
                delete ethernetPacket;
            }
            else if (noOfFragmentsPerPacket == 1)
            {
                fileHandlerRead->ReadIQFile(IQSamples, maxNRBsPerPacket * 12);
                ethernetPacket = new EthernetPacketOran(maxNRBsPerPacket, IQSamples, transmissionConfigOran->destAddress, transmissionConfigOran->srcAddress);
                string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, noOfIFGsPerFullPacket);
                fileHandler->writeFile(packet);
                delete ethernetPacket;
            }
            else
            {
                for (int k = 0; k < noOfFragmentsPerPacket - 1; k++)
                {
                    fileHandlerRead->ReadIQFile(IQSamples, maxNRBsPerPacket * 12);
                    ethernetPacket = new EthernetPacketOran(maxNRBsPerPacket, IQSamples, transmissionConfigOran->destAddress, transmissionConfigOran->srcAddress);
                    string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, noOfIFGsPerFullPacket);
                    fileHandler->writeFile(packet);
                    delete ethernetPacket;
                }

                fileHandlerRead->ReadIQFile(IQSamples, lastFragmentNRBs * 12);
                ethernetPacket = new EthernetPacketOran(lastFragmentNRBs, IQSamples, transmissionConfigOran->destAddress, transmissionConfigOran->srcAddress);
                string packet = ethernetPacket->getPacketAsString() + concatNStrings(IFG_VAL, noOfIFGsPerLastPacketInFragment);
                fileHandler->writeFile(packet);
                delete ethernetPacket;
            }
        }
    }
}

void TransmissionHandlerOran::printTransmissionParams()
{
    cout << "TRANSMISSION PARAMS" << endl;
    cout << "No. of Packets per Symbol: " << noOfPacketsPerSymbol << endl;
    cout << "No. of Fragments per Packet: " << noOfFragmentsPerPacket << endl;
    cout << "No. of IFGs per Full Packet: " << noOfIFGsPerFullPacket << endl;
    cout << "No. of IFGs per Last Packet in Fragment: " << noOfIFGsPerLastPacketInFragment << endl;
    cout << "No. of IFGs per Last Packet in Symbol: " << noOfIFGsPerLastPacketInSymbol << endl;
    cout << "Max NRBs per Packet: " << maxNRBsPerPacket << endl;
    cout << "Last Fragment NRBs: " << lastFragmentNRBs << endl;
    cout << "Last Packet per Symbol NRBs: " << lastPacketPerSymbolNRBs << endl;
    cout << "No. of Fragments per Last Packet in Symbol: " << noOfFragmentsPerLastPacketInSymbol << endl;
    cout << "Last Fragment in Last Packet per Symbol NRBs: " << lastFragmentInLastPacketPerSymbolNRBs << endl;
    cout << "No. of Symbols: " << noOfSymbols << endl;
    cout << endl;
}