#pragma once

#include <memory>
#include <limits>
#include <initializer_list>

namespace blk
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

	ListIterator();
	ListIterator(const ListIterator<value_type, IsConst>& it);
	ListIterator(const ListIterator<value_type, !IsConst>& it);
	explicit ListIterator(ListNode<value_type>* node);

	template<bool B>
	ListIterator& operator=(const ListIterator<value_type, B>& it);


	template<bool B = IsConst>
	bool operator==(const ListIterator<value_type, B>& it) const;

	template<bool B = IsConst>
	bool operator!=(const ListIterator<value_type, B>& it) const;

	ListIterator& operator++();
	ListIterator& operator--();
	ListIterator operator++(int);
	ListIterator operator--(int);

	template<bool B = IsConst, typename Enabled = typename std::enable_if<!B, reference>::type>
	Enabled operator*();

	template<bool B = IsConst, typename Enabled = typename std::enable_if<B, reference>::type>
	Enabled operator*() const;

	template<bool B = IsConst, typename Enabled = typename std::enable_if<!B, pointer>::type>
	Enabled operator->();

	template<bool B = IsConst, typename Enabled = typename std::enable_if<B, pointer>::type>
	Enabled operator->() const;

	ListNode<value_type> * getNode();
	ListNode<value_type> * const getNode() const;

private:
	ListNode<T> *m_item;
};

template<class T, class Allocator = std::allocator<T>>
class list
{
public:
	using value_type = T;
	using allocator_type = Allocator;
	using iterator = ListIterator<value_type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_iterator = ListIterator<value_type, true>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using size_type = size_t;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using difference_type = std::ptrdiff_t;

	list();
	explicit list(const Allocator& alloc);
	explicit list(size_type count, const value_type& value, const Allocator& alloc = Allocator());
	explicit list(size_type count, const Allocator& alloc = Allocator());

	template<class InputIt, typename Enabled = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value>>
	list(InputIt first, InputIt last, const Allocator& alloc = Allocator());
	list(const list& other);
	list(const list& other, const Allocator& alloc);
	list(list&& other);
	list(list&& other, const Allocator& alloc);
	list(std::initializer_list<T> init, const Allocator& alloc = Allocator());
	~list();

	allocator_type get_allocator() const;
	iterator insert(const_iterator pos, const value_type& value);
	iterator insert(const_iterator pos, value_type&& value);
	iterator insert(const_iterator pos, size_type count, const value_type& value);

	template<class InputIt, typename Enabled = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value>>
	iterator insert(const_iterator pos, InputIt first, InputIt last);

	void push_front(const value_type& value);
	void push_back(const value_type& value);
	template<class... Args>
	iterator emplace(const_iterator pos, Args&&... args);
	template<class... Args>
	reference emplace_back(Args&&... args);
	template<class... Args>
	reference emplace_front(Args&&... args);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	void clear() noexcept;
	void pop_back();
	void pop_front();
	void remove(const value_type& value);
	template<class UnaryPredicate>
	void remove_if(UnaryPredicate p);
	value_type& front();
	const value_type& front() const;
	value_type& back();
	const value_type& back() const;
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;
	size_type size() const noexcept;
	size_type max_size() const noexcept;
	bool empty() const noexcept;
private:
	using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<ListNode<T>>;

	ListNode<value_type>* allocateNode(ListNode<value_type>* prev, ListNode<value_type>* next);
	ListNode<value_type>* allocateHeadNode();
	ListNode<value_type>* insertNode(ListNode<value_type>* prev, ListNode<value_type>* next);
	ListNode<value_type>* destroyNode(ListNode<value_type>* node);

	allocator_type m_alloc;
	ListNode<value_type>* m_headNode;
	size_type m_size;
};

}

#include "../src/list.cpp"
