#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <Arduino.h>

void initFileSystem();
void resetData();
void saveTableToFile(String table);

#endif
