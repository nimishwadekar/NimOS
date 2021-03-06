#include <stdint.h>
#include <Memory/Memory.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageMapIndexer.hpp>
#include <Memory/PageTableManager.hpp>

// The global page table manager.
PageTableManager PagingManager(nullptr);

PageTableManager::PageTableManager(PageTable *tableLevel4) :
    TableLevel4(tableLevel4) { }

void PageTableManager::MapPage(void *virtualAddress, void *physicalAddress, bool supervisor)
{
    PageMapIndexer indexer((uint64_t) virtualAddress);
    PageTableEntry entry;

    // Level 4 Table entry.
    TableLevel4->Entries[indexer.TableLevel4Index].SetFlag(PageTableFlags::UserAccess, true);
    entry = TableLevel4->Entries[indexer.TableLevel4Index];
    PageTable *level3Table;
    if(!entry.GetFlag(PageTableFlags::Present))
    {
        level3Table = (PageTable*) FrameAllocator.RequestPageFrame();
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
    level3Table->Entries[indexer.TableLevel3Index].SetFlag(PageTableFlags::UserAccess, true);
    entry = level3Table->Entries[indexer.TableLevel3Index];
    PageTable *level2Table;
    if(!entry.GetFlag(PageTableFlags::Present))
    {
        level2Table = (PageTable*) FrameAllocator.RequestPageFrame();
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
    level2Table->Entries[indexer.TableLevel2Index].SetFlag(PageTableFlags::UserAccess, true);
    entry = level2Table->Entries[indexer.TableLevel2Index];
    PageTable *level1Table;
    if(!entry.GetFlag(PageTableFlags::Present))
    {
        level1Table = (PageTable*) FrameAllocator.RequestPageFrame();
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
    entry.SetFlag(PageTableFlags::UserAccess, !supervisor);
    level1Table->Entries[indexer.TableLevel1Index] = entry;
}

PageTableEntry *PageTableManager::GetTableEntry(void *virtualAddress, int level)
{
    if(level > 3 || level < 0) return nullptr;

    PageMapIndexer indexer((uint64_t) virtualAddress);
    if(level == 3) return TableLevel4->Entries + indexer.TableLevel4Index;

    PageTable *table = (PageTable*) TableLevel4->Entries[indexer.TableLevel4Index].GetAddress();
    if(level == 2) return table->Entries + indexer.TableLevel3Index;

    table = (PageTable*) table->Entries[indexer.TableLevel3Index].GetAddress();
    if(level == 1) return table->Entries + indexer.TableLevel2Index;

    table = (PageTable*) table->Entries[indexer.TableLevel2Index].GetAddress();
    return table->Entries + indexer.TableLevel1Index;
}