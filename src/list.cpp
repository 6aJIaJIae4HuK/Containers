#include "../include/list.h"

using namespace blk;

// ListIterator implementation

template<class T, bool IsConst>
ListIterator<T, IsConst>::ListIterator() : m_item(nullptr) {}

template<class T, bool IsConst>
ListIterator<T, IsConst>::ListIterator(const ListIterator<value_type, IsConst>& it) : m_item(it.m_item) {}

template<class T, bool IsConst>
ListIterator<T, IsConst>::ListIterator(const ListIterator<value_type, !IsConst>& it) : m_item(it.getNode()) {}

template<class T, bool IsConst>
ListIterator<T, IsConst>::ListIterator(ListNode<value_type>* node) : m_item(node) {}

template<class T, bool IsConst>
template<bool B>
ListIterator<T, IsConst>& ListIterator<T, IsConst>::operator=(const ListIterator<value_type, B>& it)
{
	m_item = it.getNode();
	return *this;
}

template<class T, bool IsConst>
template<bool B>
bool ListIterator<T, IsConst>::operator==(const ListIterator<value_type, B>& it) const
{
	return m_item == it.m_item;
}

template<class T, bool IsConst>
template<bool B>
bool ListIterator<T, IsConst>::operator!=(const ListIterator<value_type, B>& it) const
{
	return !(*this == it);
}

template<class T, bool IsConst>
ListIterator<T, IsConst>& ListIterator<T, IsConst>::operator++()
{
	m_item = m_item->next;
	return *this;
}

template<class T, bool IsConst>
ListIterator<T, IsConst>& ListIterator<T, IsConst>::operator--()
{
	m_item = m_item->prev;
	return *this;
}

template<class T, bool IsConst>
ListIterator<T, IsConst> ListIterator<T, IsConst>::operator++(int)
{
	ListIterator res = *this;
	this->operator++();
	return res;
}

template<class T, bool IsConst>
ListIterator<T, IsConst> ListIterator<T, IsConst>::operator--(int)
{
	ListIterator res = *this;
	this->operator--();
	return res;
}

template<class T, bool IsConst>
template<bool B, typename Enabled>
Enabled ListIterator<T, IsConst>::operator*()
{
	return m_item->val;
}

template<class T, bool IsConst>
template<bool B, typename Enabled>
Enabled ListIterator<T, IsConst>::operator*() const
{
	return m_item->val;
}

template<class T, bool IsConst>
template<bool B, typename Enabled>
Enabled ListIterator<T, IsConst>::operator->()
{
	return &m_item->val;
}

template<class T, bool IsConst>
template<bool B, typename Enabled>
Enabled ListIterator<T, IsConst>::operator->() const
{
	return &m_item->val;
}

template<class T, bool IsConst>
ListNode<typename ListIterator<T, IsConst>::value_type> * ListIterator<T, IsConst>::getNode()
{
	return m_item;
}

template<class T, bool IsConst>
ListNode<typename ListIterator<T, IsConst>::value_type> * const ListIterator<T, IsConst>::getNode() const
{
	return m_item;
}

// List implementation

template<class T, class Allocator>
list<T, Allocator>::list() :
	list(Allocator()) {}

template<class T, class Allocator>
list<T, Allocator>::list(const Allocator& alloc) :
	m_headNode(allocateHeadNode()),
	m_size(0),
	m_alloc(alloc) {}

template<class T, class Allocator>
list<T, Allocator>::list(size_type count, const value_type& value, const Allocator& alloc) :
	list(alloc)
{
	while (count > 0)
	{
		push_back(value);
		count--;
	}
}

template<class T, class Allocator>
list<T, Allocator>::list(size_type count, const Allocator& alloc) :
	list(alloc)
{
	while (count > 0)
	{
		emplace_back();
		count--;
	}
}

template<class T, class Allocator>
template<class InputIt, typename Enabled>
list<T, Allocator>::list(InputIt first, InputIt last, const Allocator& alloc) :
	list(alloc)
{
	for (auto it = first; it != last; it++)
		push_back(*it);
}

template<class T, class Allocator>
list<T, Allocator>::list(const list& other) :
	list(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator()))
{
	insert(begin(), other.begin(), other.end());
}

template<class T, class Allocator>
list<T, Allocator>::list(const list& other, const Allocator& alloc) :
	list(alloc)
{
	insert(begin(), other.begin(), other.end());
}

