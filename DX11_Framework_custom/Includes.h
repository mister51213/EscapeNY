#pragma once

/*
* This is for files that will not be accessing the custom classes
* in our framework, and therefore safe to include anywhere.
*/

#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////


// C++ standard headers
#include <algorithm>
#include <array>
#include <chrono>
#include <fstream>
#include <math.h>
#include <memory>
#include <queue>
#include <random>
#include <stack>
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>

// Win API headers
#include <windows.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dwrite.h>
#include <wincodec.h>

// Microsoft helper headers
#include <DirectXMath.h>
#include <wrl.h>

