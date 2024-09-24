#include "ReceivingHandler.h"
#include <ParseOutput.h>

using namespace std;

int main(int argc, char* argv[]) 
{
    if (argc < 2)
    {
        cerr << "insufficient number of arguments" << endl;
        return 1;
    }

    string OutputfileName = argv[1];

    ReceivingHandler* receivingHandler = new ReceivingHandler(OutputfileName);
    receivingHandler->listenToTCPTransmission();

    ParseOutput *parseOutput = new ParseOutput(OutputfileName); 

    delete receivingHandler;
    delete parseOutput;

    return 0;
}