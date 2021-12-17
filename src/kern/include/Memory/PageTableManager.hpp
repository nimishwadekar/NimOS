#pragma once

#include <Memory/Paging.hpp>

class PageTableManager
{
    public:
    PageTableManager(PageTable *tableLevel4);
    PageTable *TableLevel4;
    void MapPage(void *virtualAddress, void *physicalAddress, bool supervisor = false);
    // Level 0 : 4KB page level. 0 <= level < 4.
    PageTableEntry *GetTableEntry(void *virtualAddress, int level = 0);
};

extern PageTableManager PagingManager;