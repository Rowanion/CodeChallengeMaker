/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

namespace platform {

class ErrnoGuard
{
public:
   ErrnoGuard();
   ~ErrnoGuard();

private:
   int m_oldErrno;
};

}  // namespace platform