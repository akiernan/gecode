/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Mikael Lagerkvist <lagerkvist@gecode.org>
 *
 *  Copyright:
 *     Mikael Lagerkvist, 2006
 *
 *  Last modified:
 *     $Date: 2010-04-08 12:35:31 +0200 (Thu, 08 Apr 2010) $ by $Author: schulte $
 *     $Revision: 10684 $
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

#ifndef __GECODE_TEST_BRANCH_HH__
#define __GECODE_TEST_BRANCH_HH__

#include <gecode/kernel.hh>

#include <gecode/int.hh>

#ifdef GECODE_HAS_SET_VARS
#include <gecode/set.hh>
#endif

#include "test/test.hh"

namespace Test {

  /// Tests for branchings
  namespace Branch {

    /**
     * \brief %Base class for tests for branching on integer variables
     *
     */
    class IntTest : public Base {
    protected:
      /// Number of variables
      int arity;
      /// Domain of variables
      Gecode::IntSet dom;
    public:
      /// Construct and register test
      IntTest(const std::string& s, int a, const Gecode::IntSet& d);
      /// Perform test
      virtual bool run(void);
      /// Post propagators on variables \a x
      virtual void post(Gecode::Space& home, Gecode::IntVarArray& x) = 0;
    };

    /**
     * \brief %Base class for tests for branching on Boolean variables
     *
     */
    class BoolTest : public Base {
    protected:
      /// Number of variables
      int arity;
    public:
      /// Construct and register test
      BoolTest(const std::string& s, int a);
      /// Perform test
      virtual bool run(void);
      /// Post propagators on variables \a x
      virtual void post(Gecode::Space& home, Gecode::BoolVarArray& x) = 0;
    };

#ifdef GECODE_HAS_SET_VARS
    /**
     * \brief %Base class for tests for branching on set variables
     *
     */
    class SetTest : public Base {
    protected:
      /// Number of variables
      int arity;
      /// Domain of variables
      Gecode::IntSet dom;
    public:
      /// Construct and register test
      SetTest(const std::string& s, int a, const Gecode::IntSet& d);
      /// Perform test
      virtual bool run(void);
      /// Post propagators on variables \a x
      virtual void post(Gecode::Space& home, Gecode::SetVarArray& x) = 0;
    };
#endif

  }

}

#endif

// STATISTICS: test-branch
