#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

#include "StatData.h"

void joinDump(const char* pathToFile);