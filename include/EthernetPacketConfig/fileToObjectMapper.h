#include <unordered_map>
#include <string>

using namespace std;

enum ConfigProperties
{
    LINE_RATE,
    CAPTURE_SIZE,
    MIN_IFGS,
    DEST_ADDRESS,
    SRC_ADDRESS,
    MAX_PACKET_SIZE,
    BURST_SIZE,
    BURST_PERIODICITY,
};

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
};