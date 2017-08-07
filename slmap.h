#include "slstruct.h"
#include <functional>
#pragma once
template<class Key, class T, class Comp = std::less<Key>, class Alloc = std::allocator<std::pair<const Key, T> > > 
class slmap
{
	template<typename _Pair>
    struct select1st
    : public std::unary_function<_Pair, typename _Pair::first_type>
    {
      typename _Pair::first_type&
      operator()(_Pair& __x) const
      { return __x.first; }

	  const typename _Pair::first_type&
      operator()(const _Pair& __x) const
      { return __x.first; }
	};
public:
	typedef Key																			key_type;
	typedef T																			mapped_type;
	typedef std::pair<const Key, T>														value_type;
	typedef Comp																		key_compare;
	typedef Alloc																		allocator_type;
	typedef typename Alloc::reference													reference;
	typedef typename Alloc::const_reference												const_reference;
	typedef typename Alloc::pointer														pointer;
	typedef typename Alloc::const_pointer												const_pointer;
	typedef skip_list<key_type, value_type, select1st<value_type>, key_compare, Alloc>  rep_type;
	typedef typename rep_type::iterator													iterator;
	typedef typename rep_type::const_iterator											const_iterator;
	typedef typename rep_type::size_type												size_type;
	typedef typename rep_type::difference_type											difference_type;
	typedef typename rep_type::reverse_iterator											reverse_iterator;
	typedef typename rep_type::const_reverse_iterator									const_reverse_iterator;

private:
	rep_type mRep;

public:
	class value_compare: public std::binary_function<value_type, value_type, bool>
	{
		friend class slmap<Key, T, Comp, Alloc>;
	protected:
		Comp cmp;
		value_compare(Comp c):cmp(c){}
	public:
		bool operator()(const value_type& lhs, const value_type& rhs)
		{
			return cmp(lhs.first, rhs.first);
		}
	};

	//construct
	slmap():
		mRep()
	{}

	explicit slmap(const Comp& c , const Alloc& a = Alloc() ):
	mRep(c, a)
	{}

	slmap(const slmap& rhs):
	mRep(rhs.mRep)
	{
	}

	template<class In>
	explicit slmap(In first, In last, const Comp& c = Comp(), const Alloc& a = Alloc()):
		mRep(c,a)
	{
		mRep.insert_unique(first, last);
	}

	slmap& operator=(const slmap& rhs)
	{
		this->mRep = rhs.mRep;
		return *this;
	}

	allocator_type get_allocator() const 
    { 
		return mRep.get_allocator(); 
	}

	//iter

    iterator begin() 
    { 
		return mRep.begin(); 
	}

    const_iterator begin() const
    { 
		return mRep.begin(); 
	}

    iterator end()
    { 
		return mRep.end(); 
	}

    const_iterator end() const 
    {
		return mRep.end(); 
	}

    reverse_iterator rbegin()
    { 
		return mRep.rbegin(); 
	}

    const_reverse_iterator rbegin() const 
    { 
		return mRep.rbegin(); 
	}

    reverse_iterator rend()
    { 
		return mRep.rend(); 
	}

    const_reverse_iterator rend() const 
	{
		return mRep.rend();
	}

	// capacity
    bool empty() const 
    { 
		return mRep.empty(); 
	}

    size_type size() const
    { 
		return mRep.size(); 
	}

    size_type max_size() const
    { 
		return mRep.max_size(); 
	}

    mapped_type& operator[](const key_type& key)
      {
		iterator itr = mRep.lower_bound(key);
		if (itr == end() || key_compare()(key, (*itr).first) )
			itr = mRep.insert_unique(itr, value_type(key, mapped_type()) );
		return (*itr).second;
      }

	mapped_type& at(const key_type& key)
    {
		iterator itr = lower_bound(key);
		if (itr == end() || key_comp()(key, (*itr).first))
			throw std::out_of_range("aaa");
		return (*itr).second;
      }

    const mapped_type& at(const key_type& key) const
      {
		const_iterator itr = lower_bound(key);
		if (itr == end() || key_comp()(key, (*itr).first))
			throw std::out_of_range("aaa");
		return (*itr).second;
      }

	//insert
	std::pair<iterator, bool> insert(const value_type& v)
    { 
		  return mRep.insert_unique(v); 
	}

	iterator insert(iterator pos, const value_type& v)
    { 
		return mRep.insert_unique(pos, v); 
	}

	template<class In>
	void insert(In first, In last)
	{
		mRep.insert_unique(first, last);
	}
	
	//erase

	iterator erase(iterator pos)
	{
		return mRep.erase(pos);
	}

	size_type erase(const key_type& key)
	{
		return mRep.erase(key);
	}

