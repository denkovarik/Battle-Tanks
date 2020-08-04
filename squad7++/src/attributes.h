#ifndef __ATTRIBUTES_
#define __ATTRIBUTES_

struct attributes
{
    int tankHealth;
    int tankDamage;
    int tankAP;
    int tankShots;
    int tankAmmo;
    int tankRadar;
    int tankRange;

    attributes( int tH, int tD, int tR, int tS, int tV ) : tankHealth(tH), 
    tankDamage(tD), tankAP(tR), tankShots(tS), tankRadar(tV) {}
    
    attributes(): tankHealth(0), tankDamage(0), tankAP(0), tankShots(0), tankAmmo(0), tankRadar(0), tankRange(0){}

};

#endif
