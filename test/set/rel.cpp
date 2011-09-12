/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2005
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

#include "test/set.hh"

using namespace Gecode;

namespace Test { namespace Set {

  /// %Tests for relation constraints
  namespace Rel {

    /**
      * \defgroup TaskTestSetRel Relation constraints
      * \ingroup TaskTestSet
      */
    //@{

    static IntSet ds_33(-3,3);

    /// %Test for binary set relation constraint
    class RelBin : public SetTest {
    private:
      Gecode::SetRelType srt;
      bool shared;
    public:
      /// Create and register test
      RelBin(Gecode::SetRelType srt0, bool shared0)
        : SetTest("Rel::Bin::"+str(srt0)+"::S"+(shared0 ? "1":"0"),
                  shared ? 1 : 2,ds_33,true)
        , srt(srt0), shared(shared0){}
      /// %Test whether \a x is solution
      bool solution(const SetAssignment& x) const {
        int x1 = shared ? x[0] : x[1];
        CountableSetRanges xr0(x.lub, x[0]);
        CountableSetRanges xr1(x.lub, x1);
        switch (srt) {
        case SRT_EQ: return Iter::Ranges::equal(xr0, xr1);
        case SRT_NQ: return !Iter::Ranges::equal(xr0, xr1);
        case SRT_SUB: return Iter::Ranges::subset(xr0, xr1);
        case SRT_SUP: return Iter::Ranges::subset(xr1, xr0);
        case SRT_DISJ:
          {
            Iter::Ranges::Inter<CountableSetRanges,CountableSetRanges>
              inter(xr0,xr1);
            return !inter();
          }
        case SRT_CMPL:
          {
            Gecode::Set::RangesCompl<CountableSetRanges> rc(xr0);
            return Iter::Ranges::equal(rc,xr1);
          }
        default:
          GECODE_NEVER;
        }
        GECODE_NEVER;
        return false;
      }
      /// Post constraint on \a x
      void post(Space& home, SetVarArray& x, IntVarArray&) {
        if (!shared)
          Gecode::rel(home, x[0], srt, x[1]);
        else
          Gecode::rel(home, x[0], srt, x[0]);
      }
      /// Post reified constraint on \a x for \a b
      void post(Space& home, SetVarArray& x, IntVarArray&, BoolVar b) {
        if (!shared)
          Gecode::rel(home, x[0], srt, x[1], b);
        else
          Gecode::rel(home, x[0], srt, x[0], b);
      }
    };
    RelBin _relbin_eq(Gecode::SRT_EQ,false);
    RelBin _relbin_nq(Gecode::SRT_NQ,false);
    RelBin _relbin_sub(Gecode::SRT_SUB,false);
    RelBin _relbin_sup(Gecode::SRT_SUP,false);
    RelBin _relbin_disj(Gecode::SRT_DISJ,false);
    RelBin _relbin_cmpl(Gecode::SRT_CMPL,false);
    RelBin _relbin_shared_eq(Gecode::SRT_EQ,true);
    RelBin _relbin_shared_nq(Gecode::SRT_NQ,true);
    RelBin _relbin_shared_sub(Gecode::SRT_SUB,true);
    RelBin _relbin_shared_sup(Gecode::SRT_SUP,true);
    RelBin _relbin_shared_disj(Gecode::SRT_DISJ,true);
    RelBin _relbin_shared_cmpl(Gecode::SRT_CMPL,true);

    //@}

}}}

// STATISTICS: test-set
