#include "bios.h"
#include "interconnect.h"
#include "cpu.h"
// using namespace std;

int main()
{

    BIOS bios("SCPH1001.BIN");
    Interconnect interconnect(bios);
    CPU cpu(interconnect);
    while (1)
    {
        cpu.run_next_instruction();
    }
}