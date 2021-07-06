#include "PageMapIndexer.hpp"

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress)
{
    virtualAddress >>= 12;
    TableLevel1Index = virtualAddress & 0x1FF;
    virtualAddress >>= 9;
    TableLevel2Index = virtualAddress & 0x1FF;
    virtualAddress >>= 9;
    TableLevel3Index = virtualAddress & 0x1FF;
    virtualAddress >>= 9;
    TableLevel4Index = virtualAddress & 0x1FF;
}