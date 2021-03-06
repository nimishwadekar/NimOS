#pragma once

// Subset, only for pointers.
template <typename T>
struct DynamicArray
{
    T *Array;
    int Capacity;
    int Count;

    DynamicArray();
    int Add(T element);
    void Delete(int index);
};

#include <Memory/Memory.hpp>
#include <Memory/Heap.hpp>

template <typename T>
DynamicArray<T>::DynamicArray() : Capacity(8), Count(0)
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
            Count += 1;
            return i;
        }
    }

    T *newLoc = (T*) KernelHeap.Malloc(Capacity * 2 * sizeof(T));
    memset(newLoc, 0, Capacity * 2 * sizeof(T));
    memcpy(Array, newLoc, Capacity * sizeof(T));
    KernelHeap.Free(Array);
    Array = newLoc;
    Array[Capacity] = element;
    Count += 1;
    Capacity *= 2;
    return Capacity / 2;
}

template <typename T>
void DynamicArray<T>::Delete(int index)
{
    delete Array[index];
    Array[index] = nullptr;
    Count -= 1;
}