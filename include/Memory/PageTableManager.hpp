#pragma once

#include <Memory/Paging.hpp>

class PageTableManager
{
    public:
    PageTableManager(PageTable *tableLevel4);
    PageTable *TableLevel4;
    void MapPage(void *virtualAddress, void *physicalAddress, bool supervisor = false);
};

extern PageTableManager PagingManager;