/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Contributing authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2004
 *     Guido Tack, 2004
 *
 *  Last modified:
 *     $Date: 2011-05-11 12:44:17 +0200 (Wed, 11 May 2011) $ by $Author: tack $
 *     $Revision: 12001 $
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

#ifndef __GECODE_KERNEL_HH__
#define __GECODE_KERNEL_HH__

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <gecode/support.hh>

/*
 * Configure linking
 *
 */
#if !defined(GECODE_STATIC_LIBS) && \
    (defined(__CYGWIN__) || defined(__MINGW32__) || defined(_MSC_VER))

#ifdef GECODE_BUILD_KERNEL
#define GECODE_KERNEL_EXPORT __declspec( dllexport )
#else
#define GECODE_KERNEL_EXPORT __declspec( dllimport )
#endif

#else

#ifdef GECODE_GCC_HAS_CLASS_VISIBILITY
#define GECODE_KERNEL_EXPORT __attribute__ ((visibility("default")))
#else
#define GECODE_KERNEL_EXPORT
#endif

#endif

// Configure auto-linking
#ifndef GECODE_BUILD_KERNEL
#define GECODE_LIBRARY_NAME "Kernel"
#include <gecode/support/auto-link.hpp>
#endif

/**
 * \namespace Gecode
 * \brief %Gecode toplevel namespace
 *
 * The Gecode namespace contains nested namespaces for
 * the various submodules (for example Int for the
 * definition of integer propagator classes). Functionality
 * that is used for interfacing (search engines, variables,
 * and so on) or belongs to the %Gecode %Kernel is contained
 * directly in the Gecode namespace.
 *
 */

/*
 * General exceptions and kernel exceptions
 *
 */

#include <gecode/kernel/exception.hpp>



/*
 * Basic kernel services
 *
 */

#include <gecode/kernel/memory-config.hpp>
#include <gecode/kernel/memory-manager.hpp>


/*
 * Macros for checking failure
 *
 */

#include <gecode/kernel/macros.hpp>


/*
 * Gecode kernel
 *
 */

#include <gecode/kernel/archive.hpp>
#include <gecode/kernel/global-prop-info.hpp>
#include <gecode/kernel/core.hpp>
#include <gecode/kernel/modevent.hpp>
#include <gecode/kernel/range-list.hpp>

/*
 * Region memory management
 *
 */

#include <gecode/kernel/region.hpp>


/*
 * Variables and testing for shared variables
 *
 */

#include <gecode/kernel/var.hpp>


/*
 * Views
 *
 */

#include <gecode/kernel/view.hpp>


/*
 * Arrays
 *
 */

#include <gecode/kernel/array.hpp>
#include <gecode/kernel/shared-array.hpp>


/*
 * Common propagator and branching patterns
 *
 */

#include <gecode/kernel/propagator.hpp>
#include <gecode/kernel/advisor.hpp>
#include <gecode/kernel/branch.hpp>
#include <gecode/kernel/brancher.hpp>
#include <gecode/kernel/brancher-view.hpp>
#include <gecode/kernel/brancher-tiebreak.hpp>
#include <gecode/kernel/brancher-val.hpp>


/*
 * Automatically generated variable implementations
 *
 */

#include <gecode/kernel/var-imp.hpp>

/*
 * Allocator support
 *
 */

#include <gecode/kernel/allocators.hpp>

#endif

// STATISTICS: kernel-other
