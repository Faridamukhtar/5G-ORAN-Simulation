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

    if (argc == 2 || argv[3] == string("ORAN"))
    {
        ParseOutput *parseOutput = new ParseOutput(OutputfileName); 
        delete parseOutput;
    }
    
    delete receivingHandler;

    return 0;
}