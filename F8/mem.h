#pragma once
#include "stdafx.h"

namespace mem
{
	void PatchMemory(BYTE* dest, BYTE* src, unsigned int size, HANDLE process);
	void NoopMemory(BYTE* dest, unsigned int size, HANDLE process);
}
