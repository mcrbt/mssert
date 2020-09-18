/*
 * mssert - assertion header for embedded systems
 * Copyright (C) 2020 Daniel Haase
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at
 * https://www.boost.org/LICENSE_1_0.txt
 *
 * File: mssert.cpp
 * Author: Daniel Haase
 *
 */

#include <iostream>

#define ms_abort std::cout << mssert_buffer << std::endl;
#include <mssert.h>

__attribute__((const)) int main(void)
{
  void *ptr = ((void *)0);
  std::cout << "mssert version " << MSSERT_VERSION << std::endl;

  ms_assert(1 == 1);
  ms_assert(!ptr);
  ms_assert((19 - 4 + 892) != 907);

  ms_reset
  std::cout << (ms_success ? "successful" : "failed") << std::endl;
  return 0;
}
