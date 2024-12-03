#include "StatData.h"
#include "StoreDump.h"
#include "LoadDump.h"
#include "SortDump.h"
#include "JoinDump.h"

int main()
{
    char fileName[] = "test.bin";
    
    storeDump(fileName);
    loadDump(fileName);

    sortDump(fileName);
    loadDump(fileName);

    joinDump(fileName);
    loadDump(fileName);

    return 0;
}
