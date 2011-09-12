/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
 *     Christian Schulte, 2004
 *
 *  Last modified:
 *     $Date: 2011-05-02 00:24:42 +0200 (Mon, 02 May 2011) $ by $Author: tack $
 *     $Revision: 11973 $
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

#include <gecode/set.hh>

#include <gecode/set/int.hh>
#include <gecode/set/rel.hh>

using namespace Gecode::Int;

namespace Gecode {

  void
  rel(Home home, SetVar s, IntRelType r, IntVar x) {
    if (home.failed()) return;
    switch (r) {
    case IRT_EQ:
      {
        Gecode::Int::IntView xv(x);
        Set::SingletonView xsingle(xv);
        GECODE_ES_FAIL(
                       (Set::Rel::Eq<Set::SetView,Set::SingletonView>
                        ::post(home,s,xsingle)));

      }
      break;
    case IRT_NQ:
      {
        Gecode::Set::SetView sv(s);
        GECODE_ME_FAIL( sv.cardMin(home, 1));
        Gecode::Int::IntView xv(x);
        Set::SingletonView xsingle(xv);
        GECODE_ES_FAIL(
                       (Set::Rel::NoSubset<Set::SingletonView,Set::SetView>
                        ::post(home,xsingle,sv)));

      }
      break;
    case IRT_LQ:
      {
        IntVar tmp(home, Int::Limits::min, Int::Limits::max);
        rel(home, tmp, IRT_LQ, x);
        GECODE_ES_FAIL(Set::Int::MaxElement<Set::SetView>::post(home,s,tmp));
      }
      break;
    case IRT_LE:
      {
        IntVar tmp(home, Int::Limits::min, Int::Limits::max);
        rel(home, tmp, IRT_LE, x);
        GECODE_ES_FAIL(Set::Int::MaxElement<Set::SetView>::post(home,s,tmp));
      }
      break;
    case IRT_GQ:
      {
        IntVar tmp(home, Int::Limits::min, Int::Limits::max);
        rel(home, tmp, IRT_GQ, x);
        GECODE_ES_FAIL(Set::Int::MinElement<Set::SetView>::post(home,s,tmp));
      }
      break;
    case IRT_GR:
      {
        IntVar tmp(home, Int::Limits::min, Int::Limits::max);
        rel(home, tmp, IRT_GR, x);
        GECODE_ES_FAIL(Set::Int::MinElement<Set::SetView>::post(home,s,tmp));
      }
      break;
    default:
      throw UnknownRelation("Set::rel");
    }

  }

  void
  rel(Home home, IntVar x, IntRelType r, SetVar s) {
    IntRelType rr;
    switch (r) {
    case IRT_LE: rr=IRT_GR; break;
    case IRT_LQ: rr=IRT_GQ; break;
    case IRT_GR: rr=IRT_LE; break;
    case IRT_GQ: rr=IRT_LQ; break;
    default: rr=r;
    }
    rel(home, s, rr, x);
  }

  void
  min(Home home, SetVar s, IntVar x){
    if (home.failed()) return;
    GECODE_ES_FAIL(Set::Int::MinElement<Set::SetView>::post(home,s,x));
  }
  void
  notMin(Home home, SetVar s, IntVar x){
    if (home.failed()) return;
    GECODE_ES_FAIL(Set::Int::NotMinElement<Set::SetView>::post(home,s,x));
  }
  void
  min(Home home, SetVar s, IntVar x, BoolVar b){
    if (home.failed()) return;
    GECODE_ES_FAIL(
                   Set::Int::ReMinElement<Set::SetView>::post(home,s,x,b));
  }
  void
  max(Home home, SetVar s, IntVar x){
    if (home.failed()) return;
    GECODE_ES_FAIL(Set::Int::MaxElement<Set::SetView>::post(home,s,x));
  }
  void
  notMax(Home home, SetVar s, IntVar x){
    if (home.failed()) return;
    GECODE_ES_FAIL(Set::Int::NotMaxElement<Set::SetView>::post(home,s,x));
  }
  void
  max(Home home, SetVar s, IntVar x, BoolVar b){
    if (home.failed()) return;
    GECODE_ES_FAIL(
                   Set::Int::ReMaxElement<Set::SetView>::post(home,s,x,b));
  }

  void
  channelSorted(Home home, const IntVarArgs& x, SetVar y) {
    if (home.failed()) return;
    ViewArray<IntView> xa(home,x);
    GECODE_ES_FAIL(Set::Int::Match<Set::SetView>::post(home,y,xa));
  }

  void
  channel(Home home, const IntVarArgs& x, const SetVarArgs& y) {
    if (home.failed()) return;
    ViewArray<Int::CachedView<Int::IntView> > xa(home,x.size());
    for (int i=x.size(); i--;)
      new (&xa[i]) Int::CachedView<Int::IntView>(x[i]);
    ViewArray<Set::CachedView<Set::SetView> > ya(home,y.size());
    for (int i=y.size(); i--;)
      new (&ya[i]) Set::CachedView<Set::SetView>(y[i]);
    GECODE_ES_FAIL((Set::Int::ChannelInt<Set::SetView>::post(home,xa,ya)));
  }

  void
  channel(Home home, const BoolVarArgs& x, SetVar y) {
    if (home.failed()) return;
    ViewArray<Int::BoolView> xv(home,x);
    GECODE_ES_FAIL((Set::Int::ChannelBool<Set::SetView>
                         ::post(home,xv,y)));
  }

  void weights(Home home, const IntArgs& elements0, const IntArgs& weights0,
               SetVar x, IntVar y) {
    if (home.failed()) return;
    SharedArray<int> elements(elements0.size());
    for (int i=elements0.size(); i--;)
      elements[i] = elements0[i];
    SharedArray<int> weights(weights0.size());
    for (int i=weights0.size(); i--;)
      weights[i] = weights0[i];
    GECODE_ES_FAIL(Set::Int::Weights<Set::SetView>::post(home,elements,
                                                              weights,x,y));
  }

}

// STATISTICS: set-post
