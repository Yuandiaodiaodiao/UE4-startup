#pragma once

#include "CoreMinimal.h"
#define LOGWARNING(FORMAT,...) UE_LOG(LogTemp,Warning,TEXT(FORMAT),##__VA_ARGS__)
#define LOGERROR(FORMAT,...) UE_LOG(LogTemp,Error,TEXT(FORMAT),##__VA_ARGS__)