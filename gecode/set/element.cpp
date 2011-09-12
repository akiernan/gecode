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
 *     $Date: 2010-06-07 14:18:14 +0200 (Mon, 07 Jun 2010) $ by $Author: tack $
 *     $Revision: 11048 $
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

#include <gecode/set/element.hh>

namespace Gecode {

  namespace Int { namespace Element {
    template<>
    /// VarArg type for constant %Set views
    class ViewToVarArg<Gecode::Set::ConstSetView> {
    public:
      typedef IntSetArgs argtype;
    };
  }}

  using namespace Gecode::Set;

  void
  element(Home home, SetOpType op, const SetVarArgs& x, SetVar y, SetVar z,
    const IntSet& universe) {
    if (home.failed()) return;
    
    switch (op) {
    case SOT_DUNION:
      {
        Set::Element::ElementDisjoint<SetView,SetView>::IdxViewArray
          iv(home, x);
        GECODE_ES_FAIL((Element::ElementDisjoint<SetView,SetView>::
                        post(home,iv,y)));
      }
      // fall through
    case SOT_UNION:
      {
        Set::Element::ElementUnion<SetView,SetView>::IdxViewArray iv(home, x);
        GECODE_ES_FAIL(
                       (Element::ElementUnion<SetView,SetView>::
                        post(home,z,iv,y)));
      }
      break;
    case SOT_INTER:
      {
        Set::Element::ElementIntersection<SetView,SetView>::IdxViewArray
          iv(home, x);
        GECODE_ES_FAIL(
                       (Element::ElementIntersection<SetView,SetView>::
                        post(home,z,iv,y,universe)));
      }
      break;
    case SOT_MINUS:
      throw IllegalOperation("Set::element");
      break;
    default:
      throw UnknownOperation("Set::element");
    }
  }

  void
  element(Home home, SetOpType op, const IntSetArgs& x, SetVar y, SetVar z,
    const IntSet& universe) {
    if (home.failed()) return;

    switch (op) {
    case SOT_DUNION:
      {
        Set::Element::ElementDisjoint<ConstSetView,SetView>::IdxViewArray
          iv(home, x.size());
        for (int i=x.size(); i--;) {
          iv[i].idx = i; iv[i].view = ConstSetView(home, x[i]);
        }
        GECODE_ES_FAIL((Element::ElementDisjoint<ConstSetView,SetView>::
                        post(home,iv,y)));
      }
      // fall through
    case SOT_UNION:
      {
        SharedArray<IntSet> s(x.size());
        for (int i=s.size(); i--;)
          new (&s[i]) IntSet(x[i]);
        GECODE_ES_FAIL(
                       (Element::ElementUnionConst<SetView,SetView>::
                        post(home,z,s,y)));
      }
      break;
    case SOT_INTER:
      {
        Set::Element::ElementIntersection<ConstSetView,SetView>::IdxViewArray
          iv(home, x.size());
        for (int i=x.size(); i--;) {
          iv[i].idx = i; iv[i].view = ConstSetView(home, x[i]);
        }
        GECODE_ES_FAIL(
                       (Element::ElementIntersection<ConstSetView,SetView>::
                        post(home,z,iv,y,universe)));
      }
      break;
    case SOT_MINUS:
      throw IllegalOperation("Set::element");
      break;
    default:
      throw UnknownOperation("Set::element");
    }
    
  }

  void
  element(Home home, const SetVarArgs& x, IntVar y, SetVar z) {
    if (x.size() == 0)
      throw Set::TooFewArguments("Set::element");
    if (home.failed()) return;
    Set::Element::ElementUnion<SetView,SingletonView>::IdxViewArray iv(home, x);
    SetView zv(z);

    Int::IntView yv(y);
    SingletonView single(yv);
    GECODE_ES_FAIL((Element::ElementUnion<SetView,
                         SingletonView>::post(home, z, iv, single)));
  }

  void
  element(Home home, const IntSetArgs& s, IntVar y, SetVar z) {
    if (s.size() == 0)
      throw Set::TooFewArguments("Set::element");
    for (int i=s.size(); i--;)
      Set::Limits::check(s[i], "Set::element");
    if (home.failed()) return;
    SetView zv(z);
    SharedArray<IntSet> x(s.size());
    for (int i=s.size(); i--;)
      new (&x[i]) IntSet(s[i]);

    Int::IntView yv(y);
    SingletonView single(yv);
    GECODE_ES_FAIL((Element::ElementUnionConst<SetView,
                         SingletonView>::post(home, z, x, single)));
  }


  namespace {
    IntVar
    pair(Home home, IntVar x, int w, IntVar y, int h) {
      IntVar xy(home,0,w*h-1);
      if (Int::Element::Pair::post(home,x,y,xy,w,h) != ES_OK)
        home.fail();
      return xy;
    }
  }

  void
  element(Home home, const IntSetArgs& a, 
          IntVar x, int w, IntVar y, int h, SetVar z) {
    if (a.size() == 0)
      throw Set::TooFewArguments("Set::element");
    if (a.size() != w*h)
      throw Set::ArgumentSizeMismatch("Set::element");
    if (home.failed()) return;
    element(home, a, pair(home,x,w,y,h), z);
  }

  void
  element(Home home, const SetVarArgs& a, 
          IntVar x, int w, IntVar y, int h, SetVar z) {
    if (a.size() == 0)
      throw Set::TooFewArguments("Set::element");
    if (a.size() != w*h)
      throw Set::ArgumentSizeMismatch("Set::element");
    if (home.failed()) return;
    element(home, a, pair(home,x,w,y,h), z);
  }

}

// STATISTICS: set-post

