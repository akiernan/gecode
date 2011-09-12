/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2009
 *
 *  Last modified:
 *     $Date: 2011-04-28 14:05:18 +0200 (Thu, 28 Apr 2011) $ by $Author: tack $
 *     $Revision: 11968 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifdef GECODE_HAS_UNISTD_H
#include <unistd.h>
#endif

namespace Gecode { namespace Support {

  /*
   * Mutex
   */
  forceinline
  Mutex::Mutex(void) {
    if (pthread_mutex_init(&p_m,NULL) != 0)
      throw OperatingSystemError("Mutex::Mutex[pthread_mutex_init]");
  }
  forceinline void
  Mutex::acquire(void) {
    if (pthread_mutex_lock(&p_m) != 0)
      throw OperatingSystemError("Mutex::acquire[pthread_mutex_lock]");
  }
  forceinline bool
  Mutex::tryacquire(void) {
    return pthread_mutex_trylock(&p_m) == 0;
  }
  forceinline void
  Mutex::release(void) {
    if (pthread_mutex_unlock(&p_m) != 0)
      throw OperatingSystemError("Mutex::release[pthread_mutex_unlock]");
  }
  forceinline
  Mutex::~Mutex(void) {
    if (pthread_mutex_destroy(&p_m) != 0)
      throw OperatingSystemError("Mutex::~Mutex[pthread_mutex_destroy]");
  }


  /*
   * Event
   */
  forceinline
  Event::Event(void) : p_s(false) {
    if (pthread_mutex_init(&p_m,NULL) != 0)
      throw OperatingSystemError("Event::Event[pthread_mutex_init]");
    if (pthread_cond_init(&p_c,NULL) != 0)
      throw OperatingSystemError("Event::Event[pthread_cond_init]");
  }
  forceinline void
  Event::signal(void) {
    if (pthread_mutex_lock(&p_m) != 0)
      throw OperatingSystemError("Event::signal[pthread_mutex_lock]");
    if (!p_s) {
      p_s = true;
      if (pthread_cond_signal(&p_c) != 0)
        throw OperatingSystemError("Event::signal[pthread_cond_signal]");
    }
    if (pthread_mutex_unlock(&p_m) != 0)
      throw OperatingSystemError("Event::signal[pthread_mutex_unlock]");
  }
  forceinline void
  Event::wait(void) {
    if (pthread_mutex_lock(&p_m) != 0)
      throw OperatingSystemError("Event::wait[pthread_mutex_lock]");
    while (!p_s)
      if (pthread_cond_wait(&p_c,&p_m) != 0)
        throw OperatingSystemError("Event::wait[pthread_cond_wait]");
    p_s = false;
    if (pthread_mutex_unlock(&p_m) != 0)
      throw OperatingSystemError("Event::wait[pthread_mutex_unlock]");
  }
  forceinline
  Event::~Event(void) {
    if (pthread_cond_destroy(&p_c) != 0)
      throw OperatingSystemError("Event::~Event[pthread_cond_destroy]");
    if (pthread_mutex_destroy(&p_m) != 0)
      throw OperatingSystemError("Event::~Event[pthread_mutex_destroy]");
  }


  /*
   * Thread
   */
  forceinline void
  Thread::sleep(unsigned int ms) {
#ifdef GECODE_HAS_UNISTD_H
    unsigned int s = ms / 1000;
    ms -= 1000 * s;
    if (s > 0) {
      // More than one million microseconds, use sleep
      ::sleep(s);
    }
    usleep(ms * 1000);
#endif
  }
  forceinline unsigned int
  Thread::npu(void) {
#ifdef GECODE_HAS_UNISTD_H
    int n=static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
    return (n>1) ? n : 1;
#else
    return 1;
#endif
  }

}}

// STATISTICS: support-any
