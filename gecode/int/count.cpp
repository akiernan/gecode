/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2002
 *
 *  Last modified:
 *     $Date: 2010-06-03 20:34:20 +0200 (Thu, 03 Jun 2010) $ by $Author: schulte $
 *     $Revision: 11017 $
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

#include <gecode/int/count.hh>

namespace Gecode {

  void
  count(Home home, const IntVarArgs& x, int n,
        IntRelType r, int m, IntConLevel) {
    using namespace Int;
    Limits::check(n,"Int::count");
    Limits::check(m,"Int::count");
    if (home.failed()) return;
    ViewArray<IntView> xv(home,x);
    ConstIntView y(n);
    switch (r) {
    case IRT_EQ:
      GECODE_ES_FAIL((Count::EqInt<IntView,ConstIntView>
                           ::post(home,xv,y,m)));
      break;
    case IRT_NQ:
      GECODE_ES_FAIL((Count::NqInt<IntView,ConstIntView>
                           ::post(home,xv,y,m)));
      break;
    case IRT_LE:
      GECODE_ES_FAIL((Count::LqInt<IntView,ConstIntView>
                           ::post(home,xv,y,m-1)));
      break;
    case IRT_LQ:
      GECODE_ES_FAIL((Count::LqInt<IntView,ConstIntView>
                           ::post(home,xv,y,m)));
      break;
    case IRT_GR:
      GECODE_ES_FAIL((Count::GqInt<IntView,ConstIntView>
                           ::post(home,xv,y,m+1)));
      break;
    case IRT_GQ:
      GECODE_ES_FAIL((Count::GqInt<IntView,ConstIntView>
                           ::post(home,xv,y,m)));
      break;
    default:
      throw UnknownRelation("Int::count");
    }
  }

  void
  count(Home home, const IntVarArgs& x, IntVar y,
        IntRelType r, int m, IntConLevel) {
    using namespace Int;
    Limits::check(m,"Int::count");
    if (home.failed()) return;
    ViewArray<IntView> xv(home,x);
    switch (r) {
    case IRT_EQ:
      GECODE_ES_FAIL((Count::EqInt<IntView,IntView>
                           ::post(home,xv,y,m)));
      break;
    case IRT_NQ:
      GECODE_ES_FAIL((Count::NqInt<IntView,IntView>
                           ::post(home,xv,y,m)));
      break;
    case IRT_LE:
      GECODE_ES_FAIL((Count::LqInt<IntView,IntView>
                           ::post(home,xv,y,m-1)));
      break;
    case IRT_LQ:
      GECODE_ES_FAIL((Count::LqInt<IntView,IntView>
                           ::post(home,xv,y,m)));
      break;
    case IRT_GR:
      GECODE_ES_FAIL((Count::GqInt<IntView,IntView>
                           ::post(home,xv,y,m+1)));
      break;
    case IRT_GQ:
      GECODE_ES_FAIL((Count::GqInt<IntView,IntView>
                           ::post(home,xv,y,m)));
      break;
    default:
      throw UnknownRelation("Int::count");
    }
  }

  void
  count(Home home, const IntVarArgs& x, const IntArgs& y,
        IntRelType r, int m, IntConLevel) {
    using namespace Int;
    if (x.size() != y.size())
      throw ArgumentSizeMismatch("Int::count");
    Limits::check(m,"Int::count");
    if (home.failed()) return;

    ViewArray<OffsetView> xy(home,x.size());
    for (int i=x.size(); i--; )
      xy[i] = OffsetView(x[i],-y[i]);

    ZeroIntView z;
    switch (r) {
    case IRT_EQ:
      GECODE_ES_FAIL((Count::EqInt<OffsetView,ZeroIntView>
                           ::post(home,xy,z,m)));
      break;
    case IRT_NQ:
      GECODE_ES_FAIL((Count::NqInt<OffsetView,ZeroIntView>
                           ::post(home,xy,z,m)));
      break;
    case IRT_LE:
      GECODE_ES_FAIL((Count::LqInt<OffsetView,ZeroIntView>
                           ::post(home,xy,z,m-1)));
      break;
    case IRT_LQ:
      GECODE_ES_FAIL((Count::LqInt<OffsetView,ZeroIntView>
                           ::post(home,xy,z,m)));
      break;
    case IRT_GR:
      GECODE_ES_FAIL((Count::GqInt<OffsetView,ZeroIntView>
                           ::post(home,xy,z,m+1)));
      break;
    case IRT_GQ:
      GECODE_ES_FAIL((Count::GqInt<OffsetView,ZeroIntView>
                           ::post(home,xy,z,m)));
      break;
    default:
      throw UnknownRelation("Int::count");
    }
  }

