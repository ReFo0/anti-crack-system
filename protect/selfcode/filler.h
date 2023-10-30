#pragma once

namespace filler {
static_assert(sizeof(double) == 0x8, "Unexpected double size, PE sections will be misaligned.\n");
const int FILL_SIZE = 7200;
extern const double rdata[FILL_SIZE];
void text();
} 