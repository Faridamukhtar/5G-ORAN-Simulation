#include<string>
#include <iostream>

#include "FileHandler.h"
#include "TransmissionConfig.h"
#include "TransmissionHandler.h"

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

    TransmissionConfigBurst* transmissionConfigBurst = new TransmissionConfigBurst();

    FileHandler* fileHandlerRead = new FileHandler(InputfileName, READ_FILE);
    fileHandlerRead->ReadFile(transmissionConfigBurst);
    fileHandlerRead->closeFile();

    transmissionConfigBurst->printProperties();

    FileHandler* fileHandlerWrite = new FileHandler(OutputfileName, WRITE_FILE);
    TransmissionHandlerBurst* transmissionHandlerBurst = new TransmissionHandlerBurst(transmissionConfigBurst);
    transmissionHandlerBurst->printTransmissionParams();
    transmissionHandlerBurst->transmitPackets(fileHandlerWrite);
    fileHandlerWrite->closeFile();

    return 0;
}