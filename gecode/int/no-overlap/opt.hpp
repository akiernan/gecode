/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2011
 *
 *  Last modified:
 *     $Date: 2011-07-13 16:42:33 +0200 (Wed, 13 Jul 2011) $ by $Author: schulte $
 *     $Revision: 12191 $
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

namespace Gecode { namespace Int { namespace NoOverlap {

  template<class Dim, int d>
  forceinline
  OptProp<Dim,d>::OptProp(Home home, OptBox<Dim,d>* b, int n, int m0)
    : Base<OptBox<Dim,d> >(home,b,n), m(m0) {
    for (int i=m; i--; )
      b[n+i].subscribe(home, *this);
  }

  template<class Dim, int d>
  ExecStatus
  OptProp<Dim,d>::post(Home home, OptBox<Dim,d>* b, int n) {
    // Partition into mandatory and optional boxes
    if (n > 1) {
      int p = Base<OptBox<Dim,d> >::partition(b, 0, n);
      (void) new (home) OptProp<Dim,d>(home,b,p,n-p);
    }
    return ES_OK;
  }

  template<class Dim, int d>
  forceinline size_t 
  OptProp<Dim,d>::dispose(Space& home) {
    for (int i=m; i--; )
      b[n+i].cancel(home, *this);
    (void) Base<OptBox<Dim,d> >::dispose(home);
    return sizeof(*this);
  }


  template<class Dim, int d>
  forceinline
  OptProp<Dim,d>::OptProp(Space& home, bool shared, OptProp<Dim,d>& p) 
    : Base<OptBox<Dim,d> >(home, shared, p, p.n + p.m), m(p.m) {}

  template<class Dim, int d>
  Actor* 
  OptProp<Dim,d>::copy(Space& home, bool share) {
    return new (home) OptProp<Dim,d>(home,share,*this);
  }

  template<class Dim, int d>
  ExecStatus 
  OptProp<Dim,d>::propagate(Space& home, const ModEventDelta& med) {
    Region r(home);

    if (BoolView::me(med) == ME_BOOL_VAL) {
      // Eliminate excluded boxes
      for (int i=m; i--; )
        if (b[n+i].excluded()) {
          b[n+i].cancel(home,*this);
          b[n+i] = b[n+(--m)];
        }
      // Reconsider optional boxes
      if (m > 0) {
        int p = Base<OptBox<Dim,d> >::partition(b+n, 0, m);
        n += p; m -= p;
      }
    }

    // Number of disjoint boxes
    int* db = r.alloc<int>(n);
    for (int i=n; i--; )
      db[i] = n-1;

    // Number of boxes to be eliminated
    int e = 0;
    for (int i=n; i--; ) {
      assert(b[i].mandatory());
      for (int j=i; j--; ) 
        if (b[i].nooverlap(b[j])) {
          assert(db[i] > 0); assert(db[j] > 0);
          if (--db[i] == 0) e++;
          if (--db[j] == 0) e++;
          continue;
        } else {
          GECODE_ES_CHECK(b[i].nooverlap(home,b[j]));
        }
    }

    if (m == 0) {
      if (e == n)
        return home.ES_SUBSUMED(*this);
      int i = n-1;
      while (e > 0) {
        // Eliminate boxes that do not overlap
        while (db[i] > 0)
          i--;
        b[i].cancel(home, *this);
        b[i] = b[--n]; b[n] = b[n+m];
        e--; i--;
      }
      if (n < 2)
        return home.ES_SUBSUMED(*this);
    }

    // Check whether some optional boxes must be excluded
    for (int i=m; i--; ) {
      assert(b[n+i].optional());
      for (int j=n; j--; )
        if (b[n+i].overlap(b[j])) {
          GECODE_ES_CHECK(b[n+i].exclude(home));
          b[n+i].cancel(home,*this);
          b[n+i] = b[n+(--m)];
          break;
        }
    }

    return ES_NOFIX;
  }

}}}

// STATISTICS: int-prop