	iterator erase(iterator first, iterator last)
	{
		return mRep.erase(first, last);
	}

	void clear()
	{
		mRep.erase(begin(), end() );
	}

	void swap(slmap& rhs)
	{
		mRep.swap(rhs.mRep);
	}

	key_compare key_comp() const
	{
		return mRep.key_comp();
	}

	value_compare value_compare() const
	{
		return value_compare(mRep.key_comp() );
	}

	//find

	iterator find(const key_type& key)
	{
		return mRep.find(key);
	}

	const_iterator find(const key_type& key) const
	{
		return mRep.find(key);
	}

	size_type count(const key_type& key) const
	{
		return mRep.count(key);
	}

	iterator lower_bound(const key_type& key)
	{
		return mRep.lower_bound(key);
	}

	const_iterator lower_bound(const key_type& key) const
	{
		return mRep.lower_bound(key);
	}

	iterator upper_bound(const key_type& key)
	{
		return mRep.upper_bound(key);
	}

	const_iterator upper_bound(const key_type& key) const
	{
		return mRep.upper_bound(key);
	}

	std::pair<iterator, iterator> equal_range(const key_type& key)
	{
		return mRep.equal_range(key);
	}

	std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
	{
		return mRep.equal_range(key);
	}

	template<class K, class T, class C, class A>
	friend bool operator==(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs);

	template<class K, class T, class C, class A>
	friend bool operator<(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs);
};

template<class K, class T, class C, class A>
bool operator==(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs)
{
	return lhs.mRep == rhs.mRep;
}

template<class K, class T, class C, class A>
bool operator<(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs)
{
	return lhs.mRep < rhs.mRep;
}

template<class K, class T, class C, class A>
bool operator!=(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs)
{
	return !(lhs == rhs);
}

template<class K, class T, class C, class A>
bool operator>(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs)
{
	return !(rhs < lhs);
}

template<class K, class T, class C, class A>
bool operator<=(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs)
{
	return !(rhs < lhs);
}

template<class K, class T, class C, class A>
bool operator>=(const slmap<K, T, C, A>& lhs, const slmap<K, T, C, A>& rhs)
{
	return !(lhs < rhs);
}

template<class Key, class Tp, class Compare, class Alloc>
inline void swap(slmap<Key, Tp, Compare, Alloc>& lhs,
         slmap<Key, Tp, Compare, Alloc>& rhs)
{ 
	lhs.swap(rhs); 
}

//////////MULTIMAP/////////////////
template<class Key, class T, class Comp = std::less<Key>, class Alloc = std::allocator<std::pair<const Key, T> > >
class slmultimap
{
	template<typename Pair>
    struct select1st
    : public std::unary_function<Pair, typename Pair::first_type>
    {
      typename Pair::first_type&
      operator()(Pair& x) const
      { return x.first; }

	  const typename Pair::first_type&
      operator()(const Pair& x) const
      { return x.first; }
	};
public:
	typedef Key																			key_type;
	typedef T																			mapped_type;
	typedef std::pair<const Key, T>														value_type;
	typedef Comp																		key_compare;
	typedef Alloc																		allocator_type;
	typedef typename Alloc::reference													reference;
	typedef typename Alloc::const_reference												const_reference;
	typedef typename Alloc::pointer														pointer;
	typedef typename Alloc::const_pointer												const_pointer;
	typedef skip_list<key_type, value_type, select1st<value_type>, key_compare, Alloc>  rep_type;
	typedef typename rep_type::iterator													iterator;
	typedef typename rep_type::const_iterator											const_iterator;
	typedef typename rep_type::size_type												size_type;
	typedef typename rep_type::difference_type											difference_type;
	typedef typename rep_type::reverse_iterator											reverse_iterator;
	typedef typename rep_type::const_reverse_iterator									const_reverse_iterator;

private:
	rep_type mRep;

public:
	class value_compare: public std::binary_function<value_type, value_type, bool>
	{
		friend class slmap<Key, T, Comp, Alloc>;
	protected:
		Comp cmp;
		value_compare(Comp c):cmp(c){}
	public:
		bool operator()(const value_type& lhs, const value_type& rhs)
		{
			return cmp(lhs.first, rhs.first);
		}
	};

	//construct
	slmultimap():
		mRep()
	{}

	explicit slmultimap(const Comp& c , const Alloc& a = Alloc() ):
	mRep(c, a)
	{}

	slmultimap(const slmultimap& rhs):
	mRep(rhs.mRep)
	{
	}

	template<class In>
	explicit slmultimap(In first, In last, const Comp& c = Comp(), const Alloc& a = Alloc()):
		mRep(c,a)
	{
		mRep.insert_equal(first, last);
	}

