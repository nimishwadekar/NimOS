#include "PageTableManager.hpp"
#include "PageMapIndexer.hpp"
#include "PageFrameAllocator.hpp"
#include "Memory.hpp"
#include <stdint.h>
#include "../Logging.hpp"

// The global page table manager.
PageTableManager PagingManager(nullptr);

PageTableManager::PageTableManager(PageTable *tableLevel4) :
    TableLevel4(tableLevel4) { }

void PageTableManager::MapPage(void *virtualAddress, void *physicalAddress)
{
    PageMapIndexer indexer((uint64_t) virtualAddress);
    PageTableEntry entry;

    // Level 4 Table entry.
    entry = TableLevel4->Entries[indexer.TableLevel4Index];
    PageTable *level3Table;
    if(!entry.GetFlag(PageTableFlags::Present))
    {
        level3Table = (PageTable*) FrameAllocator.RequestPageFrame();
        #ifdef LOGGING
        logf("PageTableManager::MapPage(void*, void*) : Page Frame at 0x%x allocated for Level 3 Table.\n", level3Table);
        #endif

        memset(level3Table, 0, 0x1000);
        entry.SetAddress((uint64_t) level3Table);
        entry.SetFlag(PageTableFlags::Present, true);
        entry.SetFlag(PageTableFlags::ReadWrite, true);
        TableLevel4->Entries[indexer.TableLevel4Index] = entry;
    }
    else
    {
        level3Table = (PageTable*) (entry.GetAddress());
    }

    // Level 3 table entry.
    entry = level3Table->Entries[indexer.TableLevel3Index];
    PageTable *level2Table;
    if(!entry.GetFlag(PageTableFlags::Present))
    {
        level2Table = (PageTable*) FrameAllocator.RequestPageFrame();
        #ifdef LOGGING
        logf("PageTableManager::MapPage(void*, void*) : Page Frame at 0x%x allocated for Level 2 Table.\n", level2Table);
        #endif

        memset(level2Table, 0, 0x1000);
        entry.SetAddress((uint64_t) level2Table);
        entry.SetFlag(PageTableFlags::Present, true);
        entry.SetFlag(PageTableFlags::ReadWrite, true);
        level3Table->Entries[indexer.TableLevel3Index] = entry;
    }
    else
    {
        level2Table = (PageTable*) (entry.GetAddress());
    }

    // Level 2 table entry.
    entry = level2Table->Entries[indexer.TableLevel2Index];
    PageTable *level1Table;
    if(!entry.GetFlag(PageTableFlags::Present))
    {
        level1Table = (PageTable*) FrameAllocator.RequestPageFrame();
        #ifdef LOGGING
        logf("PageTableManager::MapPage(void*, void*) : Page Frame at 0x%x allocated for Level 1 Table.\n", level1Table);
        #endif

        memset(level1Table, 0, 0x1000);
        entry.SetAddress((uint64_t) level1Table);
        entry.SetFlag(PageTableFlags::Present, true);
        entry.SetFlag(PageTableFlags::ReadWrite, true);
        level2Table->Entries[indexer.TableLevel2Index] = entry;
    }
    else
    {
        level1Table = (PageTable*) (entry.GetAddress());
    }

    // Level 1 table entry.
    entry = level1Table->Entries[indexer.TableLevel1Index];
    entry.SetAddress((uint64_t) physicalAddress);
    entry.SetFlag(PageTableFlags::Present, true);
    entry.SetFlag(PageTableFlags::ReadWrite, true);
    level1Table->Entries[indexer.TableLevel1Index] = entry;
}