// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework 
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2006 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Id: DPeakArray.h,v 1.18 2006/06/01 15:26:18 groepl Exp $
// $Author: groepl $
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_KERNEL_DPEAKARRAY_H
#define OPENMS_KERNEL_DPEAKARRAY_H

#include <OpenMS/config.h>
#include <OpenMS/KERNEL/DPeak.h>
#include <OpenMS/KERNEL/ComparatorUtils.h>

#include <queue>
#include <vector>
#include <algorithm>

namespace OpenMS
{

	/**	
		@brief Polymorphic peak container implemented as an array.  
		
		This class represents an array of D-dimensional
		peaks.  It fulfills the requirements of an STL vector class, but
		provides a more convenient interface to manipulate these vectors,
		sort with respect to specific dimensions and intensity, and a
		convenient interface to the other OpenMS classes.
		
		If you do only use one type of peaks it it more efficient to use
		DPeakArrayNonPolymorphic instead of this class!
		
		<i><b>ATTENTION:</b> DPeakArray can be used with DPeak and instances of
		classes derived from DPeak at a time, but do not use mutating STL
		algorithms on an inhomogenous DPeakArray! These algorithms may affect
		only the DPeak part of the contained Peaks. </i>
	
		@ingroup Kernel, Serialization
	*/
	template <Size D, typename PeakT = DPeak<D> >
	class DPeakArray
		: public PersistentObject
	{

		/// PointerPriorityQueue accesses vector_ through  internalPointerVector()
		template < typename Value, typename Comparator > friend class PointerPriorityQueue;

		public:
		
		///ConstIterator for the DPeakArray
		template <class IteratorPeakT>
		class DPeakArrayConstIterator
		{
			
			friend class DPeakArray;
			
			public:
			/**	@name Type definitions */
			//@{
			typedef IteratorPeakT IteratorPeakType;
			typedef IteratorPeakType value_type;
			typedef typename std::vector<IteratorPeakType*>::difference_type difference_type;
			typedef const value_type& reference;
			typedef const value_type* pointer;
			typedef std::random_access_iterator_tag iterator_category;
			//@}
			
			DPeakArrayConstIterator()
			{
			}
			
			DPeakArrayConstIterator(const typename std::vector<IteratorPeakType*>* vec , unsigned int position)
			{
				vector_ = (typename std::vector<IteratorPeakType*>*)vec;
				position_ = position;
			}

			DPeakArrayConstIterator(typename std::vector<IteratorPeakType*>* vec , unsigned int position)
			{
				vector_ = vec;
				position_ = position;
			}

			DPeakArrayConstIterator(const DPeakArrayConstIterator& it)
			{
				vector_=it.vector_;
				position_=it.position_;
			}

			~DPeakArrayConstIterator()
			{
			}

			DPeakArrayConstIterator& operator = (const DPeakArrayConstIterator& rhs)
			{
				if (this==&rhs) return *this;
				
				vector_=rhs.vector_;
				position_=rhs.position_;
				
				return *this;
			}
		
			bool operator < (const DPeakArrayConstIterator& it) const
			{
				return position_ < it.position_;
			}

			bool operator > (const DPeakArrayConstIterator& it) const
			{
				return position_ > it.position_;
			}

			bool operator <= (const DPeakArrayConstIterator& it) const
			{
				return (position_ < it.position_ || position_ == it.position_);
			}

			bool operator >= (const DPeakArrayConstIterator& it) const
			{
				return (position_ > it.position_ || position_ == it.position_);
			}
			
			bool operator == (const DPeakArrayConstIterator& it) const
			{
				return position_ == it.position_ && vector_ == it.vector_;
			}

			bool operator != (const DPeakArrayConstIterator& it) const
			{
				return position_ != it.position_ || vector_ != it.vector_;
			}
			
			DPeakArrayConstIterator& operator ++ ()
			{
				position_ += 1;
				return *this;
			} 

			DPeakArrayConstIterator operator ++ (int)
			{
	      DPeakArrayConstIterator tmp(*this);
	      ++(*this);
	      return tmp;
			} 

			DPeakArrayConstIterator& operator -- ()
			{
				position_-=1;
				return *this;
			} 

			DPeakArrayConstIterator operator -- (int)
			{
	      DPeakArrayConstIterator tmp(*this);
	      --(*this);
	      return tmp;
			} 

			DPeakArrayConstIterator operator - (difference_type n) const
			{
				DPeakArrayConstIterator tmp(*this);
				tmp.position_ -= n;
				return tmp;
			} 

			DPeakArrayConstIterator operator + (difference_type n) const
			{
				DPeakArrayConstIterator tmp(*this);
				tmp.position_ += n;
				return tmp;
			} 

			DPeakArrayConstIterator& operator += (difference_type n)
			{
				position_ += n;
				return *this;
			} 

			DPeakArrayConstIterator& operator -= (difference_type n)
			{
				position_ -= n;
				return *this;
			}

			friend difference_type operator - ( const DPeakArrayConstIterator& i1, const DPeakArrayConstIterator& i2 )
			{
				return (i1.position_ - i2.position_);
			} 

			friend DPeakArrayConstIterator operator + ( difference_type n, const DPeakArrayConstIterator& i )
			{
				DPeakArrayConstIterator tmp(i);
				tmp.position_ += n;
				return tmp;
			} 
			
			reference operator * ()
			{
				return *((*vector_)[position_]);
			}
			
			pointer operator -> ()
			{
				return (*vector_)[position_];
			}

			pointer operator -> () const
			{
				return (*vector_)[position_];
			}
			
			reference operator [] (difference_type n)
			{
				return *((*this)+n);
			}
			
			protected:
			
			typename std::vector<IteratorPeakType*>* vector_;
			unsigned int position_; 
		};
		
	
		/// Mutable iterator for the DPeakArray
		template <class IteratorPeakT>
		class DPeakArrayIterator : public DPeakArrayConstIterator<IteratorPeakT>
		{
			friend class DPeakArray;
			
			public:	
			typedef IteratorPeakT IteratorPeakType;
			typedef typename DPeakArrayConstIterator<IteratorPeakType>::value_type& reference;
			typedef typename DPeakArrayConstIterator<IteratorPeakType>::value_type* pointer;

			using DPeakArrayConstIterator<IteratorPeakType>::vector_;
			using DPeakArrayConstIterator<IteratorPeakType>::position_;

			
			DPeakArrayIterator()
			{
				
			}

			DPeakArrayIterator(typename std::vector<IteratorPeakType*>* vec, unsigned int position): DPeakArrayConstIterator<IteratorPeakType>(vec,position)
			{

			}

			DPeakArrayIterator(const DPeakArrayIterator<IteratorPeakType>& it): DPeakArrayConstIterator<IteratorPeakType>(it)
			{

			}

			~DPeakArrayIterator()
			{
				
			}
			
			reference operator * ()
			{
				return *((*vector_)[position_]);
			}

			pointer operator -> ()
			{
				return (*vector_)[position_];
			}

			const pointer operator -> () const
			{
				return (*vector_)[position_];
			}

			typename DPeakArrayIterator::reference operator [] (typename DPeakArrayIterator::difference_type n)
			{
				return *((*this)+n);
			}

			DPeakArrayIterator& operator ++ ()
			{
				DPeakArrayConstIterator<IteratorPeakType>::position_+=1;
				return *this;
			} 

			DPeakArrayIterator operator ++ (int)
			{
	      DPeakArrayIterator tmp(*this);
	      ++(*this);
	      return tmp;
			} 

			DPeakArrayIterator& operator -- ()
			{
				DPeakArrayConstIterator<IteratorPeakType>::position_-=1;
				return *this;
			} 

			DPeakArrayIterator operator -- (int)
			{
	      DPeakArrayIterator tmp(*this);
	      --(*this);
	      return tmp;
			} 

			DPeakArrayIterator operator - (typename DPeakArrayIterator::difference_type n) const
			{
				DPeakArrayIterator tmp(*this);
				tmp.position_ -= n;
				return tmp;
			} 

			DPeakArrayIterator operator + (typename DPeakArrayIterator::difference_type n) const
			{
				DPeakArrayIterator tmp(*this);
				tmp.position_ += n;
				return tmp;
			} 

			friend DPeakArrayIterator operator + (typename DPeakArrayIterator::difference_type n, const DPeakArrayIterator& i )
			{
				DPeakArrayIterator tmp(i);
				tmp.position_ += n;
				return tmp;
			} 

			DPeakArrayIterator& operator += (typename DPeakArrayIterator::difference_type n)
			{
				DPeakArrayConstIterator<IteratorPeakType>::position_ += n;
				return *this;
			} 

			DPeakArrayIterator& operator -= (typename DPeakArrayIterator::difference_type n)
			{
				DPeakArrayConstIterator<IteratorPeakType>::position_ -= n;
				return *this;
			}
			
			friend void swap(DPeakArrayIterator& i1, DPeakArrayIterator& i2)
			{
				unsigned int tmp = i1.position_;
				i1.position_ = i2.position_;
				i2.position_ = tmp;
			}
			
			protected:
			
		};	
		/// Peak type
		typedef PeakT PeakType;
		/// Mutable iterator
		typedef DPeakArrayIterator<PeakType> Iterator;
		/// Non-mutable iterator
		typedef DPeakArrayConstIterator<PeakType> ConstIterator;
		/// Mutable reverse iterator
		typedef std::reverse_iterator<Iterator> ReverseIterator;
		/// Non-mutable reverse iterator
		typedef std::reverse_iterator<ConstIterator> ConstReverseIterator;

		/** @name STL-compliance type definitions
		*/
		//@{		
		typedef typename std::vector<PeakType>::value_type value_type;
		typedef typename std::vector<PeakType>::size_type size_type;
		typedef typename std::vector<PeakType>::difference_type difference_type;
		typedef typename std::vector<PeakType>::reference reference;
		typedef typename std::vector<PeakType>::const_reference const_reference;
		typedef typename std::vector<PeakType>::pointer pointer;
		typedef Iterator iterator;
		typedef ConstIterator const_iterator;
		typedef ReverseIterator reverse_iterator;
		typedef ConstReverseIterator const_reverse_iterator;
		//@}
		
		/// See std::vector documentation.
		void push_back(const PeakType& x)
		{
			PeakType* peak = static_cast<PeakType*>(x.clone());
			vector_.push_back(peak);
		}

		/// See std::vector documentation.
		void pop_back()
		{
			delete vector_[vector_.size()-1];
			vector_.pop_back();
		}
		
		/// See std::vector documentation.
		size_type size() const
		{
			return vector_.size();
		}

		/// See std::vector documentation.
		size_type capacity() const
		{
			return std::max(vector_.size(),capacity_);
		}
		
		/// See std::vector documentation.
		void reserve(size_type n)
		{
			size_type cap = capacity();
			if (n>cap)
			{
			vector_.reserve(n);
			for (size_type i=cap ; i<n;i++)
				{
					PeakType* peak = new PeakType();
					vector_[i]=peak;
				}
			capacity_ = n;
			}
		}

		/// See std::vector documentation.
		size_type max_size() const
		{
			return vector_.max_size();
		}
		
		/// See std::vector documentation.
		Iterator begin()
		{
			return Iterator((std::vector<PeakType*>*)&vector_,(unsigned int)0);
		}
		
		/// See std::vector documentation.
		Iterator end()
		{
			return Iterator((std::vector<PeakType*>*)&vector_,(unsigned int)(vector_.size()));
		}
		
		/// See std::vector documentation.
		ConstIterator begin() const
		{
			return ConstIterator((const std::vector<PeakType*>*)&vector_,(unsigned int)0);
		}
		
		/// See std::vector documentation.
		ConstIterator end() const
		{
			return ConstIterator((const std::vector<PeakType*>*)&vector_,(unsigned int)(vector_.size()));
		}
		
		/// See std::vector documentation.
		ReverseIterator rbegin()
		{
			return ReverseIterator(end());
		}
		
		/// See std::vector documentation.
		ReverseIterator rend()
		{
			return ReverseIterator(begin());
		}
		
		/// See std::vector documentation.
		ConstReverseIterator rbegin() const
		{
			return ConstReverseIterator(end());
		}
		
		/// See std::vector documentation.
		ConstReverseIterator rend() const
		{
			return ConstReverseIterator(begin());
		}
		
		/// See std::vector documentation.
		void resize(size_type new_size, const PeakType& t=PeakType())
		{
			size_type old_size = vector_.size();
			if (new_size<old_size)
			{
				for (size_type i=new_size ; i<old_size;i++)
				{
					delete(vector_[i]);
				}		
				vector_.resize(new_size);		
			}
			else if (new_size>old_size)
			{
				vector_.resize(new_size);
				for (size_type i=old_size ; i<new_size;i++)
				{
					PeakType* peak = static_cast<PeakType*>(t.clone());
					vector_[i]=peak;
				}
			}
		}
		
		/// See std::vector documentation.
		reference front()
		{
			return *(begin());
		}
		
		/// See std::vector documentation.
		const_reference front() const
		{
			return *(begin());
		}

		/// See std::vector documentation.
		reference back()
		{
			return *(end()-1);
		}
		
		/// See std::vector documentation.
		const_reference back() const
		{
			return *(end()-1);
		}
		
		/// See std::vector documentation.
		void clear()
		{
			for (unsigned int i=0;i<vector_.size();i++)
			{
				delete(vector_[i]);
			}
			vector_.clear();	
		}
		
		/// See std::vector documentation.
		bool empty() const
		{
			return vector_.empty();
		}

		/// See std::vector documentation.
		reference operator [](size_type n)
		{
			return *(vector_[n]);
		}
		
		/// See std::vector documentation.
		const_reference operator [](size_type n) const
		{
			return *(vector_[n]);
		}
		
		/// See std::vector documentation.
		bool operator == (const DPeakArray& array) const
		{
			if (size()!=array.size())
			{
				return false;
			}
			for (unsigned int i=0;i<size();i++)
			{
				if (typeid(*(vector_[i]))!=typeid(*(array.vector_[i])))
				{
					return false;
				}
				if ( vector_[i]->operator!=(*array.vector_[i]) )
				{
					return false;
				}
			}
			return true;
		}
		
		/// See std::vector documentation.
		bool operator !=(const DPeakArray& array) const
		{
			return !(operator==(array));
		}
		
		/// Comparison of container sizes
		bool operator < (const DPeakArray& array) const
		{
			return size() < array.size();
		}
		
		/// Comparison of container sizes
		bool operator > (const DPeakArray& array) const
		{
			return size() > array.size();
		}
		
		/// Comparison of container sizes
		bool operator <= (const DPeakArray& array) const
		{
			return operator<(array) || operator==(array);
		}
		
		/// Comparison of container sizes
		bool operator >= (const DPeakArray& array) const
		{
			return operator>(array) || operator==(array);
		}

		/// See std::vector documentation.
		void swap(DPeakArray& array)
		{
			vector_.swap(array.vector_);
		}
		
		/// See std::vector documentation.
		friend void swap(DPeakArray& a1, DPeakArray& a2)
		{
			a1.vector_.swap(a2.vector_);
		}
		
		/// See std::vector documentation.
		Iterator insert(Iterator pos, const PeakType& peak)
		{
			PeakType* pointer = static_cast<PeakType*>(peak.clone());
			vector_.insert(vector_.begin()+pos.position_,pointer);
			return pos;
		}
		
		/// See std::vector documentation.
		void insert(Iterator pos, size_type n, const PeakType& peak)
		{
			PeakType* pointer;
			std::vector<PeakType*> tmp;
			for (size_type i=0;i<n;i++)
			{
				pointer = static_cast<PeakType*>(peak.clone());
				tmp.push_back(pointer);
			}
			vector_.insert(vector_.begin()+pos.position_,tmp.begin(),tmp.end());
		}

		/// See std::vector documentation.
		template <class InputIterator>
		void insert(Iterator pos, InputIterator f, InputIterator l)
		{
			PeakType* pointer;
			std::vector<PeakType*> tmp;
			for (InputIterator it=f;it!=l;++it)
			{
				pointer = static_cast<PeakType*>(it->clone());
				tmp.push_back(pointer);
			}
			vector_.insert(vector_.begin()+pos.position_,tmp.begin(),tmp.end());
		}
		
		/// See std::vector documentation.
		Iterator erase(Iterator pos)
		{
			delete(&(*pos));
			vector_.erase(vector_.begin()+pos.position_);
			return pos;
		}
		
		/// See std::vector documentation.
		Iterator erase(Iterator first,Iterator last)
		{
			for (Iterator it=first;it!=last;++it)
			{
				delete(&(*it));
			}
			vector_.erase(vector_.begin()+first.position_,vector_.begin()+last.position_);
			return first;
		}
	
		/**	@name Constructors and Destructor
		*/
		//@{
		/// See std::vector documentation.
		DPeakArray() : PersistentObject(), capacity_(0) {}
		
		/// See std::vector documentation.
		DPeakArray(size_type n) : PersistentObject(), capacity_(0) 
		{
			vector_=std::vector<PeakType*>(n);
			for (size_type i=0;i<n;i++)
			{
				PeakType* peak = new PeakType();
				vector_[i]=peak;
			}
		} 
		/// See std::vector documentation.
		DPeakArray(size_type n, const PeakType& peak) : PersistentObject(), capacity_(0) 
		{
			vector_=std::vector<PeakType*>(n);
			for (size_type i=0;i<n;i++)
			{
				PeakType* peak_p = static_cast<PeakType*>(peak.clone());
				vector_[i]=peak_p;
			}
		} 
		/// See std::vector documentation.
		DPeakArray(const DPeakArray& p) : PersistentObject(p), capacity_(0) 
		{
			PeakType* peak;
			for (ConstIterator it=p.begin(); it!=p.end();++it)
			{
				peak = static_cast<PeakType*>(it->clone());
				vector_.push_back(peak);
			}
		}
		/// See std::vector documentation.
		template <class InputIterator>
		DPeakArray(InputIterator f, InputIterator l) : PersistentObject(), capacity_(0) 
		{
			PeakType* pointer;
			for (InputIterator it=f;it!=l;++it)
			{
				pointer = static_cast<PeakType*>(it->clone());
				vector_.push_back(pointer);
			}
		}
		
		/// See std::vector documentation.
		~DPeakArray() 
		{
			for (unsigned int i=0;i<vector_.size();i++)
			{
				delete(vector_[i]);
			}
		}
		//@}
		
		/// See std::vector documentation.
		DPeakArray& operator = (const DPeakArray& rhs) 
		{
			if (this==&rhs) return *this;
			
			clear();
			reserve(rhs.size());
			PeakType* peak;
			for (ConstIterator it=rhs.begin(); it!=rhs.end();++it)
			{
				peak = static_cast<PeakType*>(it->clone());
				vector_.push_back(peak);
			}
			
			return *this;
		}
		
		/// See std::vector documentation.
		template <class InputIterator>
		void assign(InputIterator f , InputIterator l)
		{
			clear();
			insert(end(),f,l);
		}
		
		/// See std::vector documentation.
		void assign(size_type n , const PeakType& x)
		{
			clear();
			insert(end(),n,x);
		}

		/**	
			@name Sorting.
			
			These simplified sorting methods are supported in addition to	the standard sorting methods of std::vector.
		*/
		//@{
		
		/// Sorts the peaks by intensity
		void sortByIntensity() 
		{ 
			std::sort(vector_.begin(), vector_.end(), PointerComparator < typename PeakType::IntensityLess > () ); 
		}

		/// Lexicographically sorts the peaks by their position.
		void sortByPosition() 
		{ 
			std::sort(vector_.begin(), vector_.end(), PointerComparator<typename PeakType::PositionLess>() );
		}

		/**
			@brief Sorts the peaks by one dimension of their position.
			
			It is only sorted according to dimentsion @p i . 
		*/
		void sortByNthPosition(UnsignedInt i) throw (Exception::NotImplemented);

		//@}

		/** 
			@name Generic sorting function templates.  
			Any peak comparator can be
			given as template argument.  You can also give the comparator as an
			argument to the function template (this is useful if the comparator is
			not default constructed, but keep in mind that STL copies comparators
			a lot).
			
			<p> Thus your can e.g. write <code>peaks.sortByComparator <
			DPeak<1>::IntensityLess > ()</code>, if peaks have type
			<code>DPeakArray < 1, DPeak <1> ></code>.
		*/
		//@{
		template < typename ComparatorType >
		void sortByComparator ( ComparatorType const & comparator )
		{ 
			std::sort(vector_.begin(), vector_.end(), PointerComparator < ComparatorType > ( comparator ) ); 
		}
		template < typename ComparatorType >
		void sortByComparator ()
		{ 
			std::sort(vector_.begin(), vector_.end(), PointerComparator< ComparatorType >() ); 
		}
		//@}

		// Docu in base class
		virtual void persistentWrite(PersistenceManager& pm, const char* name=0) const throw (Exception::Base)
		{
			pm.writeObjectHeader(this,name);
			//TODO Persistence
			pm.writeObjectTrailer(name);
		}
		
		// Docu in base class
		virtual void persistentRead(PersistenceManager& pm) throw (Exception::Base)
		{
			//TODO Persistence
			int dummy;
			pm.readPrimitive(dummy,"dummy_");
		}
		
		//----------------------------------------------------------------------
		
	protected:

		// Docu in base class
    virtual void clearChildIds_()
    {
    	//TODO Persistence	
    };		

		typedef std::vector<PeakType*> InternalPointerVector;
		
		/**
			@brief Do not use this.		
			
			...unless you know what you are doing!  It is used by PointerPriorityQueue
			which has a special constructor for vector arguments, which is faster than
			using begin() and end() because it does not rely on push_back().
		*/
		InternalPointerVector const & internalPointerVector() const
		{
			return vector_;
		}
		
		///the internal vector of PeakType pointers
		std::vector<PeakType*> vector_;
		///the current capacity
		size_type capacity_;
		
		///@name Serialization
		//@{
	 private:
		/// Serialization interface
		template<class Archive>
		void serialize(Archive & ar, const unsigned int /* version */ )
		{
			ar & boost::serialization::make_nvp("vector",vector_);
			ar & boost::serialization::make_nvp("capacity",capacity_);
		}
		//@}

		/// Serialization
		friend class boost::serialization::access;

	};

