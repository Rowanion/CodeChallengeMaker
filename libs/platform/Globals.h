/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#ifdef STRINGIFY
#undef STRINGIFY
#endif
#ifdef STRINGIFY2
#undef STRINGIFY2
#endif

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)
