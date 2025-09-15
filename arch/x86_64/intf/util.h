#pragma once
#include "stdint.h"

void memset(void *dest, char val, uint32_t count);

char inPortB(uint16_t port);
void outPortB(uint16_t port, uint8_t value);