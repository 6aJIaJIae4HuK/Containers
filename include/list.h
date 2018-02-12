#pragma once

#include <memory>
#include <limits>

namespace
{

template<class T>
struct ListNode
{
	ListNode(const T& val) : val(val), next(nullptr), prev(nullptr) {}
	T val;
	ListNode<T> *next;
	ListNode<T> *prev;
};

template<class T>
class ListIterator
{
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using difference_type = std::ptrdiff_t;

	ListIterator() : m_item(nullptr) {}

	ListIterator(const ListIterator<value_type>& it)
	{
		m_item = it.m_item;
	}
	ListIterator& operator=(const ListIterator<value_type>& it)
	{
		m_item = it.m_item;
		return *this;
	}

	explicit ListIterator(ListNode<value_type>* node) : m_item(node) {}

	bool operator==(const ListIterator<value_type>& it) const
	{
		return m_item == it.m_item;
	}

	bool operator!=(const ListIterator<value_type>& it) const
	{
		return !(*this == it);
	}

	ListIterator& operator++()
	{
		m_item = m_item->next;
		return *this;
	}

	ListIterator& operator--()
	{
		m_item = m_item->prev;
		return *this;
	}

	ListIterator operator++(int)
	{
		ListIterator<value_type> res = *this;
		this->operator++();
		return res;
	}

	ListIterator operator--(int)
	{
		ListIterator<value_type> res = *this;
		this->operator--();
		return res;
	}

	value_type& operator*()
	{
		return m_item->val;
	}
	
	const value_type& operator*() const
	{
		return m_item->val;
	}

	value_type *operator->()
	{
		return &m_item->val;
	}

	const value_type *operator->() const
	{
		return &m_item->val;
	}

	ListNode<value_type> *&getNode()
	{
		return m_item;
	}

	ListNode<value_type> *getNode() const
	{
		return m_item;
	}

private:
	ListNode<T> *m_item;
};

}

namespace blk
{

template<class T, class Allocator = std::allocator<T>>
class list
{
public:
	using value_type = T;
	using allocator_type = Allocator;
	using real_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<::ListNode<T>>;
	using iterator = ::ListIterator<value_type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_iterator = const iterator;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using size_type = size_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using difference_type = std::ptrdiff_t;

	list() : 
		list(Allocator()) {}
	
	explicit list(const Allocator& alloc) : 
		m_headNode(allocateHeadNode()),
		m_size(0),
		m_fake_alloc(alloc),
		m_alloc(real_allocator_type()) {}
	
	list(size_type count, const value_type& value, const Allocator& alloc = Allocator()) : 
		list(alloc)
	{
		while (count > 0)
		{
			push_back(value);
			count--;
		}
	}
	
	explicit list(size_type count, const Allocator& alloc = Allocator()) :
		list(count, value_type(), alloc) {}
		
	template<class InputIt>
	list(InputIt first, InputIt last, const Allocator& alloc = Allocator()) :
		list(alloc)
	{
		for (auto it = first; it != last; it++)
			push_back(*it);
	}
	
	~list()
	{
		clear();
	}

	allocator_type get_allocator() const
	{
		return m_fake_alloc;
	}
	
	iterator insert(const_iterator pos, const value_type& value)
	{
		::ListNode<value_type> *node = insertNode(pos.getNode()->prev, pos.getNode());
		node->val = value;
		m_size++;
		return iterator(node);
	}

	iterator insert(const_iterator pos, value_type&& value)
	{
		return emplace(pos, std::forward<value_type>(value));
	}

	iterator insert(const_iterator pos, size_type count, const value_type& value)
	{
		iterator res = pos;
		while (count > 0)
		{
			res = insert(res, value);
			count--;
		}
		return res;
	}
	
	template<class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last)
	{
		iterator res = pos;
		for (auto it = first; it != last; it++)
			res = insert(res, *it);
		return res;
	}

	void push_front(const value_type& value)
	{
		insert(begin(), value);
	}

	void push_back(const value_type& value)
	{
		insert(end(), value);
	}

	template<class... Args>
	iterator emplace(const_iterator pos, Args&&... args)
	{
		::ListNode<value_type> *node = insertNode(pos.getNode()->prev, pos.getNode());
		new (&node->val) value_type (std::forward<Args>(args)...);
		m_size++;
		return iterator(node);
	}

	template<class... Args>
	reference emplace_back(Args&&... args)
	{
		return *emplace<Args...>(end(), args...);
	}

	template<class... Args>
	reference emplace_front(Args&&... args)
	{
		return *emplace<Args...>(begin(), args...);
	}

	iterator erase(const_iterator pos)
	{
		pos.getNode()->prev->next = pos.getNode()->next;
		pos.getNode()->next->prev = pos.getNode()->prev;
		iterator res(pos.getNode()->next);
		pos.getNode()->val.~value_type();
		m_alloc.deallocate(pos.getNode(), 1);
		return res;
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		iterator cur = first;
		while (cur != last)
			cur = erase(cur);
		return cur;
	}

	void clear() noexcept
	{
		erase(begin(), end());
		m_alloc.deallocate(m_headNode, 1);
	}

	void pop_back()
	{
		erase(--end());
	}

	void pop_front()
	{
		erase(begin());
	}

	void remove(const value_type& value)
	{
		remove_if([&value](const value_type& v) { return value == v; });
	}

	template<class UnaryPredicate>
	void remove_if(UnaryPredicate p)
	{
		iterator cur = begin();
		while (cur != end())
		{
			if (p(*cur))
				cur = erase(cur);
			else
				++cur;
		}
	}

	value_type& front()
	{
		return *begin();
	}

	const value_type& front() const
	{
		return *begin();
	}

	value_type& back()
	{
		return *rbegin();
	}

	const value_type& back() const
	{
		return *rbegin();
	}

	iterator begin() noexcept
	{
		return iterator(m_headNode->next);
	}

	const_iterator begin() const noexcept
	{
		return iterator(m_headNode->next);
	}

	const_iterator cbegin() const noexcept
	{
		return iterator(m_headNode->next);
	}

	iterator end() noexcept
	{
		return iterator(m_headNode);
	}

	const_iterator end() const noexcept
	{
		return iterator(m_headNode);
	}

	const_iterator cend() const noexcept
	{
		return iterator(m_headNode);
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const noexcept
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator crend() const noexcept
	{
		return reverse_iterator(begin());
	}

	size_type size() const noexcept
	{
		return m_size;
	}

	size_type max_size() const noexcept
	{
		return std::numeric_limits<size_type>::max();
	}

	bool empty() const noexcept
	{
		return m_size == 0;
	}

private:
	::ListNode<value_type>* allocateNode(::ListNode<value_type>* prev, ::ListNode<value_type>* next)
	{
		::ListNode<value_type> *res = m_alloc.allocate(1);
		res->next = next;
		res->prev = prev;
		return res;
	}
	
	::ListNode<value_type>* allocateHeadNode()
	{
		::ListNode<value_type> *res = allocateNode(nullptr, nullptr);
		res->next = res->prev = res;
		return res;
	}

	::ListNode<value_type>* insertNode(::ListNode<value_type>* prev, ::ListNode<value_type>* next)
	{
		::ListNode<value_type> *node = allocateNode(prev, next);
		node->prev->next = node;
		node->next->prev = node;
		return node;
	}

	allocator_type m_fake_alloc;
	real_allocator_type m_alloc;
	::ListNode<value_type>* m_headNode;
	size_type m_size;

};

}