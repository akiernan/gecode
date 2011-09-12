/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
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

namespace Gecode { namespace Int { namespace Dom {

  template<class View>
  forceinline
  ReIntSet<View>::ReIntSet
  (Home home, View x, const IntSet& s, BoolView b)
    : ReUnaryPropagator<View,PC_INT_DOM,BoolView>(home,x,b), is(s) {
    home.notice(*this,AP_DISPOSE);
  }

  template<class View>
  forceinline size_t
  ReIntSet<View>::dispose(Space& home) {
    home.ignore(*this,AP_DISPOSE);
    is.~IntSet();
    (void) ReUnaryPropagator<View,PC_INT_DOM,BoolView>::dispose(home);
    return sizeof(*this);
  }

  template<class View>
  ExecStatus
  ReIntSet<View>::post(Home home, View x, const IntSet& s, BoolView b) {
    if (s.ranges() == 0) {
      GECODE_ME_CHECK(b.zero(home));
    } else if (s.ranges() == 1) {
      return ReRange<View>::post(home,x,s.min(),s.max(),b);
    } else {
      (void) new (home) ReIntSet<View>(home,x,s,b);
    }
    return ES_OK;
  }


  template<class View>
  forceinline
  ReIntSet<View>::ReIntSet(Space& home, bool share, ReIntSet& p)
    : ReUnaryPropagator<View,PC_INT_DOM,BoolView>(home,share,p) {
    is.update(home,share,p.is);
  }

  template<class View>
  Actor*
  ReIntSet<View>::copy(Space& home, bool share) {
    return new (home) ReIntSet(home,share,*this);
  }

  template<class View>
  ExecStatus
  ReIntSet<View>::propagate(Space& home, const ModEventDelta&) {
    IntSetRanges i_is(is);
    if (b.one()) {
      GECODE_ME_CHECK(x0.inter_r(home,i_is,false));
      return home.ES_SUBSUMED(*this);
    }
    if (b.zero()) {
      GECODE_ME_CHECK(x0.minus_r(home,i_is,false));
      return home.ES_SUBSUMED(*this);
    }

    {
      ViewRanges<View> i_x(x0);

      switch (Iter::Ranges::compare(i_x,i_is)) {
      case Iter::Ranges::CS_SUBSET:
        GECODE_ME_CHECK(b.one_none(home));
        return home.ES_SUBSUMED(*this);
      case Iter::Ranges::CS_DISJOINT:
        GECODE_ME_CHECK(b.zero_none(home));
        return home.ES_SUBSUMED(*this);
      case Iter::Ranges::CS_NONE:
        break;
      default: GECODE_NEVER;
      }
    }
    return ES_FIX;
  }

}}}

// STATISTICS: int-prop

