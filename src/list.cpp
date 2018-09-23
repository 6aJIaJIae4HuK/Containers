#include <exception>
#include "../include/list.h"

namespace blk
{

class not_implemented : public std::exception
{
public:
	virtual char const * what() const
	{
		return "Function is not implemented yet";
	}
};

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
template<bool B, typename EnabledType>
EnabledType ListIterator<T, IsConst>::operator*()
{
	return m_item->val;
}

template<class T, bool IsConst>
template<bool B, typename EnabledType>
EnabledType ListIterator<T, IsConst>::operator*() const
{
	return m_item->val;
}

template<class T, bool IsConst>
template<bool B, typename EnabledType>
EnabledType ListIterator<T, IsConst>::operator->()
{
	return &m_item->val;
}

template<class T, bool IsConst>
template<bool B, typename EnabledType>
EnabledType ListIterator<T, IsConst>::operator->() const
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
		insert(begin(), other.begin(), other.end());
		other.~list();
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
		m_headNode = nullptr;
	}
}

template<class T, class Allocator>
list<T, Allocator>& list<T, Allocator>::operator=(const list<T, Allocator>& other)
{
	throw not_implemented();
}

template<class T, class Allocator>
list<T, Allocator>& list<T, Allocator>::operator=(list<T, Allocator>&& other)
{
	throw not_implemented();
}

template<class T, class Allocator>
list<T, Allocator>& list<T, Allocator>::operator=(std::initializer_list<T> init)
{
	assign(init);
	return *this;
}

template<class T, class Allocator>
void list<T, Allocator>::assign(size_type count, const T& value)
{
	clear();
	insert(end(), count, value);
}

template<class T, class Allocator>
template<class InputIt, typename Enabled>
void list<T, Allocator>::assign(InputIt first, InputIt last)
{
	clear();
	insert(end(), first, last);
}

template<class T, class Allocator>
void list<T, Allocator>::assign(std::initializer_list<T> init)
{
	clear();
	insert(end(), init);
}
template<class T, class Allocator>
typename list<T, Allocator>::allocator_type list<T, Allocator>::get_allocator() const
{
	return m_alloc;
}

template<class T, class Allocator>
typename list<T, Allocator>::reference list<T, Allocator>::front()
{
	return *begin();
}

template<class T, class Allocator>
typename list<T, Allocator>::const_reference list<T, Allocator>::front() const
{
	return *begin();
}

template<class T, class Allocator>
typename list<T, Allocator>::reference list<T, Allocator>::back()
{
	return *rbegin();
}

template<class T, class Allocator>
typename list<T, Allocator>::const_reference list<T, Allocator>::back() const
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
bool list<T, Allocator>::empty() const noexcept
{
	return m_size == 0;
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
void list<T, Allocator>::clear() noexcept
{
	m_size = 0;
	node_type *node = m_headNode->next;
	while (node != m_headNode)
		node = destroyNode(node);
}

template<class T, class Allocator>
typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, const value_type& value)
{
	node_type *node = insertNode(pos.getNode()->prev, pos.getNode());
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
typename list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, std::initializer_list<T> init)
{
	return insert(init.begin(), init.end());
}

