/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Bugfixes provided by:
 *     Gr�goire Dooms <dooms@info.ucl.ac.be>
 *
 *  Copyright:
 *     Guido Tack, 2004
 *     Christian Schulte, 2004
 *
 *  Last modified:
 *     $Date: 2010-03-03 17:32:21 +0100 (Wed, 03 Mar 2010) $ by $Author: schulte $
 *     $Revision: 10364 $
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

namespace Gecode { namespace Set { namespace Rel {

  template<class View0, class View1>
  forceinline
  ReEq<View0,View1>::ReEq(Home home, View0 y0, View1 y1,
                          Gecode::Int::BoolView y2)
    : Propagator(home), x0(y0), x1(y1), b(y2) {
    b.subscribe(home,*this, Gecode::Int::PC_INT_VAL);
    x0.subscribe(home,*this, PC_SET_ANY);
    x1.subscribe(home,*this, PC_SET_ANY);
  }

  template<class View0, class View1>
  forceinline
  ReEq<View0,View1>::ReEq(Space& home, bool share, ReEq& p)
    : Propagator(home,share,p) {
    x0.update(home,share,p.x0);
    x1.update(home,share,p.x1);
    b.update(home,share,p.b);
  }

  template<class View0, class View1>
  PropCost
  ReEq<View0,View1>::cost(const Space&, const ModEventDelta&) const {
    return PropCost::ternary(PropCost::LO);
  }

  template<class View0, class View1>
  forceinline size_t
  ReEq<View0,View1>::dispose(Space& home) {
    b.cancel(home,*this, Gecode::Int::PC_INT_VAL);
    x0.cancel(home,*this, PC_SET_ANY);
    x1.cancel(home,*this, PC_SET_ANY);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }

  template<class View0, class View1>
  ExecStatus
  ReEq<View0,View1>::post(Home home, View0 x0, View1 x1,
                            Gecode::Int::BoolView b) {
    (void) new (home) ReEq<View0,View1>(home,x0,x1,b);
    return ES_OK;
  }

  template<class View0, class View1>
  Actor*
  ReEq<View0,View1>::copy(Space& home, bool share) {
    return new (home) ReEq<View0,View1>(home,share,*this);
  }

  template<class View0, class View1>
  ExecStatus
  ReEq<View0,View1>::propagate(Space& home, const ModEventDelta&) {
    if (b.one())
      GECODE_REWRITE(*this,(Eq<View0,View1>::post(home(*this),x0,x1)));
    if (b.zero())
      GECODE_REWRITE(*this,(Distinct<View0,View1>::post(home(*this),x0,x1)));

    if (x0.assigned() && x1.assigned()) {
      // directly test x0==x1
      GlbRanges<View0> x0lb(x0);
      GlbRanges<View1> x1lb(x1);
      for (; x0lb() && x1lb(); ++x0lb, ++x1lb) {
        if (x0lb.min() != x1lb.min() ||
            x0lb.max() != x1lb.max()) {
          GECODE_ME_CHECK(b.zero_none(home));
          return home.ES_SUBSUMED(*this);
        }
      }
      if (!x0lb() && !x1lb()) {
        GECODE_ME_CHECK(b.one_none(home));
        return home.ES_SUBSUMED(*this);
      } else {
        GECODE_ME_CHECK(b.zero_none(home));
        return home.ES_SUBSUMED(*this);
      }
    }

    // check whether cardinalities still allow equality
    if (x0.cardMin() > x1.cardMax() ||
        x1.cardMin() > x0.cardMax()) {
      GECODE_ME_CHECK(b.zero_none(home));
      return home.ES_SUBSUMED(*this);
    }

    // check glb(x0) subset lub(x1)
    GlbRanges<View0> x0lb(x0);
    LubRanges<View1> x1ub(x1);
    Iter::Ranges::Diff<GlbRanges<View0>, LubRanges<View1> > diff1(x0lb, x1ub);
    if ( diff1() ) {
      GECODE_ME_CHECK(b.zero_none(home));
      return home.ES_SUBSUMED(*this);
    }

    // check glb(x1) subset lub(x0)
    GlbRanges<View1> x1lb(x1);
    LubRanges<View0> x0ub(x0);
    Iter::Ranges::Diff<GlbRanges<View1>, LubRanges<View0> > diff2(x1lb, x0ub);
    if ( diff2() ) {
      GECODE_ME_CHECK(b.zero_none(home));
      return home.ES_SUBSUMED(*this);
    }

    return ES_FIX;
  }

}}}

// STATISTICS: set-prop
