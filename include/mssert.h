/*
 * mssert - assertion header for embedded systems
 * Copyright (C) 2020 Daniel Haase
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at
 * https://www.boost.org/LICENSE_1_0.txt
 *
 * File: mssert.h
 * Author: Daniel Haase
 *
 */

#ifndef _MSSERT_H_
#define _MSSERT_H_

#ifndef MSSERT_VERSION
#ifdef VERSION
#define MSSERT_VERSION VERSION
#else
#define MSSERT_VERSION "0.1.0"
#endif /* VERSION */
#endif /* MSSERT_VERSION */

#ifndef NDEBUG
#ifdef MSSERT

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef assert
#undef assert
#endif /* assert */

#ifndef MSSERT_BUFFER_SIZE
#define MSSERT_BUFFER_SIZE 32
#endif /* MSSERT_BUFFER_SIZE */

#ifndef ms_abort
#define ms_abort
#endif

#define MS_VERBAL(E) #E

char mssert_buffer[MSSERT_BUFFER_SIZE];
unsigned int mssert_fail = (unsigned int)0;

#define ms_success (!(mssert_fail))

static __attribute__((pure)) unsigned int ms_strlen(const char *str)
{
	unsigned int len = 0;
	while(*(str++)) ++len;
	return len;
}

static void ms_strncpy(char *dst, const char *src, int len)
{
	while(*src && len--) *(dst++) = *(src++);
	*(dst++) = 0;
}

static void ms_assert_impl(const char *verb, const char *line)
{
	++mssert_fail;

	if(MSSERT_BUFFER_SIZE > 0)
	{
		unsigned int len = ms_strlen(line);
		char code[2] = {58, 32};
		ms_strncpy(mssert_buffer, line, len);
		ms_strncpy((mssert_buffer + len), code, (MSSERT_BUFFER_SIZE - len - 1));
		ms_strncpy((mssert_buffer + len + 2), verb, (MSSERT_BUFFER_SIZE - len - 3));
	}

	ms_abort
}

#define MS_LINENO(L) MS_VERBAL(L)
#define ms_assert(E) if(!(E)) ms_assert_impl(MS_VERBAL(E), MS_LINENO(__LINE__))
#define ms_reset mssert_buffer[0] = mssert_fail = 0;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MSSERT */
#endif /* NDEBUG */

#if defined(NDEBUG) || !defined(MSSERT)

#ifdef ms_abort
#undef ms_abort
#define ms_abort
#else
#define ms_abort
#endif /* ms_abort */

#define ms_assert(E)
#define ms_success (!0)
#define ms_reset

#endif /* defined(NDEBUG) || !defined(MSSERT) */

#endif /* _MSSERT_H_ */
