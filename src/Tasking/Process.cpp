#include <Tasking/Process.hpp>

Process *ProcessTop;

void PushProcess(void *pc, void *stackTop)
{
    Process p;
    p.PC = pc;
    p.StackTop = stackTop;
    p.ProcessID = (ProcessTop - 1)->ProcessID + 1;

    *ProcessTop = p;
    ProcessTop += 1;
}


Process PopProcess()
{
    ProcessTop -= 1;
    return *ProcessTop;
}