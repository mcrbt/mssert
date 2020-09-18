/*
 * mssert - assertion header for embedded systems
 * Copyright (C) 2020 Daniel Haase
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at
 * https://www.boost.org/LICENSE_1_0.txt
 *
 * File: mssert.c
 * Author: Daniel Haase
 *
 */

#include <stdio.h>

#define ms_abort printf("%s\n", mssert_buffer);
#include <mssert.h>

__attribute__((const)) int main(void)
{
  void *ptr = ((void *)0);
  printf("mssert version %s\n", MSSERT_VERSION);

  ms_assert(1 == 1);
  ms_assert(!ptr);
  ms_assert((19 - 4 + 892) != 907);

  ms_reset
  printf("%s\n", (ms_success ? "successful" : "failed"));
  return 0;
}
