
#include "ParseOutput.h"

#define IFG_VAL "07"
#define IFG_HALF_LINE "0707"
#define IFG_LINE "07070707"
#define startOfPacketByte1 "FB555555"
#define startOfPacketByte2 "555555D5"

using json = nlohmann::json;


using namespace std;

int hex16BitToSigned(string hexNum)
{
    try
    {
        int signedNum = stoi(hexNum, nullptr, 16);

        if (signedNum & 0x8000) // checks if negative number by comparing highest bit (2s complement)
            signedNum -= pow(2, 16);

        return signedNum;
    }
    catch (const std::exception &e)
    {
        return -1;
    }
}

int hexToInt(string hexNum)
{
    try
    {
        int num = stoi(hexNum, nullptr, 16);
        return num;
    }
    catch (const std::exception &e)
    {
        return -1;
    }
}

ParseOutput::ParseOutput(string outputFilename)
{
    fileHandlerWrite = new FileHandler("parsedPackets.json", WRITE_FILE);
    parseOutput(outputFilename);
}

ParseOutput::~ParseOutput()
{
    delete fileHandlerWrite;
}

void ParseOutput::parseOutput(string outputFilename)
{
    FileHandler *fileHandler = new FileHandler(outputFilename, READ_FILE);
    json jsonArray = json::array();

    string line;

    while (fileHandler->ReadFileLineByLine(line))
    {
        if (line == IFG_LINE)
            continue;

        if (startOfPacketByte1 == line)
        {
            ParsedPacket parsedPacket;

            fileHandler->ReadFileLineByLine(line);
            if (startOfPacketByte2 != line) // Check if a wrong "start of byte" was detected
                continue;

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.destAddress = line;

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.destAddress.append(line.substr(0, 4));
            parsedPacket.srcAddress = line.substr(4);

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.srcAddress.append(line);

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.etherType = line.substr(0, 4);
            parsedPacket.ecpriVersionReservedConcat = line.substr(4, 2);
            parsedPacket.ecpriMessage = line.substr(6);

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.ecpriPayload = line.substr(0, 4);
            parsedPacket.ecpriPcRtc = line.substr(4);

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.ecpriSeqId = line.substr(0, 4);
            parsedPacket.oranDataDirectionPLVersionFilterIndex = line.substr(4, 2);
            parsedPacket.oranFrameId = line.substr(6);

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.oranSubFrameId = line.substr(0, 1);
            parsedPacket.oranSlotId = line.substr(1, 2);
            parsedPacket.oranSymbolId = line.substr(3, 1);
            parsedPacket.sectionIdRbSymInc = line.substr(4, 3);
            parsedPacket.startPrbu = line.substr(7);

            fileHandler->ReadFileLineByLine(line);
            parsedPacket.startPrbu.append(line.substr(0, 2));
            parsedPacket.numPrbu = line.substr(2, 2);

            string iSampleStr, qSampleStr, currentISampleStr;
            int iSample, qSample, currentISample;

            iSampleStr = currentISampleStr = line.substr(4);
            iSample = currentISample = hex16BitToSigned(iSampleStr);

            string newLine, secondIQstr;
            bool midLineIfgFlag = false;

            while (fileHandler->ReadFileLineByLine(newLine))
            {
                if (newLine == IFG_LINE)
                {
                    if (midLineIfgFlag)
                    {
                        parsedPacket.crc = iSampleStr + qSampleStr;
                        parsedPacket.IQSamples.pop_back(); // Remove crc mistaken priorly as IQ sample
                    }
                    else
                    {
                        parsedPacket.crc = line;
                    }

                    break;
                }

                iSampleStr = currentISampleStr;
                iSample = currentISample;

                midLineIfgFlag = false;
                qSampleStr = newLine.substr(0, 4);
                qSample = hex16BitToSigned(qSampleStr);

                parsedPacket.IQSamples.emplace_back(iSample, qSample);

                currentISampleStr = newLine.substr(4);
                currentISample = hex16BitToSigned(currentISampleStr);

                if (currentISampleStr == IFG_HALF_LINE)
                {
                    midLineIfgFlag = true;
                }

                line = newLine;
            }

            jsonArray.push_back(parsedPacket.toJsonObject());
        }
    }

    fileHandlerWrite->writeFileString(jsonArray.dump(4));

    delete fileHandler;
}

json ParsedPacket::printVectorInJson()
{
    json iqArray = json::array();

    for (const auto& sample : IQSamples)
    {
        iqArray.push_back({{"i", to_string(sample.first)}, {"q", to_string(sample.second)}});
    }

    return iqArray;
}

json ParsedPacket::toJsonObject()
{
    json j;

    j["ethernet"] = {
        {"destAddress", destAddress},
        {"srcAddress", srcAddress},
        {"etherType", etherType}};
    j["ecpri"] = {
        {"versionReserved", ecpriVersionReservedConcat},
        {"message", ecpriMessage},
        {"payload", hexToInt(ecpriPayload)},
        {"pcRtc", ecpriPcRtc},
        {"seqId", hexToInt(ecpriSeqId)}};
    j["oran"] = {
        {"dataDirectionPLVersionFilterIndex", oranDataDirectionPLVersionFilterIndex},
        {"frameId", hexToInt(oranFrameId)},
        {"subFrameId", hexToInt(oranSubFrameId)},
        {"slotId", hexToInt(oranSlotId)/(int) pow(2, 2)},
        {"symbolId", hexToInt(oranSymbolId)},
        {"sectionIdRbSymInc", sectionIdRbSymInc},
        {"startPrbu", hexToInt(startPrbu)},
        {"numPrbu", hexToInt(numPrbu)},
        {"crc", crc},
        {"payload", printVectorInJson()}};

    return j;
}

void ParsedPacket::printProperties()
{
    cout << "Ethernet Details:\n"
         << "  Destination Address: " << destAddress << "\n"
         << "  Source Address: " << srcAddress << "\n"
         << "  Ether Type: " << etherType << "\n\n"
         << "eCPRI Details:\n"
         << "  Version Reserved: " << ecpriVersionReservedConcat << "\n"
         << "  Message: " << ecpriMessage << "\n"
         << "  Payload: " << ecpriPayload << "\n"
         << "  PC RTC: " << ecpriPcRtc << "\n"
         << "  Sequence ID: " << ecpriSeqId << "\n\n"
         << "ORAN Details:\n"
         << "  Data Direction PL Version Filter Index: " << oranDataDirectionPLVersionFilterIndex << "\n"
         << "  Frame ID: " << oranFrameId << "\n"
         << "  Sub Frame ID: " << oranSubFrameId << "\n"
         << "  Slot ID: " << oranSlotId << "\n"
         << "  Symbol ID: " << oranSymbolId << "\n"
         << "  Section ID RB Symbol Inc: " << sectionIdRbSymInc << "\n"
         << "  Start PRBU: " << startPrbu << "\n"
         << "  Number of PRBU: " << numPrbu << "\n"
         << "  CRC: " << crc << "\n"
         << "  Payload:" << printVectorInJson().dump(4) << "\n";
}
