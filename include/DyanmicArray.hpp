#pragma once

// Niche variety.
template <typename T>
struct DynamicArray
{
    T *Array;
    int Capacity;

    DynamicArray();
    int Add(T element);
};

#include <Memory/Memory.hpp>
#include <Memory/Heap.hpp>

template <typename T>
DynamicArray<T>::DynamicArray() : Capacity(8)
{
    Array = (T*) KernelHeap.Malloc(8 * sizeof(T));
    memset(Array, 0, 8 * sizeof(T));
}

template <typename T>
int DynamicArray<T>::Add(T element)
{
    for(int i = 0; i < Capacity; i++)
    {
        if(!Array[i])
        {
            Array[i] = element;
            return i;
        }
    }

    T *newLoc = (T*) KernelHeap.Malloc(Capacity * 2 * sizeof(T));
    memset(newLoc, 0, Capacity * 2 * sizeof(T));
    memcpy(Array, newLoc, Capacity * sizeof(T));
    KernelHeap.Free(Array);
    Array = newLoc;
    Array[Capacity] = element;
    Capacity *= 2;
    return Capacity / 2;
}