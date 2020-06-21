#ifndef STATICMEMBER_H
#define STATICMEMBER_H

#include "QDebug"
#include "i2cwrite.h"

void myCallback_1 (bool _retVal);
void myCallback_2 (bool _retVal);

extern I2CWrite *g_i2cWriteLayer;


#endif // STATICMEMBER_H