  void
  count(Home home, const IntVarArgs& x, int n,
        IntRelType r, IntVar z, IntConLevel) {
    using namespace Int;
    Limits::check(n,"Int::count");
    if (home.failed()) return;
    ViewArray<IntView> xv(home,x);
    ConstIntView yv(n);
    switch (r) {
    case IRT_EQ:
      GECODE_ES_FAIL((Count::EqView<IntView,ConstIntView,IntView,true>
                           ::post(home,xv,yv,z,0)));
      break;
    case IRT_NQ:
      GECODE_ES_FAIL((Count::NqView<IntView,ConstIntView,IntView,true>
                           ::post(home,xv,yv,z,0)));
      break;
    case IRT_LE:
      GECODE_ES_FAIL((Count::LqView<IntView,ConstIntView,IntView,true>
                           ::post(home,xv,yv,z,-1)));
      break;
    case IRT_LQ:
      GECODE_ES_FAIL((Count::LqView<IntView,ConstIntView,IntView,true>
                           ::post(home,xv,yv,z,0)));
      break;
    case IRT_GR:
      GECODE_ES_FAIL((Count::GqView<IntView,ConstIntView,IntView,true>
                           ::post(home,xv,yv,z,1)));
      break;
    case IRT_GQ:
      GECODE_ES_FAIL((Count::GqView<IntView,ConstIntView,IntView,true>
                           ::post(home,xv,yv,z,0)));
      break;
    default:
      throw UnknownRelation("Int::count");
    }
  }

  void
  count(Home home, const IntVarArgs& x, IntVar y,
        IntRelType r, IntVar z, IntConLevel) {
    using namespace Int;
    if (home.failed()) return;
    ViewArray<IntView> xv(home,x);
    switch (r) {
    case IRT_EQ:
      GECODE_ES_FAIL((Count::EqView<IntView,IntView,IntView,true>
                           ::post(home,xv,y,z,0)));
      break;
    case IRT_NQ:
      GECODE_ES_FAIL((Count::NqView<IntView,IntView,IntView,true>
                           ::post(home,xv,y,z,0)));
      break;
    case IRT_LE:
      GECODE_ES_FAIL((Count::LqView<IntView,IntView,IntView,true>
                           ::post(home,xv,y,z,-1)));
      break;
    case IRT_LQ:
      GECODE_ES_FAIL((Count::LqView<IntView,IntView,IntView,true>
                           ::post(home,xv,y,z,0)));
      break;
    case IRT_GR:
      GECODE_ES_FAIL((Count::GqView<IntView,IntView,IntView,true>
                           ::post(home,xv,y,z,1)));
      break;
    case IRT_GQ:
      GECODE_ES_FAIL((Count::GqView<IntView,IntView,IntView,true>
                           ::post(home,xv,y,z,0)));
      break;
    default:
      throw UnknownRelation("Int::count");
    }
  }

  void
  count(Home home, const IntVarArgs& x, const IntArgs& y,
        IntRelType r, IntVar z, IntConLevel) {
    using namespace Int;
    if (x.size() != y.size())
      throw ArgumentSizeMismatch("Int::count");
    if (home.failed()) return;

    ViewArray<OffsetView> xy(home,x.size());
    for (int i=x.size(); i--; )
      xy[i] = OffsetView(x[i],-y[i]);

    ZeroIntView u;
    switch (r) {
    case IRT_EQ:
      GECODE_ES_FAIL((Count::EqView<OffsetView,ZeroIntView,IntView,true>
                           ::post(home,xy,u,z,0)));
      break;
    case IRT_NQ:
      GECODE_ES_FAIL((Count::NqView<OffsetView,ZeroIntView,IntView,true>
                           ::post(home,xy,u,z,0)));
      break;
    case IRT_LE:
      GECODE_ES_FAIL((Count::LqView<OffsetView,ZeroIntView,IntView,true>
                           ::post(home,xy,u,z,-1)));
      break;
    case IRT_LQ:
      GECODE_ES_FAIL((Count::LqView<OffsetView,ZeroIntView,IntView,true>
                           ::post(home,xy,u,z,0)));
      break;
    case IRT_GR:
      GECODE_ES_FAIL((Count::GqView<OffsetView,ZeroIntView,IntView,true>
                           ::post(home,xy,u,z,1)));
      break;
    case IRT_GQ:
      GECODE_ES_FAIL((Count::GqView<OffsetView,ZeroIntView,IntView,true>
                           ::post(home,xy,u,z,0)));
      break;
    default:
      throw UnknownRelation("Int::count");
    }
  }

}

// STATISTICS: int-post
