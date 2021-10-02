#pragma once

#include <stdint.h>

class PageMapIndexer
{
    public:
    PageMapIndexer(uint64_t virtualAddress);
    uint64_t TableLevel4Index;
    uint64_t TableLevel3Index;
    uint64_t TableLevel2Index;
    uint64_t TableLevel1Index;
};