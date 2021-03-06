/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2007
 *
 *  Last modified:
 *     $Date: 2010-07-28 16:13:53 +0200 (Wed, 28 Jul 2010) $ by $Author: schulte $
 *     $Revision: 11292 $
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

namespace Gecode { namespace Iter { namespace Values {

  /**
   * \brief Value iterator for pointwise minus of a value iterator
   *
   * This iterator in effect changes the order of how values
   * are iterated: the first values of the input iterator defines
   * the last value of the Minus iterator. Upon initialization
   * all values of the input iterator are stored in an array
   * which later allows iteration in inverse direction.
   *
   * \ingroup FuncIterValues
   */
  template<class I>
  class Minus : public ValueListIter {
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    Minus(void);
    /// Copy constructor
    Minus(const Minus& m);
    /// Initialize with values from \a i
    Minus(Region& r, I& i);
    /// Initialize with values from \a i
    void init(Region& r, I& i);
    /// Assignment operator
    Minus& operator =(const Minus& m);
    //@}
  };


  template<class I>
  forceinline
  Minus<I>::Minus(void) {}

  template<class I>
  forceinline
  Minus<I>::Minus(const Minus& m) 
    : ValueListIter(m) {}

  template<class I>
  void
  Minus<I>::init(Region& r, I& i) {
    ValueListIter::init(r);
    ValueList* p = NULL;
    for (; i(); ++i) {
      ValueList* t = new (*vlio) ValueList;
      t->next = p;
      t->val = -i.val();
      p = t;
    }
    ValueListIter::set(p);
  }

  template<class I>
  forceinline
  Minus<I>::Minus(Region& r, I& i) {
    init(r,i);
  }

  template<class I>
  forceinline Minus<I>&
  Minus<I>::operator =(const Minus<I>& m) {
    return static_cast<Minus&>(ValueListIter::operator =(m));
  }

}}}

// STATISTICS: iter-any