	slmultimap& operator=(const slmultimap& rhs)
	{
		this->mRep = rhs.mRep;
		return *this;
	}

	allocator_type get_allocator() const 
    { 
		return mRep.get_allocator(); 
	}

	//iter

    iterator begin() 
    { 
		return mRep.begin(); 
	}

    const_iterator begin() const
    { 
		return mRep.begin(); 
	}

    iterator end()
    { 
		return mRep.end(); 
	}

    const_iterator end() const 
    {
		return mRep.end(); 
	}

    reverse_iterator rbegin()
    { 
		return mRep.rbegin(); 
	}

    const_reverse_iterator rbegin() const 
    { 
		return mRep.rbegin(); 
	}

    reverse_iterator rend()
    { 
		return mRep.rend(); 
	}

    const_reverse_iterator rend() const 
	{
		return mRep.rend();
	}

	// capacity
    bool empty() const 
    { 
		return mRep.empty(); 
	}

    size_type size() const
    { 
		return mRep.size(); 
	}

    size_type max_size() const
    { 
		return mRep.max_size(); 
	}

   	mapped_type& at(const key_type& key)
    {
		iterator itr = lower_bound(key);
		if (itr == end() || key_comp()(key, (*itr).first))
			throw std::out_of_range("aaa");
		return (*itr).second;
      }

    const mapped_type& at(const key_type& key) const
      {
		const_iterator itr = lower_bound(key);
		if (itr == end() || key_comp()(key, (*itr).first))
			throw std::out_of_range("aaa");
		return (*itr).second;
      }

	//insert
	iterator insert(const value_type& v)
    { 
		  return mRep.insert_equal(v); 
	}

	iterator insert(iterator pos, const value_type& v)
    { 
		return mRep.insert_equal(pos, v); 
	}

	template<class In>
	void insert(In first, In last)
	{
		mRep.insert_equal(first, last);
	}
	
	//erase

	iterator erase(iterator pos)
	{
		return mRep.erase(pos);
	}

	size_type erase(const key_type& key)
	{
		return mRep.erase(key);
	}

	iterator erase(iterator first, iterator last)
	{
		return mRep.erase(first, last);
	}

	void clear()
	{
		mRep.erase(begin(), end() );
	}

	void swap(slmultimap& rhs)
	{
		mRep.swap(rhs.mRep);
	}

	key_compare key_comp() const
	{
		return mRep.key_comp();
	}

	value_compare value_compare() const
	{
		return value_compare(mRep.key_comp() );
	}

	//find

	iterator find(const key_type& key)
	{
		return mRep.find(key);
	}

	const_iterator find(const key_type& key) const
	{
		return mRep.find(key);
	}

	size_type count(const key_type& key) const
	{
		return mRep.count(key);
	}

	iterator lower_bound(const key_type& key)
	{
		return mRep.lower_bound(key);
	}

	const_iterator lower_bound(const key_type& key) const
	{
		return mRep.lower_bound(key);
	}

	iterator upper_bound(const key_type& key)
	{
		return mRep.upper_bound(key);
	}

	const_iterator upper_bound(const key_type& key) const
	{
		return mRep.upper_bound(key);
	}

	std::pair<iterator, iterator> equal_range(const key_type& key)
	{
		return mRep.equal_range(key);
	}

	std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
	{
		return mRep.equal_range(key);
	}

	template<class K, class T, class C, class A>
	friend bool operator==(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs);

	template<class K, class T, class C, class A>
	friend bool operator<(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs);

	//
	void show()
	{
		mRep.show();
	}
	//
};

template<class K, class T, class C, class A>
bool operator==(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs)
{
	return lhs.mRep == rhs.mRep;
}

template<class K, class T, class C, class A>
bool operator<(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs)
{
	return lhs.mRep < rhs.mRep;
}

template<class K, class T, class C, class A>
bool operator!=(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs)
{
	return !(lhs == rhs);
}

template<class K, class T, class C, class A>
bool operator>(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs)
{
	return !(rhs < lhs);
}

template<class K, class T, class C, class A>
bool operator<=(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs)
{
	return !(rhs < lhs);
}

template<class K, class T, class C, class A>
bool operator>=(const slmultimap<K, T, C, A>& lhs, const slmultimap<K, T, C, A>& rhs)
{
	return !(lhs < rhs);
}

template<class Key, class Tp, class Compare, class Alloc>
inline void swap(slmultimap<Key, Tp, Compare, Alloc>& lhs,
         slmultimap<Key, Tp, Compare, Alloc>& rhs)
{ 
	lhs.swap(rhs); 
}