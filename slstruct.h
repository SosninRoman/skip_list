#include <iostream>
#include <utility>
#include <random>
#include <ctime>
#include <sstream>
#include <iterator>
#pragma once
typedef std::pair<int, float> item;

struct skip_list_node_base
{
	typedef skip_list_node_base*			Base_ptr;
	typedef const skip_list_node_base*		Const_Base_ptr;

	Base_ptr*								mNext;
	Base_ptr								mPrev;
	int										h;
};

template<class T>
struct skip_list_node: public skip_list_node_base
{
	typedef skip_list_node<T>* Link_type;

	T value_field;

	template<class U>
	explicit skip_list_node(const U& v):
		value_field(v){}

	explicit skip_list_node(const T& v):
		value_field(v){}
};

template<class T>
struct skip_list_iterator
{
	typedef std::bidirectional_iterator_tag			iterator_category;
	typedef T										value_type;
	typedef T&										reference;
	typedef T*										pointer;
	typedef ptrdiff_t								difference_type;

	typedef skip_list_iterator<T>					Self_type;
	typedef skip_list_node_base::Base_ptr			Base_ptr;
	typedef skip_list_node<T>*						Link_type;

	Base_ptr mNode;

	skip_list_iterator():
		mNode()
	{}

	explicit skip_list_iterator(Base_ptr x):
	mNode(x)
	{}

	skip_list_iterator(const skip_list_iterator<T>& x):
	mNode(x.mNode)
	{}

	reference operator*() const
	{
		return static_cast<Link_type>(mNode)->value_field;
	}

	pointer operator->() const
	{
		return &static_cast<Link_type>(mNode)->value_field;
	}

	Self_type& operator++()
	{
		mNode = mNode->mNext[0];
		return *this;
	}

	Self_type operator++(int)
	{
		Self_type tmp = *this;
		mNode = mNode->mNext[0];
		return tmp;
	}

	Self_type& operator--()
	{
		mNode = mNode->mPrev;
		return *this;
	}

	Self_type operator--(int)
	{
		self_type tmp = *this;
		mNode = mNode->mPrev;
		return tmp;
	}

	bool operator==(const Self_type& rhs) const
	{
		return mNode == rhs.mNode;
	}

	bool operator!=(const Self_type& rhs) const
	{
		return !(this->operator==(rhs));
	}
};

template <class T>
struct skip_list_const_iterator
{
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef const T& reference;
	typedef const T* pointer;
	typedef ptrdiff_t difference_type;

	typedef skip_list_const_iterator<T> Self_type;
	typedef skip_list_node_base::Const_Base_ptr Base_ptr;
	typedef const skip_list_node<T>* Link_type;

	Base_ptr mNode;

	skip_list_const_iterator():
		mNode()
	{}

	explicit skip_list_const_iterator(Base_ptr x):
	mNode(x)
	{}

	skip_list_const_iterator(const skip_list_const_iterator<T>& itr):
      mNode(itr.mNode) 
	{
	}

	template<class U >
	skip_list_const_iterator(const skip_list_iterator<U>& itr):
      mNode(itr.mNode) 
	{
	}

	skip_list_const_iterator(const skip_list_iterator<T>& itr):
      mNode(itr.mNode) 
	{
	}

	reference operator*() const
	{
		return static_cast<Link_type>(mNode)->value_field;
	}

	pointer operator->() const
	{
		return &static_cast<Link_type>(mNode)->value_field;
	}

	Self_type& operator++()
	{
		mNode = mNode->mNext[0];
		return *this;
	}

	Self_type operator++(int)
	{
		Self_type tmp = *this;
		mNode = mNode->mNext[0];
		return tmp;
	}

	Self_type& operator--()
	{
		mNode = mNode->mPrev;
		return *this;
	}

	Self_type operator--(int)
	{
		self_type tmp = *this;
		mNode = mNode->mPrev;
		return tmp;
	}

	bool operator==(const Self_type& rhs) const
	{
		return mNode == rhs.mNode;
	}

	bool operator!=(const Self_type& rhs) const
	{
		return !(this->operator==(rhs));
	}
};

