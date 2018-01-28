#ifndef LOGO_H
#define LOGO_H
#include<can.h>

class Logo
{
public:
    Logo();
    void read();
    unsigned char isLogo;
};

#endif // LOGO_H
