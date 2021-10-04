#pragma once

class Env
{
    public:
    const unsigned char *Kernel;
    const unsigned char *Screen;
    const unsigned char *Timezone;

    void ParseEnvironemnt(unsigned char *environment);
};

extern Env Environment;