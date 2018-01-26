#pragma once

#include <memory>

namespace
{

template<class T>
struct ListNode
{
	ListNode(const T& val) : val(val), next(nullptr), prev(nullptr) {}
	~ListNode() = default;
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

	bool operator==(const ListIterator<value_type>& it) const
	{
		return m_item == it.m_item;
	}

	bool operator!=(const ListIterator<value_type>& it) const
	{
		return m_item != it.m_item;
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

	explicit list(const Allocator& alloc = Allocator()) : 
		m_begin(iterator()),
		m_end(iterator()),
		m_size(0),
		m_fake_alloc(alloc),
		m_alloc(real_allocator_type())
	{
		m_end.getNode() = allocateNode(nullptr, nullptr);
		m_begin = m_end;
	}
	
	~list()
	{
		clear();
	}

	iterator insert(iterator pos, const value_type& value)
	{
		::ListNode<value_type> *node = allocateNode(pos.getNode()->prev, pos.getNode());
		node->val = value;
		if (node->prev != nullptr)
			node->prev->next = node;
		pos.getNode()->prev = node;
		iterator it;
		it.getNode() = node;
		if (begin() == pos)
			m_begin = it;
		m_size++;
		return it;
	}

	iterator erase(const_iterator pos)
	{
		iterator res = pos;
		++res;
		pos.getNode()->next->prev = pos.getNode()->prev; //pos can't be end()
		if (pos.getNode()->prev != nullptr)
			pos.getNode()->prev->next = pos.getNode()->next;
		auto node = pos.getNode();
		if (node == begin().getNode())
			m_begin = res;
		m_alloc.deallocate(node, 1);
		m_size--;
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
		m_alloc.deallocate(m_end.getNode(), 1);
	}

	void push_back(const value_type& value)
	{
		insert(end(), value);
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
		return m_begin;
	}

	const_iterator begin() const noexcept
	{
		return m_begin;
	}

	const_iterator cbegin() const noexcept
	{
		return m_begin;
	}

	iterator end() noexcept
	{
		return m_end;
	}

	const_iterator end() const noexcept
	{
		return m_end;
	}

	const_iterator cend() const noexcept
	{
		return m_end;
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
		return begin() == end();
	}

private:
	::ListNode<value_type>* allocateNode(::ListNode<value_type>* prev, ::ListNode<value_type>* next)
	{
		::ListNode<value_type> *res = m_alloc.allocate(1);
		res->next = next;
		res->prev = prev;
		return res;
	}

	allocator_type m_fake_alloc;
	real_allocator_type m_alloc;
	iterator m_begin;
	iterator m_end;
	size_type m_size;

};

}