template<typename T> 
inline bool operator==(const skip_list_const_iterator<T>& lhs,
               const skip_list_const_iterator<T>& rhs) 
{ 
	return lhs.mNode == rhs.mNode; 
}

template<typename T>
inline bool operator!=(const skip_list_const_iterator<T>& lhs,
            const skip_list_const_iterator<T>& rhs) 
{ 
	return lhs.mNode != rhs.mNode; 
}

template<typename T> 
inline bool operator==(const skip_list_iterator<T>& lhs,
               const skip_list_const_iterator<T>& rhs) 
{ 
	return lhs.mNode == rhs.mNode; 
}

template<typename T>
inline bool operator!=(const skip_list_iterator<T>& lhs,
            const skip_list_const_iterator<T>& rhs) 
{ 
	return lhs.mNode != rhs.mNode; 
}

template< class Key, class T, class KeyOfValue, class Cmp, class A = std::allocator<T> >
class skip_list
{
public:
	typedef Key															 key_type;
	typedef T															 value_type;
	typedef T*															 pointer;
	typedef T&															 reference;
	typedef const T&													 const_reference;
	typedef const skip_list_node<T>*									 Const_Link_type;
	typedef skip_list_node<T>*											 Link_type;
	typedef size_t														 size_type;
	typedef ptrdiff_t													 difference_type;

	typedef skip_list_iterator<value_type>								 iterator;
    typedef skip_list_const_iterator<value_type>						 const_iterator;

    typedef std::reverse_iterator<iterator>								 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>						 const_reverse_iterator;
	
private:
	typedef skip_list_node_base::Base_ptr								 Base_ptr;
	typedef A															 allocator_type;
	typedef typename A::template rebind<skip_list_node_base*>::other     Base_Link_allocator;
	typedef typename A::template rebind<skip_list_node<T> >::other       Node_allocator;
	typedef typename A::template rebind<skip_list_node_base >::other     Base_Node_allocator;
	
	template<class key_compare>
	struct skip_list_impl
	{
		key_compare						mKeyCompare;

		Base_ptr						mHead;
		Base_ptr						mEndNode;		

		A								mAlloc;
		Base_Link_allocator				mBaseLinkAlloc;
		Node_allocator					mNodeAlloc;
		Base_Node_allocator				mBaseNodeAlloc;
		
		size_type						max_lvl;
		size_type						lvl;
		size_type						mNodeCount;

		skip_list_impl(int sz = 128):
			mKeyCompare(), mHead(), mEndNode(), mNodeCount(0), mAlloc(), mBaseLinkAlloc(), mNodeAlloc(), max_lvl(sz), lvl(0)
		{
			impl_init();
		}
		
		skip_list_impl(const key_compare& cmp, const A& alloc, const Base_Link_allocator& link_alloc = Base_Link_allocator(), const Node_allocator& node_alloc = Node_allocator(), int sz = 10):
			mKeyCompare(cmp), 
			mHead(), 
			mEndNode(), 
			mNodeCount(0), 
			mAlloc(alloc), 
			mBaseLinkAlloc(link_alloc), 
			mNodeAlloc(node_alloc),
			max_lvl(sz), 
			lvl(0)
		{
			impl_init();
		}
		
		skip_list_impl(const skip_list_impl& rhs):
			mKeyCompare(rhs.mKeyCompare), 
			mHead(), 
			mEndNode(), 
			mNodeCount(), 
			mAlloc(rhs.mAlloc), 
			mBaseLinkAlloc(),
			mNodeAlloc(),
			max_lvl(rhs.max_lvl),
			lvl(rhs.lvl)
		{
			impl_init();
		}

		~skip_list_impl()
		{
			while(mHead->mNext[0] != mEndNode)
			{
				Link_type lnk = static_cast<Link_type>(mHead->mNext[0]);
				mHead->mNext[0] = lnk->mNext[0];
				destroy_node(lnk);
			}
			//
			mBaseLinkAlloc.deallocate(mHead->mNext, mHead->h);
			mBaseLinkAlloc.deallocate(mEndNode->mNext, mEndNode->h);

			mBaseNodeAlloc.deallocate(mHead, 1);
			mBaseNodeAlloc.deallocate(mEndNode, 1);
		}
		
