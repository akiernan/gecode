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

#ifndef __GECODE_SET_SELECT_HH__
#define __GECODE_SET_SELECT_HH__

#include <gecode/set.hh>

#include <gecode/int/element.hh>
#include <gecode/set/rel.hh>
#include <gecode/set/rel-op.hh>

namespace Gecode { namespace Int { namespace Element {
  /// VarArg type for %Set views
  template<>
  class ViewToVarArg<Gecode::Set::SetView> {
  public:
    typedef Gecode::SetVarArgs argtype;
  };
}}}

namespace Gecode { namespace Set { namespace Element {

  /**
   * \namespace Gecode::Set::Element
   * \brief %Set element propagators
   */

  /**
   * \brief %Propagator for element with intersection
   *
   * Requires \code #include <gecode/set/element.hh> \endcode
   * \ingroup FuncSetProp
   */
  template<class SView, class RView>
  class ElementIntersection : public Propagator {
  public:
    typedef Gecode::Int::Element::IdxViewArray<SView> IdxViewArray;
  protected:
    IntSet universe;
    RView x0;
    IdxViewArray iv;
    RView x1;

    /// Constructor for cloning \a p
    ElementIntersection(Space& home, bool share,ElementIntersection& p);
    /// Constructor for posting
    ElementIntersection(Home home,RView,IdxViewArray&,RView,
                       const IntSet& universe);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home,bool);
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /** Post propagator for \f$ z=\bigcap\langle x_0,\dots,x_{n-1}\rangle[y] \f$ using \a u as universe
     *
     * If \a y is empty, \a z will be constrained to be the given universe
     * \a u (as an empty intersection is the universe).
     */
    static ExecStatus post(Home home,RView z,IdxViewArray& x,
                           RView y, const IntSet& u);
  };

  /**
   * \brief %Propagator for element with union
   *
   * Requires \code #include <gecode/set/element.hh> \endcode
   * \ingroup FuncSetProp
   */
  template<class SView, class RView>
  class ElementUnion : public Propagator {
  public:
    typedef Gecode::Int::Element::IdxViewArray<SView> IdxViewArray;
  protected:
    SView x0;
    IdxViewArray iv;
    RView x1;

    /// Constructor for cloning \a p
    ElementUnion(Space& home, bool share,ElementUnion& p);
    /// Constructor for posting
    ElementUnion(Home home,SView,IdxViewArray&,RView);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home,bool);
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus propagate(Space& home, const ModEventDelta& med);
    /** Post propagator for \f$ z=\bigcup\langle x_0,\dots,x_{n-1}\rangle[y] \f$
     *
     * If \a y is empty, \a z will be constrained to be empty
     * (as an empty union is the empty set).
     */
    static  ExecStatus post(Home home,SView z,IdxViewArray& x,
                            RView y);
  };

  /**
   * \brief %Propagator for element with union of constant sets
   *
   * Requires \code #include <gecode/set/element.hh> \endcode
   * \ingroup FuncSetProp
   */
  template<class SView, class RView>
  class ElementUnionConst : public Propagator {
  protected:
    SView x0;
    SharedArray<IntSet> iv;
    RView x1;

    /// Constructor for cloning \a p
    ElementUnionConst(Space& home, bool share,ElementUnionConst& p);
    /// Constructor for posting
    ElementUnionConst(Home home,SView,SharedArray<IntSet>&,RView);
  public:
    /// Copy propagator during cloning
    virtual Actor* copy(Space& home,bool);
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus  propagate(Space& home, const ModEventDelta& med);
    /** Post propagator for \f$ z=\bigcup\langle s_0,\dots,s_{n-1}\rangle[y] \f$
     *
     * If \a y is empty, \a z will be constrained to be empty
     * (as an empty union is the empty set).
     */
    static  ExecStatus  post(Home home,SView z,SharedArray<IntSet>& x,
                             RView y);
  };

  /**
   * \brief %Propagator for element with disjointness
   *
   * Requires \code #include <gecode/set/element.hh> \endcode
   * \ingroup FuncSetProp
   */
  template<class SView, class RView>
  class ElementDisjoint : public Propagator {
  public:
    typedef Gecode::Int::Element::IdxViewArray<SView> IdxViewArray;
  protected:
    IdxViewArray iv;
    RView x1;

    /// Constructor for cloning \a p
    ElementDisjoint(Space& home, bool share,ElementDisjoint& p);
    /// Constructor for posting
    ElementDisjoint(Home home,IdxViewArray&,RView);
  public:
    /// Copy propagator during cloning
    virtual Actor*      copy(Space& home,bool);
    virtual PropCost cost(const Space& home, const ModEventDelta& med) const;
    /// Delete propagator and return its size
    virtual size_t dispose(Space& home);
    /// Perform propagation
    virtual ExecStatus  propagate(Space& home, const ModEventDelta& med);
    /// Post propagator for \f$ \parallel\langle x_0,\dots,x_{n-1}\rangle[y] \f$
    static  ExecStatus  post(Home home,IdxViewArray& x,RView y);
  };

}}}

#include <gecode/set/element/inter.hpp>
#include <gecode/set/element/union.hpp>
#include <gecode/set/element/unionConst.hpp>
#include <gecode/set/element/disjoint.hpp>

#endif

// STATISTICS: set-prop

