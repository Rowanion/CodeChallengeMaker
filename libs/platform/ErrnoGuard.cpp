/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#include "ErrnoGuard.h"

#include <cerrno>

namespace platform {

ErrnoGuard::ErrnoGuard() : m_oldErrno(errno) {}
ErrnoGuard::~ErrnoGuard()
{
   errno = m_oldErrno;
}

}  // namespace platform