		Link_type get_node()
		{ 
			return mNodeAlloc.allocate(1); 
		}

		void put_node(Link_type p) 
		{ 
			mNodeAlloc.deallocate(p, 1); 
		}

		Link_type create_node(const value_type& v, size_type h)
		{
			Link_type tmp = get_node();			
			try
			{ 
				mNodeAlloc.construct(tmp, skip_list_node<value_type>(v));
				tmp->h = h;
				tmp->mNext = mBaseLinkAlloc.allocate(h+1);
				for(size_type i = h+1; i > 0; --i)
				{
					tmp->mNext[i-1] = mEndNode;					
				}
				tmp->mPrev = mHead;
			}
			catch(...)
			{
				put_node(tmp);
				throw std::runtime_error("error");
			}
			return tmp;
		}

		void destroy_node(Link_type p)
		{
			mBaseLinkAlloc.deallocate(p->mNext, p->h);
			mAlloc.destroy(&p->value_field);
			put_node(p);
		}

		void swap(skip_list_impl& rhs)
		{
			std::swap(mHead, rhs.mHead);
			std::swap(mEndNode, rhs.mEndNode);
			std::swap(mNodeCount, rhs.mNodeCount);
			std::swap(max_lvl, rhs.max_lvl);
			std::swap(lvl, rhs.lvl);
		}
	private:
		void impl_init()
		{
			mHead = mBaseNodeAlloc.allocate(1);
			mEndNode = mBaseNodeAlloc.allocate(1);
			mEndNode->h = max_lvl + 1;
			mHead->h = max_lvl + 1;
			skip_list_node_base::Base_ptr* p = mBaseLinkAlloc.allocate(mHead->h+1);
			skip_list_node_base::Base_ptr* l = mBaseLinkAlloc.allocate(mHead->h+1);
			for(int i = 0; i <= mHead->h; ++i)
			{
				p[i] = mEndNode;
				l[i] = nullptr;
			}
			mHead->mNext = p;
			mHead->mPrev = nullptr;
			mEndNode->mNext = l;
			mEndNode->mPrev = mHead;
		}
	};

	skip_list_impl<Cmp> mImpl;

	Base_ptr head_base_ptr() const
	{
		return mImpl.mHead;
	}

	Base_ptr end_base_ptr() const
	{
		return mImpl.mEndNode;
	}

	size_type max_lvl() const
	{
		return mImpl.max_lvl;
	}

	size_type current_lvl() const
	{
		return mImpl.lvl;
	}

	template<class Argl, class Argr>
	bool equivalent_keys(const Argl& l_value, const Argr& r_value) const
	{
		return !compare_keys(l_value, r_value) &&
			! compare_keys(r_value, l_value);
	}

	bool equivalent_keys(const key_type& kl, const key_type& kr) const
	{
		return ! compare_keys(kl, kr) && ! compare_keys(kr, kl);
	}

	bool compare_keys(const key_type& kl, const key_type& kr) const
	{
		return mImpl.mKeyCompare(kl, kr);
	}

	template<class Arg1, class Arg2>
	bool compare_keys(const Arg1& arg_l, const Arg2& arg_r) const
	{
		return mImpl.mKeyCompare(KeyOfValue()(arg_l), KeyOfValue()(arg_r) );
	}

	reference Node_value(Const_Link_type lnk) const
	{
		return lnk->value_field;
	}

	reference Node_value(Base_ptr p) const
	{
		return static_cast<Link_type>(p)->value_field;
	}

	const key_type& Node_key(Const_Link_type lnk) const
	{
		return KeyOfValue()( Node_value(lnk) );
	}

	const key_type& Node_key(Base_ptr p) const
	{
		return KeyOfValue()( Node_value(p) );
	}

	Base_ptr find_lower_ptr(const key_type& key) const;

	Base_ptr find_lower_ptr(Base_ptr*& update, const value_type& v);

public:
	//construct
	skip_list() {}

	explicit skip_list(const Cmp& comp, const allocator_type& a = allocator_type() ):
		mImpl(comp, a)
	{}