template<class T, class Allocator>
template<class... Args>
typename list<T, Allocator>::iterator list<T, Allocator>::emplace(const_iterator pos, Args&&... args)
{
	node_type *node = insertNode(pos.getNode()->prev, pos.getNode());
	std::allocator_traits<Allocator>::construct(m_alloc, &node->val, std::forward<Args>(args)...);
	m_size++;
	return iterator(node);
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
void list<T, Allocator>::push_front(const value_type& value)
{
	insert(begin(), value);
}

template<class T, class Allocator>
void list<T, Allocator>::push_front(value_type&& value)
{
	insert(begin(), std::move(value));
}

template<class T, class Allocator>
void list<T, Allocator>::push_back(const value_type& value)
{
	insert(end(), value);
}

template<class T, class Allocator>
void list<T, Allocator>::push_back(value_type&& value)
{
	insert(end(), std::move(value));
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
void list<T, Allocator>::resize(size_type count)
{
	size_type curSize = size();
	if (curSize >= count)
	{
		size_type countToRemove = curSize - count;
		while (countToRemove > 0)
		{
			pop_back();
			countToRemove--;
		}
	}
	else
	{
		size_type countToAdd = count - curSize;
		while (countToAdd > 0)
		{
			emplace_back();
			countToAdd--;
		}
	}
}

template<class T, class Allocator>
void list<T, Allocator>::resize(size_type count, const value_type& value)
{
	size_type curSize = size();
	if (curSize >= count)
	{
		size_type countToRemove = curSize - count;
		while (countToRemove > 0)
		{
			pop_back();
			countToRemove--;
		}
	}
	else
	{
		insert(end(), count - curSize, value);
	}
}

template<class T, class Allocator>
void list<T, Allocator>::swap(list& other)
{
	std::swap(m_size, other.m_size);
	std::swap(m_headNode, other.m_headNode);
	std::swap(m_alloc, other.m_alloc);
}

template<class T, class Allocator>
void list<T, Allocator>::merge(list& other)
{

}

template<class T, class Allocator>
void list<T, Allocator>::merge(list&& other)
{
	throw not_implemented();
}

template<class T, class Allocator>
template <class Compare>
void list<T, Allocator>::merge(list& other, Compare comp)
{
	throw not_implemented();
}

template<class T, class Allocator>
template <class Compare>
void list<T, Allocator>::merge(list&& other, Compare comp)
{
	throw not_implemented();
}

template<class T, class Allocator>
void list<T, Allocator>::commonSplice(const_iterator pos, list& other)
{
	if (other.empty())
		return;
	node_type *posNode = pos.getNode();
	node_type *beforePosNode = posNode->prev;
	node_type *firstNode = other.begin().getNode();
	node_type *lastNode = other.end().getNode()->prev;
	beforePosNode->next = firstNode;
	firstNode->prev = beforePosNode;
	posNode->prev = lastNode;
	lastNode->next = posNode;
	m_size += other.size();

	other.m_headNode->next = other.m_headNode;
	other.m_headNode->prev = other.m_headNode;
	other.m_size = 0;
}

template<class T, class Allocator>
void list<T, Allocator>::commonSplice(const_iterator pos, list& other, const_iterator it)
{
	node_type *itNode = it.getNode();
	itNode->prev->next = itNode->next;
	itNode->next->prev = itNode->prev;
	other.m_size--;

	node_type *posNode = pos.getNode();
	node_type *beforePosNode = posNode->prev;
	beforePosNode->next = itNode;
	itNode->prev = beforePosNode;
	posNode->prev = itNode;
	itNode->next = posNode;
	m_size++;
}

template<class T, class Allocator>
void list<T, Allocator>::commonSplice(const_iterator pos, list& other, const_iterator first, const_iterator last)
{
	// TODO: Replace to cyclic commonSplice(pos, other, it)
	size_type size = 0;
	for (const_iterator cur = first; cur != last; cur++)
		size++;

	node_type *posNode = pos.getNode();
	node_type *beforePosNode = posNode->prev;
	node_type *firstNode = first.getNode();
	node_type *lastNode = last.getNode()->prev;

	firstNode->prev->next = lastNode->next;
	lastNode->next->prev = firstNode->prev;

	beforePosNode->next = firstNode;
	firstNode->prev = beforePosNode;
	posNode->prev = lastNode;
	lastNode->next = posNode;

	m_size += size;
	other.m_size -= size;
}

template<class T, class Allocator>
void list<T, Allocator>::splice(const_iterator pos, list& other)
{
	commonSplice(pos, other);
}

template<class T, class Allocator>
void list<T, Allocator>::splice(const_iterator pos, list&& other)
{
	commonSplice(pos, other);
}

template<class T, class Allocator>
void list<T, Allocator>::splice(const_iterator pos, list& other, const_iterator it)
{
	commonSplice(pos, other, it);
}

template<class T, class Allocator>
void list<T, Allocator>::splice(const_iterator pos, list&& other, const_iterator it)
{
	commonSplice(pos, other, it);
}

template<class T, class Allocator>
void list<T, Allocator>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last)
{
	commonSplice(pos, other, first, last);
}

template<class T, class Allocator>
void list<T, Allocator>::splice(const_iterator pos, list&& other, const_iterator first, const_iterator last)
{
	commonSplice(pos, other, first, last);
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
void list<T, Allocator>::reverse() noexcept
{
	if (empty())
		return;
	node_type* head = m_headNode;
	node_type* cur = head;
	while (true)
	{
		node_type* next = cur->next;
		cur->next = cur->prev;
		cur->prev = next;
		if (next == head)
			break;
		cur = next;
	}
}

template<class T, class Allocator>
void list<T, Allocator>::unique_equal(std::function<bool(const T& left, const T& right)> equalFunc)
{
	if (size() <= 2)
		return;
	auto itLeft = begin();
	auto itRight = itLeft;
	itRight++;
	while (itRight != end())
	{
		while (itRight != end() && equalFunc(*itLeft, *itRight))
			itRight = erase(itRight);
		if (itRight != end())
		{
			++itLeft;
			++itRight;
		}
	}
}

template<class T, class Allocator>
void list<T, Allocator>::unique()
{
	unique_equal([](const T& left, const T& right) { return left == right; });
}

template<class T, class Allocator>
template<class BinaryPredicate>
void list<T, Allocator>::unique(BinaryPredicate p)
{
	unique_equal([&p](const T& left, const T& right) { return !p(left, right) && !p(right, left); });
}

template<class T, class Allocator>
void list<T, Allocator>::sort()
{
	sort([](const T& t1, const T& t2) { return t1 < t2; });
}

template<class T, class Allocator>
template<class Compare>
void list<T, Allocator>::sort(Compare comp)
{
	node_type* first = m_headNode->next;
	node_type* last = m_headNode;
	sort(first, last, comp);
}

template<class T, class Allocator>
void list<T, Allocator>::sort(node_type*& first, node_type*& last, std::function<bool(const T& left, const T& right)> lessFunc)
{
	size_type size = 0;
	node_type* cur = first;
	while (cur != last)
	{
		size++;
		cur = cur->next;
	}
	if (size < 2)
		return;
	cur = first;
	for (size_t i = 0; i < size / 2; i++)
		cur = cur->next;
	sort(first, cur, lessFunc);
	sort(cur, last, lessFunc);

	node_type *nodeBeforeInterval = first->prev;
	node_type *nodeAfterInterval = last;
	node_type *leftCurrent = first;
	node_type *rightCurrent = cur;
	node_type *lastSucceed = leftCurrent->prev;

	lastSucceed->next = nullptr;
	rightCurrent->prev->next = nullptr;
	nodeAfterInterval->prev->next = nullptr;

	while (leftCurrent != nullptr || rightCurrent != nullptr)
	{
		if (leftCurrent == nullptr)
		{
			rightCurrent->prev = lastSucceed;
			lastSucceed->next = rightCurrent;
			lastSucceed = rightCurrent;
			rightCurrent = rightCurrent->next;
		}
		else
		if (rightCurrent == nullptr)
		{
			leftCurrent->prev = lastSucceed;
			lastSucceed->next = leftCurrent;
			lastSucceed = leftCurrent;
			leftCurrent = leftCurrent->next;
		}
		else
		if (lessFunc(leftCurrent->val, rightCurrent->val))
		{
			leftCurrent->prev = lastSucceed;
			lastSucceed->next = leftCurrent;
			lastSucceed = leftCurrent;
			leftCurrent = leftCurrent->next;
		}
		else
		{
			rightCurrent->prev = lastSucceed;
			lastSucceed->next = rightCurrent;
			lastSucceed = rightCurrent;
			rightCurrent = rightCurrent->next;
		}
	}

	lastSucceed->next = nodeAfterInterval;
	nodeAfterInterval->prev = lastSucceed;

	first = nodeBeforeInterval->next;
	last = nodeAfterInterval;
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

template<class T, class Allocator>
bool operator==(const list<T, Allocator>& left, const list<T, Allocator>& right)
{
	if (left.size() != right.size())
		return false;
	auto itLeft = left.begin();
	auto itRight = right.begin();
	while (itLeft != left.end() && itRight != right.end())
	{
		if (*itLeft != *itRight)
			return false;
		++itLeft;
		++itRight;
	}
	return true;
}

template<class T, class Allocator>
bool operator!=(const list<T, Allocator>& left, const list<T, Allocator>& right)
{
	return !(left == right);
}

template<class T, class Allocator>
bool operator<(const list<T, Allocator>& left, const list<T, Allocator>& right)
{
	auto itLeft = left.begin();
	auto itRight = right.begin();
	while (itLeft != left.end() && itRight != right.end())
	{
		if (*itLeft < *itRight)
			return true;
		if (*itRight < *itLeft)
			return false;
		++itLeft;
		++itRight;
	}
	return itLeft == left.end() && itRight != right.end();
}

template<class T, class Allocator>
bool operator<=(const list<T, Allocator>& left, const list<T, Allocator>& right)
{
	return !(right < left);
}

template<class T, class Allocator>
bool operator>(const list<T, Allocator>& left, const list<T, Allocator>& right)
{
	return right < left;
}

template<class T, class Allocator>
bool operator>=(const list<T, Allocator>& left, const list<T, Allocator>& right)
{
	return !(left < right);
}

}

namespace std
{

template<class T, class Allocator>
void swap(blk::list<T, Allocator>& left, blk::list<T, Allocator>& right)
{
	left.swap(right);
}

}