template<class T, class Allocator>
list<T, Allocator>::list(list&& other)
{
	m_headNode = std::move(other.m_headNode);
	other.m_headNode = nullptr;
	m_alloc = std::move(other.m_alloc);
	m_size = std::move(other.m_size);
}

template<class T, class Allocator>
list<T, Allocator>::list(list&& other, const Allocator& alloc)
{
	if (alloc == other.get_allocator())
	{
		m_headNode = std::move(other.m_headNode);
		other.m_headNode = nullptr;
		m_alloc = std::move(other.m_alloc);
		m_size = std::move(other.m_size);
	}
	else
	{
		m_alloc = alloc;
		m_headNode = allocateHeadNode();
		m_size = 0;
		for (auto it = other.begin(); it != other.end(); it++)
			emplace_back(std::move(*it));
		other.clear();
		other.m_headNode = nullptr;
	}
}

template<class T, class Allocator>
list<T, Allocator>::list(std::initializer_list<T> init, const Allocator& alloc) :
	list(alloc)
{
	insert(begin(), init.begin(), init.end());
}

template<class T, class Allocator>
list<T, Allocator>::~list()
{
	if (m_headNode)
	{
		auto nodeAlloc = node_allocator_type();
		clear();
		std::allocator_traits<node_allocator_type>::deallocate(nodeAlloc, m_headNode, 1);
	}
}

template<class T, class Allocator>
typename list<T, Allocator>::allocator_type list<T, Allocator>::get_allocator() const
{
	return m_alloc;
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, const value_type& value)
{
	::ListNode<value_type> *node = insertNode(pos.getNode()->prev, pos.getNode());
	std::allocator_traits<Allocator>::construct(m_alloc, &node->val, value);
	m_size++;
	return iterator(node);
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, value_type&& value)
{
	return emplace(pos, std::move(value));
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, size_type count, const value_type& value)
{
	while (count > 0)
	{
		pos = insert(pos, value);
		count--;
	}
	return pos;
}

template<class T, class Allocator>
template<class InputIt, typename Enabled>
typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last)
{
	iterator res = pos;
	for (auto it = first; it != last; it++)
		res = insert(pos, *it);
	return res;
}

template<class T, class Allocator>
void list<T, Allocator>::push_front(const value_type& value)
{
	insert(begin(), value);
}

template<class T, class Allocator>
void list<T, Allocator>::push_back(const value_type& value)
{
	insert(end(), value);
}

template<class T, class Allocator>
template<class... Args>
typename list<T, Allocator>::iterator list<T, Allocator>::emplace(const_iterator pos, Args&&... args)
{
	::ListNode<value_type> *node = insertNode(pos.getNode()->prev, pos.getNode());
	std::allocator_traits<Allocator>::construct(m_alloc, &node->val, std::forward<Args>(args)...);
	m_size++;
	return iterator(node);
}

template<class T, class Allocator>
template<class... Args>
typename list<T, Allocator>::reference list<T, Allocator>::emplace_back(Args&&... args)
{
	return *emplace<Args...>(end(), std::forward<Args>(args)...);
}

template<class T, class Allocator>
template<class... Args>
typename list<T, Allocator>::reference list<T, Allocator>::emplace_front(Args&&... args)
{
	return *emplace<Args...>(begin(), args...);
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator pos)
{
	pos.getNode()->prev->next = pos.getNode()->next;
	pos.getNode()->next->prev = pos.getNode()->prev;
	iterator res(pos.getNode()->next);
	destroyNode(pos.getNode());
	return res;
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first, const_iterator last)
{
	if (first == begin() && last == end())
	{
		clear();
		return end();
	}
	else
	{
		while (first != last)
			first = erase(first);
		return first;
	}
}

template<class T, class Allocator>
void list<T, Allocator>::clear() noexcept
{
	m_size = 0;
	::ListNode<value_type>* node = m_headNode->next;
	while (node != m_headNode)
		node = destroyNode(node);
}

template<class T, class Allocator>
void list<T, Allocator>::pop_back()
{
	erase(--end());
}

template<class T, class Allocator>
void list<T, Allocator>::pop_front()
{
	erase(begin());
}

