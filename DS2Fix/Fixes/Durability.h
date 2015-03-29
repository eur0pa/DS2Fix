#pragma once
#include "stdafx.h"

typedef void(__stdcall *pSetDurability)();
static pSetDurability oSetDurability = nullptr;
void __stdcall tSetDurability();