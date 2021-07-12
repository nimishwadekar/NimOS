#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/Heap.hpp"

struct node
{
    int data;
    node *next;
};

// Kernel's main function.
void KernelStart(void)
{
    #ifdef LOGGING
    Logf("Kernel initialized.\n");
    #endif

    MainRenderer.ClearScreen();
    MainRenderer.Printf("Kernel initialized.\n");

    int a[] = {3, 19, 423, 936, 114, 11463, 9678};
    node *head = (node*) Malloc(sizeof(node));
    head->data = a[0];
    node *cur = head;
    for(int i = 1; i < 7; i++)
    {
        cur->next = (node*) Malloc(sizeof(node));
        cur = cur->next;
        cur->data = a[i];
        cur->next = NULL;
    }

    cur = head;
    while(cur)
    {
        MainRenderer.Printf("%u ", cur->data);
        cur = cur->next;
    }

    MainRenderer.Printf("\nKernel ended.\n");

    while(true);
}