	skip_list(const skip_list& rhs):
		mImpl(rhs.mImpl)
	{
		this->insert_equal(rhs.begin(), rhs.end());
	}

	skip_list& operator=(const skip_list& rhs);

	//iterators
	iterator begin()
	{
		return iterator(head_base_ptr()->mNext[0]);
	}

	const_iterator begin() const
	{
		return const_iterator(head_base_ptr()->mNext[0]);
	}

	iterator end()
	{
		return iterator(end_base_ptr());
	}

	const_iterator end() const
	{
		return const_iterator(end_base_ptr());
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator( end() );
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator( end() );
	}

	reverse_iterator rend()
	{
		return reverse_iterator( begin() );
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator( begin() );
	}

	//insert

	std::pair<iterator, bool> insert_unique(const value_type& v);

	iterator insert_unique(iterator start_iterator, const value_type& v)
	{
		return insert_unique(v).first;
	}

	template<class In>
	void insert_unique(In first, In last)
	{
		iterator pos = begin();
		for(; first != last; ++first)
			pos = insert_unique(pos, *first);
	}

	iterator insert_equal(Base_ptr pos, const value_type& v);

	iterator insert_equal(const value_type& v)
	{
		return insert_equal(head_base_ptr(), v);
	}

	iterator insert_equal(iterator start_iterator, const value_type& v)
	{
		return insert_equal(start_iterator.mNode, v);
	}

	template<class In>
	void insert_equal(In first, In last)
	{
		iterator pos = begin();
		for(; first != last; ++first)
				insert_equal(*first);
	}

	//erase

	Base_ptr erase(Base_ptr pos);

	size_type erase(const key_type& key);
	
	iterator erase(iterator first, iterator last);

	iterator erase(iterator itr)
	{
		return iterator(erase(itr.mNode) );
	}

	//find

	iterator find(const key_type& key);

	const_iterator find(const key_type& key) const;

	iterator lower_bound( const key_type& key);

	iterator upper_bound( const key_type& key);

	std::pair<iterator, iterator> equal_range(const key_type& key);

	const_iterator lower_bound( const key_type& key) const;

	const_iterator upper_bound( const key_type& key) const;
	
	std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

	size_type count(const key_type& key) const;

	allocator_type get_allocator() const
	{
		return mImpl.mAlloc;
	}

	Cmp key_comp() const
	{
		return mImpl.mKeyCompare;
	}

	size_type size() const
	{
		return mImpl.mNodeCount;
	}

	size_type max_size() const
	{
		return mImpl. mNodeAlloc.max_size();
	}

	bool empty() const
	{
		return ! size();
	}

	//swap
	void swap(skip_list& rhs)
	{
		this->mImpl.swap(rhs.mImpl);
	}