template<class T, class Allocator>
void list<T, Allocator>::remove(const value_type& value)
{
	remove_if([&value](const value_type& v) { return value == v; });
}

template<class T, class Allocator>
template<class UnaryPredicate>
void list<T, Allocator>::remove_if(UnaryPredicate p)
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

template<class T, class Allocator>
typename list<T, Allocator>::value_type& list<T, Allocator>::front()
{
	return *begin();
}

template<class T, class Allocator>
const typename list<T, Allocator>::value_type& list<T, Allocator>::front() const
{
	return *begin();
}

template<class T, class Allocator>
typename list<T, Allocator>::value_type& list<T, Allocator>::back()
{
	return *rbegin();
}

template<class T, class Allocator>
const typename list<T, Allocator>::value_type& list<T, Allocator>::back() const
{
	return *rbegin();
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::begin() noexcept
{
	return iterator(m_headNode->next);
}

template<class T, class Allocator>
typename list<T, Allocator>::const_iterator list<T, Allocator>::begin() const noexcept
{
	return const_iterator(m_headNode->next);
}

template<class T, class Allocator>
typename list<T, Allocator>::const_iterator list<T, Allocator>::cbegin() const noexcept
{
	return const_iterator(m_headNode->next);
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::end() noexcept
{
	return iterator(m_headNode);
}

template<class T, class Allocator>
typename list<T, Allocator>::const_iterator list<T, Allocator>::end() const noexcept
{
	return const_iterator(m_headNode);
}

template<class T, class Allocator>
typename list<T, Allocator>::const_iterator list<T, Allocator>::cend() const noexcept
{
	return const_iterator(m_headNode);
}

template<class T, class Allocator>
typename list<T, Allocator>::reverse_iterator list<T, Allocator>::rbegin() noexcept
{
	return reverse_iterator(end());
}

template<class T, class Allocator>
typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::rbegin() const noexcept
{
	return const_reverse_iterator(end());
}

template<class T, class Allocator>
typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crbegin() const noexcept
{
	return const_reverse_iterator(end());
}

template<class T, class Allocator>
typename list<T, Allocator>::reverse_iterator list<T, Allocator>::rend() noexcept
{
	return reverse_iterator(begin());
}

template<class T, class Allocator>
typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::rend() const noexcept
{
	return const_reverse_iterator(begin());
}

template<class T, class Allocator>
typename list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crend() const noexcept
{
	return const_reverse_iterator(begin());
}

template<class T, class Allocator>
typename list<T, Allocator>::size_type list<T, Allocator>::size() const noexcept
{
	return m_size;
}

template<class T, class Allocator>
typename list<T, Allocator>::size_type list<T, Allocator>::max_size() const noexcept
{
	return std::numeric_limits<size_type>::max();
}

template<class T, class Allocator>
bool list<T, Allocator>::empty() const noexcept
{
	return m_size == 0;
}

template<class T, class Allocator>
ListNode<typename list<T, Allocator>::value_type>* list<T, Allocator>::allocateNode(ListNode<value_type>* prev, ListNode<value_type>* next)
{
	auto nodeAlloc = node_allocator_type();
	ListNode<value_type> *res = std::allocator_traits<node_allocator_type>::allocate(nodeAlloc, 1);
	res->next = next;
	res->prev = prev;
	return res;
}

template<class T, class Allocator>
ListNode<typename list<T, Allocator>::value_type>* list<T, Allocator>::allocateHeadNode()
{
	ListNode<value_type> *res = allocateNode(nullptr, nullptr);
	res->next = res->prev = res;
	return res;
}

template<class T, class Allocator>
ListNode<typename list<T, Allocator>::value_type>* list<T, Allocator>::insertNode(ListNode<value_type>* prev, ListNode<value_type>* next)
{
	ListNode<value_type> *node = allocateNode(prev, next);
	node->prev->next = node;
	node->next->prev = node;
	return node;
}

template<class T, class Allocator>
ListNode<typename list<T, Allocator>::value_type>* list<T, Allocator>::destroyNode(ListNode<value_type>* node)
{
	auto nodeAlloc = node_allocator_type();
	std::allocator_traits<Allocator>::destroy(m_alloc, &node->val);
	ListNode<value_type>* res = node->next;
	std::allocator_traits<node_allocator_type>::deallocate(nodeAlloc, node, 1);
	return res;
}
