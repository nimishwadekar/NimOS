#pragma once

#include "Paging.hpp"

class PageTableManager
{
    public:
    PageTableManager(PageTable *tableLevel4);
    PageTable *TableLevel4;
    void MapPage(void *virtualAddress, void *physicalAddress);
};