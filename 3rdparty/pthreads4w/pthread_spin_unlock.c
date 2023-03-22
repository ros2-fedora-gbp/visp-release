/*
 * pthread_spin_unlock.c
 *
 * Description:
 * This translation unit implements spin lock primitives.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads4w - POSIX Threads for Windows
 *      Copyright 1998 John E. Bossom
 *      Copyright 1999-2018, Pthreads4w contributors
 *
 *      Homepage: https://sourceforge.net/projects/pthreads4w/
 *
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *
 *      https://sourceforge.net/p/pthreads4w/wiki/Contributors/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "pthread.h"
#include "implement.h"


int
pthread_spin_unlock (pthread_spinlock_t * lock)
{
  register pthread_spinlock_t s;

  if (NULL == lock || NULL == *lock)
    {
      return (EINVAL);
    }

  s = *lock;

  if (s == PTHREAD_SPINLOCK_INITIALIZER)
    {
      return EPERM;
    }

  switch ((long)
	   __PTW32_INTERLOCKED_COMPARE_EXCHANGE_LONG  ((__PTW32_INTERLOCKED_LONGPTR) &s->interlock,
					       (__PTW32_INTERLOCKED_LONG)  __PTW32_SPIN_UNLOCKED,
					       (__PTW32_INTERLOCKED_LONG)  __PTW32_SPIN_LOCKED))
    {
    case  __PTW32_SPIN_LOCKED:
    case  __PTW32_SPIN_UNLOCKED:
      return 0;
    case  __PTW32_SPIN_USE_MUTEX:
      return pthread_mutex_unlock (&(s->u.mutex));
    }

  return EINVAL;
}
