#pragma once

#include <memory>
#include <limits>
#include <initializer_list>

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

template<class T, bool IsConst = false>
class ListIterator
{
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = T;
	using pointer = typename std::conditional<IsConst, const T*, T*>::type;
	using reference = typename std::conditional<IsConst, const T&, T&>::type;
	using difference_type = std::ptrdiff_t;

	ListIterator() : m_item(nullptr) {}

	ListIterator(const ListIterator<value_type, IsConst>& it) : m_item(it.m_item) {}
	ListIterator(const ListIterator<value_type, !IsConst>& it) : m_item(it.getNode()) {}

	template<bool B>
	ListIterator& operator=(const ListIterator<value_type, B>& it)
	{
		m_item = it.getNode();
		return *this;
	}

	explicit ListIterator(ListNode<value_type>* node) : m_item(node) {}

	template<bool B>
	bool operator==(const ListIterator<value_type, B>& it) const
	{
		return m_item == it.m_item;
	}

	template<bool B>
	bool operator!=(const ListIterator<value_type, B>& it) const
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
		ListIterator res = *this;
		this->operator++();
		return res;
	}

	ListIterator operator--(int)
	{
		ListIterator res = *this;
		this->operator--();
		return res;
	}
	
	template<bool B = IsConst>
	typename std::enable_if<!B, reference>::type operator*()
	{
		return m_item->val;
	}
	
	template<bool B = IsConst>
	typename std::enable_if<B, reference>::type operator*() const
	{
		return m_item->val;
	}

	template<bool B = IsConst>
	typename std::enable_if<!B, pointer>::type operator->()
	{
		return &m_item->val;
	}

	template<bool B = IsConst>
	typename std::enable_if<B, pointer>::type operator->() const
	{
		return &m_item->val;
	}

	ListNode<value_type> *getNode()
	{
		return m_item;
	}
	
	ListNode<value_type> * const getNode() const
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
	using iterator = ::ListIterator<value_type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_iterator = ::ListIterator<value_type, true>;
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
		m_alloc(alloc) {}
	
	explicit list(size_type count, const value_type& value, const Allocator& alloc = Allocator()) : 
		list(alloc)
	{
		while (count > 0)
		{
			push_back(value);
			count--;
		}
	}
	
	explicit list(size_type count, const Allocator& alloc = Allocator()) :
		list(alloc)
	{
		while (count > 0)
		{
			emplace_back();
			count--;
		}
	}
		
	template<class InputIt, typename = std::enable_if<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
	list(InputIt first, InputIt last, const Allocator& alloc = Allocator()) :
		list(alloc)
	{
		for (auto it = first; it != last; it++)
			push_back(*it);
	}
	
	list(const list& other) :
		list(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator()))
	{
		insert(begin(), other.begin(), other.end());
	}

	list(const list& other, const Allocator& alloc) :
		list(alloc)
	{
		insert(begin(), other.begin(), other.end());
	}

	list(list&& other)
	{
		m_headNode = std::move(other.m_headNode);
		other.m_headNode = nullptr;
		m_alloc = std::move(other.m_alloc);
		m_size = std::move(other.m_size);
	}

	list(list&& other, const Allocator& alloc)
	{
		if (alloc == other.get_allocator())
		{
			m_headNode = move(other.m_headNode);
			other.m_headNode = nullptr;
			m_alloc = move(other.m_alloc);
			m_size = move(other.m_size);
		}
		else
		{
			m_alloc = alloc;
			m_headNode = allocateHeadNode();
			m_size = 0;
			for (auto it = other.begin(); it != other.end(); it++)
				emplace_back(move(*it));
			other.clear();
			other.m_headNode = nullptr;
		}
	}
	
	list(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
		list(alloc)
	{
		for (auto item : init)
			emplace_back(item);
	}

	~list()
	{
		if (m_headNode)
		{
			auto nodeAlloc = node_allocator_type();
			clear();
			std::allocator_traits<node_allocator_type>::deallocate(nodeAlloc, m_headNode, 1);
		}
	}

	allocator_type get_allocator() const
	{
		return m_alloc;
	}
	
	iterator insert(const_iterator pos, const value_type& value)
	{
		::ListNode<value_type> *node = insertNode(pos.getNode()->prev, pos.getNode());
		std::allocator_traits<Allocator>::construct(m_alloc, &node->val, value);
		m_size++;
		return iterator(node);
	}

	iterator insert(const_iterator pos, value_type&& value)
	{
		return emplace(pos, std::move(value));
	}

	iterator insert(const_iterator pos, size_type count, const value_type& value)
	{
		while (count > 0)
		{
			pos = insert(pos, value);
			count--;
		}
		return pos;
	}
	
	template<class InputIt, typename = std::enable_if<std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>>>
	iterator insert(const_iterator pos, InputIt first, InputIt last)
	{
		iterator res = pos;
		for (auto it = first; it != last; it++)
			res = insert(pos, *it);
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
		std::allocator_traits<Allocator>::construct(m_alloc, &node->val, std::forward<Args>(args)...);
		m_size++;
		return iterator(node);
	}

	template<class... Args>
	reference emplace_back(Args&&... args)
	{
		return *emplace<Args...>(end(), std::forward<Args>(args)...);
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
		destroyNode(pos.getNode());
		return res;
	}

	iterator erase(const_iterator first, const_iterator last)
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

	void clear() noexcept
	{
		m_size = 0;
		::ListNode<value_type>* node = m_headNode->next;
		while (node != m_headNode)
			node = destroyNode(node);
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
		return const_iterator(m_headNode->next);
	}

	const_iterator cbegin() const noexcept
	{
		return const_iterator(m_headNode->next);
	}

	iterator end() noexcept
	{
		return iterator(m_headNode);
	}

	const_iterator end() const noexcept
	{
		return const_iterator(m_headNode);
	}

	const_iterator cend() const noexcept
	{
		return const_iterator(m_headNode);
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(begin());
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
	using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<::ListNode<T>>;

	::ListNode<value_type>* allocateNode(::ListNode<value_type>* prev, ::ListNode<value_type>* next)
	{
		auto nodeAlloc = node_allocator_type();
		::ListNode<value_type> *res = std::allocator_traits<node_allocator_type>::allocate(nodeAlloc, 1);
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
	
	::ListNode<value_type>* destroyNode(::ListNode<value_type>* node)
	{
		auto nodeAlloc = node_allocator_type();
		std::allocator_traits<Allocator>::destroy(m_alloc, &node->val);
		::ListNode<value_type>* res = node->next;
		std::allocator_traits<node_allocator_type>::deallocate(nodeAlloc, node, 1);
		return res;
	}
	
	allocator_type m_alloc;
	::ListNode<value_type>* m_headNode;
	size_type m_size;

};

}
