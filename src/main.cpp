#include<string>
#include <iostream>

#include "FileHandler.h"
#include "TransmissionConfig.h"

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

    FileHandler* fileHandler = new FileHandler(InputfileName, READ_FILE);

    fileHandler->ReadFile(transmissionConfigBurst);

    transmissionConfigBurst->printProperties();

    return 0;
}