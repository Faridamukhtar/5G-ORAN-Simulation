#include <string>
#include <iostream>

#include "FileHandler.h"
#include "TransmissionConfig.h"
#include "TransmissionHandler.h"
#include "ParseOutput.h"

using namespace std;

void TransmitOran(string InputfileName, string OutputfileName);

void TransmitBurst(string InputfileName, string OutputFileName);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "insufficient number of arguments" << endl;
        return 1;
    }

    string InputfileName = argv[1];
    string OutputfileName = argv[2];

    if (argc == 2 || argv[3] == string("ORAN"))
        TransmitOran(InputfileName, OutputfileName);
    else
        TransmitBurst(InputfileName, OutputfileName);

    return 0;
}

void TransmitOran(string InputfileName, string OutputfileName)
{
    TransmissionConfigOran *transmissionConfigOran = new TransmissionConfigOran();

    FileHandler *fileHandlerRead = new FileHandler(InputfileName, READ_FILE);
    fileHandlerRead->ReadFile(transmissionConfigOran);
    delete fileHandlerRead;

    transmissionConfigOran->printProperties();

    FileHandler *TempGeneratedPacketsFile = new FileHandler("tempGen.txt", WRITE_FILE);
    TransmissionHandlerOran *transmissionHandlerOran = new TransmissionHandlerOran(transmissionConfigOran);
    transmissionHandlerOran->printTransmissionParams();
    transmissionHandlerOran->generatePackets(TempGeneratedPacketsFile);
    delete TempGeneratedPacketsFile;


    FileHandler *generatedPacketsFileForRead = new FileHandler("tempGen.txt", READ_FILE);
    transmissionHandlerOran->transmitPackets(generatedPacketsFileForRead);

    ParseOutput *parseOutput = new ParseOutput(OutputfileName); //TODO: To be moved to other process when TCP is finalized


    delete transmissionConfigOran;
    delete transmissionHandlerOran;
    delete generatedPacketsFileForRead;
}

void TransmitBurst(string InputfileName, string OutputFileName)
{
    TransmissionConfigBurst *transmissionConfigBurst = new TransmissionConfigBurst();

    FileHandler *fileHandlerRead = new FileHandler(InputfileName, READ_FILE);
    fileHandlerRead->ReadFile(transmissionConfigBurst);
    delete fileHandlerRead;

    transmissionConfigBurst->printProperties();

    FileHandler *TempGeneratedPacketsFile = new FileHandler("tempGen.txt", WRITE_FILE);
    TransmissionHandlerBurst *transmissionHandlerBurst = new TransmissionHandlerBurst(transmissionConfigBurst);
    transmissionHandlerBurst->printTransmissionParams();
    transmissionHandlerBurst->generatePackets(TempGeneratedPacketsFile);
    delete TempGeneratedPacketsFile;

    FileHandler *generatedPacketsFileForRead = new FileHandler("tempGen.txt", READ_FILE);
    transmissionHandlerBurst->transmitPackets(generatedPacketsFileForRead);

    delete transmissionConfigBurst;
    delete transmissionHandlerBurst;
    delete generatedPacketsFileForRead;
}