	void show()
	{
		for(iterator itr = begin(); itr != end(); ++itr)
		{
			std::cout << itr->first << "  " << itr->second << '\n';
			std::cout << "h = " << itr.mNode->h << '\n';
			for(int i = 0; i <= itr.mNode->h; ++i)
				std::cout <<"i = " << i << ": " << static_cast<Link_type>(itr.mNode->mNext[i])->value_field.first << '\n';
			std::cout << "prev: " << static_cast<Link_type>(itr.mNode->mPrev)->value_field.first << '\n' << '\n';
		}
	}
};

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline bool operator==(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	return lhs.size() == rhs.size() 
		&& std::equal(lhs.begin(), lhs.end(), rhs.begin() );
}

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline bool operator<(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline bool operator!=(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	return !(lhs == rhs);
}

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline bool operator>(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	return rhs < lhs;
}

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline bool operator<=(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	return !(rhs < lhs);
}

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline bool operator>=(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	return !(lhs < rhs);
}

template< class Key, class T, class KeyOfValue, class Cmp, class A>
inline void swap(const skip_list<Key, T, KeyOfValue, Cmp, A>& lhs,
					   const skip_list<Key, T, KeyOfValue, Cmp, A>& rhs)
{
	lhs.swap(rhs);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
skip_list<Key, T, KeyOfValue, Comp, Alloc>& skip_list<Key, T, KeyOfValue, Comp, Alloc>::
	operator=(const skip_list& rhs)
{
	this->erase(begin(), end() );
	this->mImpl.mKeyCompare = rhs.mImpl.mKeyCompare;
	this->insert_equal(rhs.begin(), rhs.end() );
	return *this;
}

/////////////////////////////////////////////////////////////////////////////

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::Base_ptr 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::find_lower_ptr(const typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::key_type& key) const
{
	Base_ptr cur = this->head_base_ptr();
	for(size_type i = current_lvl() + 1; i > 0; --i)
	{
		while(cur->mNext[i-1] != end_base_ptr() && 
			compare_keys(Node_key(cur->mNext[i-1]), key) )
			cur = cur->mNext[i-1];
	}
	cur = cur->mNext[0];
	return cur;
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::Base_ptr 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::find_lower_ptr(Base_ptr*& update, const value_type& v)
	{
		Base_ptr cur = this->head_base_ptr();
		for(size_type i = current_lvl() + 1 ; i > 0; --i)
		{
			while(cur->mNext[i-1] != end_base_ptr() &&
				compare_keys(Node_value(cur->mNext[i-1]) , v ) )
				cur = cur->mNext[i-1];
			update[i-1] = cur;
		}
		cur = cur->mNext[0];
		return cur;
	}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
std::pair<typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator, bool> 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::insert_unique(const value_type& v)
{
	Base_ptr* update = mImpl.mBaseLinkAlloc.allocate(mImpl.max_lvl + 1);
	Base_ptr cur_ptr = find_lower_ptr(update, v);
		
	if( cur_ptr != end_base_ptr() && equivalent_keys(Node_value(cur_ptr), v) )
		return std::make_pair(iterator(cur_ptr), false);
		
	size_type new_lvl = rand() % (mImpl.max_lvl);
	if(new_lvl > current_lvl())
	{
		for(size_type i = current_lvl() + 1; i <= new_lvl; ++i)
			update[i] = head_base_ptr();
		mImpl.lvl = new_lvl;
	}

	Link_type new_elem = mImpl.create_node(v, new_lvl);
	for(size_type i = new_lvl + 1; i > 0; --i)
	{
		new_elem->mNext[i-1] = update[i-1]->mNext[i-1];
		update[i-1]->mNext[i-1] = new_elem;
	}
	new_elem->mPrev = update[0];
	new_elem->mNext[0]->mPrev = new_elem;

	mImpl.mBaseLinkAlloc.deallocate(update, mImpl.max_lvl + 1);
	//
	mImpl.mNodeCount++;
	//
	return std::make_pair(iterator(new_elem), true);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::insert_equal(Base_ptr pos, const value_type& v)
{
	Base_ptr* update = mImpl.mBaseLinkAlloc.allocate(mImpl.max_lvl + 1);
	Base_ptr cur_ptr = find_lower_ptr(update, v);
		
	while( cur_ptr != end_base_ptr() && equivalent_keys(Node_value(cur_ptr), v) && 
		(!equivalent_keys(Node_value(pos), v) || (equivalent_keys(Node_value(pos),v) && cur_ptr != pos) ) )
	{
		for(int i = 0; i <= cur_ptr->h; ++i)
			update[i] = cur_ptr;
		cur_ptr = cur_ptr->mNext[0];			
	}
		
	size_type new_lvl = rand() % (mImpl.max_lvl);

	if(new_lvl > current_lvl())
	{
		for(size_type i = current_lvl() + 1; i <= new_lvl; ++i)
			update[i] = head_base_ptr();
		mImpl.lvl = new_lvl;
	}

	Link_type new_elem = mImpl.create_node(v, new_lvl);
	for(size_type i = new_lvl + 1; i > 0; --i)
	{
		new_elem->mNext[i-1] = update[i-1]->mNext[i-1];
		update[i-1]->mNext[i-1] = new_elem;
	}
	new_elem->mPrev = update[0];
	new_elem->mNext[0]->mPrev = new_elem;

	mImpl.mBaseLinkAlloc.deallocate(update, mImpl.max_lvl + 1);
	//
	mImpl.mNodeCount++;
	//
	return iterator(new_elem);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::Base_ptr 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::erase(Base_ptr pos)
{
	Base_ptr res = pos->mNext[0];
	key_type key = Node_key(pos);

	Base_ptr* update = mImpl.mBaseLinkAlloc.allocate(mImpl.max_lvl + 1);
	Base_ptr cur_ptr = find_lower_ptr(update, Node_value(pos) );
		
	while( cur_ptr != end_base_ptr() && equivalent_keys(Node_key(cur_ptr), key) && 
		(!equivalent_keys(Node_key(pos), key) || (equivalent_keys(Node_key(pos), key) && cur_ptr != pos) ) )
	{
		for(int i = 0; i <= cur_ptr->h; ++i)
			update[i] = cur_ptr;
		cur_ptr = cur_ptr->mNext[0];			
	}

	for(size_type i = 0; i <= pos->h; ++i)
	{
		update[i]->mNext[i] = pos->mNext[i];
	}
	res->mPrev = pos->mPrev;
	mImpl.destroy_node(static_cast<Link_type>(pos) );
	mImpl.mNodeCount--;
	//
	return res;
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::size_type 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::erase(const key_type& key)
{
	Base_ptr pos = find_lower_ptr(key);
	
	if(pos == end_base_ptr() || ! equivalent_keys(Node_key(pos), key) )
		return 0;
	else
	{
		size_type count = 0;
		while(equivalent_keys(Node_key(pos), key) )
		{
			Base_ptr tmp = pos;
			pos = pos->mNext[0];
			erase(tmp);
			++count;
		}
		return count;
	}
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::erase(iterator first, iterator last)
{
	while(first != last)
	{
		Base_ptr tmp = first.mNode;
		++first;
		erase(tmp);
	}
	return last;
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::find(const key_type& key)
{
	Base_ptr cur = find_lower_ptr(key);
	if( cur != end_base_ptr() && equivalent_keys(Node_key(cur), key) )
		return iterator(cur);
	return end();
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::const_iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::find(const key_type& key) const
{
	Base_ptr cur = find_lower_ptr(key);
	if( cur != end_base_ptr() && equivalent_keys(Node_key(cur), key) )
		return const_iterator(cur);
	return end();
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::lower_bound( const key_type& key)
{
	Base_ptr cur = find_lower_ptr(key);
	return iterator(cur);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::upper_bound( const key_type& key)
{
	Base_ptr cur = find_lower_ptr(key);
	while(cur != end_base_ptr() && equivalent_keys(Node_key(cur), key) )
		cur = cur->mNext[0];
	return iterator(cur);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
std::pair<typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator, typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::iterator> 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::equal_range(const key_type& key)
{
	Base_ptr cur = find_lower_ptr(key);
	iterator first(cur);
	while(cur != end_base_ptr() && equivalent_keys(Node_key(cur), key) )
		cur = cur->mNext[0];
	return std::make_pair(first, iterator(cur) );
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::const_iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::lower_bound( const key_type& key) const
{
	Base_ptr cur = find_lower_ptr(key);
	return const_iterator(cur);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::const_iterator 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::upper_bound( const key_type& key) const
{
	Base_ptr cur = find_lower_ptr(key);
	while(cur != end_base_ptr() && equivalent_keys(Node_key(cur), key) )
		cur = cur->mNext[0];
	return const_iterator(cur);
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
std::pair<typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::const_iterator, typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::const_iterator> 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::equal_range(const key_type& key) const
{
	Base_ptr cur = find_lower_ptr(key);
	iterator first(cur);
	while(cur != end_base_ptr() && equivalent_keys(Node_key(cur), key) )
		cur = cur->mNext[0];
	return std::make_pair(first, iterator(cur) );
}

template<class Key, class T, class KeyOfValue, class Comp, class Alloc>
typename skip_list<Key, T, KeyOfValue, Comp, Alloc>::size_type 
	skip_list<Key, T, KeyOfValue, Comp, Alloc>::count(const key_type& key) const
{
	Base_ptr cur = find_lower_ptr(key);
	size_type count = 0;
	while(cur != end_base_ptr() &&  equivalent_keys(Node_key(cur), key) )
	{
		cur = cur->mNext[0];
		++count;
	}
	return count;
}