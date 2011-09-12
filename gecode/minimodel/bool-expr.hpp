/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
 *
 *  Last modified:
 *     $Date: 2011-01-21 16:01:14 +0100 (Fri, 21 Jan 2011) $ by $Author: schulte $
 *     $Revision: 11553 $
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

namespace Gecode {

  /*
   * Operations for expressions
   *
   */
  forceinline
  BoolExpr::Node::Node(void) : use(1) {}

  forceinline
  BoolExpr::Node::~Node(void) { delete m; }

  forceinline void*
  BoolExpr::Node::operator new(size_t size) {
    return heap.ralloc(size);
  }
  forceinline void
  BoolExpr::Node::operator delete(void* p, size_t) {
    heap.rfree(p);
  }

  forceinline void*
  BoolExpr::MiscExpr::operator new(size_t size) {
    return heap.ralloc(size);
  }
  forceinline void
  BoolExpr::MiscExpr::operator delete(void* p, size_t) {
    heap.rfree(p);
  }

  forceinline
  BoolExpr::BoolExpr(void) : n(NULL) {}

  forceinline
  BoolExpr::BoolExpr(const BoolExpr& e) : n(e.n) {
    n->use++;
  }

  inline BoolVar
  BoolExpr::expr(Home home, IntConLevel icl) const {
    Region r(home);
    return NNF::nnf(r,n,false)->expr(home,icl);
  }

  inline void
  BoolExpr::rel(Home home, IntConLevel icl) const {
    Region r(home);
    return NNF::nnf(r,n,false)->rel(home,icl);
  }

}

// STATISTICS: minimodel-any
