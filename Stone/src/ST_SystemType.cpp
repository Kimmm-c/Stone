#include "ST_SystemType.h"

SystemTypeID getUniqueSystemTypeID()
{
    static SystemTypeID lastID = 0;
    return lastID++;
}