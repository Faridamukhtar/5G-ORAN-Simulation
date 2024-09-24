#include <string>
#include <iostream>

#include "FileHandler.h"
#include "TransmissionConfig.h"
#include "TransmissionHandler.h"
#include "ParseOutput.h"

using namespace std;

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
    {
        TransmissionConfigOran *transmissionConfigOran = new TransmissionConfigOran();

        FileHandler *fileHandlerRead = new FileHandler(InputfileName, READ_FILE);
        fileHandlerRead->ReadFile(transmissionConfigOran);
        fileHandlerRead->closeFile();

        transmissionConfigOran->printProperties();

        FileHandler *fileHandlerWrite = new FileHandler(OutputfileName, WRITE_FILE);
        TransmissionHandlerOran *transmissionHandlerOran = new TransmissionHandlerOran(transmissionConfigOran);
        transmissionHandlerOran->printTransmissionParams();
        transmissionHandlerOran->transmitPackets(fileHandlerWrite);
        fileHandlerWrite->closeFile();

        ParseOutput* parseOutput = new ParseOutput(OutputfileName);

    }
    else
    {
        TransmissionConfigBurst *transmissionConfigBurst = new TransmissionConfigBurst();

        FileHandler *fileHandlerRead = new FileHandler(InputfileName, READ_FILE);
        fileHandlerRead->ReadFile(transmissionConfigBurst);
        fileHandlerRead->closeFile();

        transmissionConfigBurst->printProperties();

        FileHandler *fileHandlerWrite = new FileHandler(OutputfileName, WRITE_FILE);
        TransmissionHandlerBurst *transmissionHandlerBurst = new TransmissionHandlerBurst(transmissionConfigBurst);
        transmissionHandlerBurst->printTransmissionParams();
        transmissionHandlerBurst->transmitPackets(fileHandlerWrite);
        fileHandlerWrite->closeFile();
    }

    return 0;
}