	///Print the contents to a stream.
	template <Size D, typename Peak>
	std::ostream& operator << (std::ostream& os, const DPeakArray<D, Peak>& array)
	{
		os << "-- DPEAKARRAY BEGIN --"<<std::endl;
		for (typename DPeakArray<D, Peak>::const_iterator it = array.begin(); it!=array.end(); ++it)
		{
			os << *it << std::endl;
		}
		os << "-- DPEAKARRAY END --"<<std::endl;
		return os;
	}

//---------------------------------------------------------------
//  Implementation of the inline / template functions
//---------------------------------------------------------------

//	// Partial specialization of DPeakArrayNonPolymorphic for use w/ DRawDataPoint.
//	// Specialization avoids issues with the missing PersistentObject of
//	// DRawDataPoint.
//	template <>
//	void DPeakArray<1, DRawDataPoint<1> >::serialize(PersistenceManager& /* f */);
//	template <>
//	void DPeakArray<2, DRawDataPoint<2> >::serialize(PersistenceManager& /* f */);
//	template <>
//	void DPeakArray<3, DRawDataPoint<3> >::serialize(PersistenceManager& /* f */);

	template <Size D, typename PeakT > 
	void DPeakArray<D,PeakT>::sortByNthPosition(UnsignedInt i) throw (Exception::NotImplemented)
	{ 
		OPENMS_PRECONDITION(i < Index(D), "illegal dimension")
		if (i==0)
		{
			std::sort(vector_.begin(), vector_.end(), PointerComparator< typename PeakType::template NthPositionLess<0> >() );
		}
		else if (i==1)
		{
			std::sort(vector_.begin(), vector_.end(), PointerComparator< typename PeakType::template NthPositionLess<1> >() );
		}
		else if (i==2)
		{
			std::sort(vector_.begin(), vector_.end(), PointerComparator< typename PeakType::template NthPositionLess<2> >() );
		}
		else
		{
			throw Exception::NotImplemented(__FILE__,__LINE__,__FUNCTION__);
		}
	}


} // namespace OpenMS

#endif // OPENMS_KERNEL_DPEAKARRAY_H
