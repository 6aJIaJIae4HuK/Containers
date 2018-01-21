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

template<class T, bool IsReverse = false>
class ListIterator
{
public:
	ListIterator() : m_item(nullptr) {}
	ListIterator(const ListIterator<T, IsReverse>& it)
	{
		m_item = it.m_item;
	}
	ListIterator& operator=(const ListIterator<T, IsReverse>& it)
	{
		m_item = it.m_item;
		return *this;
	}

	bool operator!=(const ListIterator<T, IsReverse>& it) const
	{
		return m_item != it.m_item;
	}

	ListIterator& operator++()
	{
		if (IsReverse)
			m_item = m_item->prev;
		else
			m_item = m_item->next;
		return *this;
	}

	ListIterator& operator--()
	{
		if (IsReverse)
			m_item = m_item->next;
		else
			m_item = m_item->prev;
		return *this;
	}
	
	T& operator*()
	{
		return m_item->val;
	}
	
	const T& operator*() const
	{
		return m_item->val;
	}

	T *operator->()
	{
		return &m_item->val;
	}

	const T *operator->() const
	{
		return &m_item->val;
	}

	ListNode<T> *&getNode()
	{
		return m_item;
	}

	const ListNode<T> *&getNode() const
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
	typedef T value_type;
	typedef ::ListIterator<T> iterator;
	typedef ::ListIterator<T, true> reverse_iterator;
	typedef const ::ListIterator<T>& const_iterator;
	typedef const ::ListIterator<T, true>& const_reverse_iterator;
	typedef size_t size_type;
	explicit list() : m_end(iterator()), m_rend(reverse_iterator()), m_size(0) {}
	
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
		if (m_rbegin.getNode() == nullptr)
		{
			ListNode<value_type> *node = new ListNode<value_type>(value);
			m_begin.getNode() = node;
			m_rbegin.getNode() = node;
		}
		else
		{
			ListNode<value_type> *node = new ListNode<value_type>(value);
			iterator it;
			m_rbegin.getNode()->next = node;
			node->prev = m_rbegin.getNode();
			it.getNode() = node;
			m_rbegin.getNode() = it.getNode();
		}
		m_size++;
	}

	value_type& front()
	{
		return *m_begin;
	}

	const value_type& front() const
	{
		return *m_begin;
	}

	value_type& back()
	{
		return *m_rbegin;
	}

	const value_type& back() const
	{
		return *m_rbegin;
	}

	iterator begin() noexcept
	{
		return m_begin;
	}

	const_iterator begin() const noexcept
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

	reverse_iterator rbegin() noexcept
	{
		return m_rbegin;
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return m_rbegin;
	}

	reverse_iterator rend() noexcept
	{
		return m_rend;
	}

	const_reverse_iterator rend() const noexcept
	{
		return m_rend;
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
	reverse_iterator m_rbegin;
	iterator m_end;
	reverse_iterator m_rend;
	size_type m_size;
};

}