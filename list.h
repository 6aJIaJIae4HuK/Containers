#pragma once

#include <memory>

namespace
{

template<class T>
struct ListNode
{
	ListNode() : val(T()), next(nullptr), prev(nullptr) {}
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

	ListIterator& operator++(int)
	{
		++(*this);
		return *this;
	}

	ListIterator& operator--(int)
	{
		--(*this);
		return *this;
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

	const ListNode<value_type> *&getNode() const
	{
		return m_item;
	}
	
private:
	ListNode<T> *m_item;
};

}

namespace blk
{

template<class T>
class list
{
public:
	using value_type = T;
	using iterator = ::ListIterator<value_type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_iterator = const iterator&;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using size_type = size_t;

	explicit list() : m_size(0) 
	{
		m_end.getNode() = new ListNode<value_type>();
		m_begin = m_end;
	}
	
	~list()
	{
		ListNode<value_type> *cur = m_begin.getNode();
		while (cur != nullptr)
		{
			if (cur->next == nullptr)
			{
				delete cur;
				cur = nullptr;
			}
			else
			{
				cur = cur->next;
				delete cur->prev;
			}
		}
	}

	void push_back(const value_type& value)
	{
		ListNode<value_type> *node = new ListNode<value_type>(value);
		node->next = m_end.getNode();
		node->prev = m_end.getNode()->prev;
		if (node->prev != nullptr)
			node->prev->next = node;
		m_end.getNode()->prev = node;
		if (empty())
			m_begin.getNode() = node;
		m_size++;
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
	iterator m_begin;
	iterator m_end;
	size_type m_size;
};

}