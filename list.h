#pragma once

#include <memory>

//namespace
//{

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
	ListIterator() : m_item(nullptr) {}
	ListIterator(const ListIterator<T>& it) 
	{
		m_item = it.m_item;
	}
	ListIterator& operator=(const ListIterator<T>& it) 
	{
		m_item = it.m_item;
		return *this;
	}

	bool operator!=(const ListIterator<T>& it) const
	{
		return m_item != it.m_item;
	}

	ListIterator& operator++()
	{
		m_item = m_item->next;
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

//}

namespace blk
{

template<class T>
class list
{
public:
	typedef ::ListIterator<T> iterator;

	explicit list() : m_end(ListIterator<T>()), m_rend(ListIterator<T>()) {}
	
	~list()
	{
		ListNode<T> *cur = m_begin.getNode();
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

	void push_back(const T& value)
	{
		if (m_rbegin.getNode() == nullptr)
		{
			ListNode<T> *node = new ListNode<T>(value);
			ListIterator<T> it;
			it.getNode() = node;
			m_begin = m_rbegin = it;
		}
		else
		{
			ListNode<T> *node = new ListNode<T>(value);
			ListIterator<T> it;
			m_rbegin.getNode()->next = node;
			node->prev = m_rbegin.getNode();
			it.getNode() = node;
			m_rbegin = it;
		}
	}

	T& front()
	{
		return *m_begin;
	}

	T& back()
	{
		return *m_rbegin;
	}

	iterator begin()
	{
		return m_begin;
	}

	iterator end()
	{
		return m_end;
	}

private:
	iterator m_begin;
	iterator m_rbegin;
	iterator m_end;
	iterator m_rend;
};

}