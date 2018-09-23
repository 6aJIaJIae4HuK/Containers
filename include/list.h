#pragma once

#include <memory>
#include <limits>
#include <initializer_list>
#include <functional>

namespace blk
{
template<class InputIt>
using IsInputIterator = typename std::enable_if<std::is_base_of<std::input_iterator_tag, typename std::iterator_traits<InputIt>::iterator_category>::value>;

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

	template<bool B = IsConst, typename EnabledType = typename std::enable_if<!B, reference>::type>
	EnabledType operator*();

	template<bool B = IsConst, typename EnabledType = typename std::enable_if<B, reference>::type>
	EnabledType operator*() const;

	template<bool B = IsConst, typename EnabledType = typename std::enable_if<!B, pointer>::type>
	EnabledType operator->();

	template<bool B = IsConst, typename EnabledType = typename std::enable_if<B, pointer>::type>
	EnabledType operator->() const;

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

	// Constructors and destructor
	list();
	explicit list(const Allocator& alloc);
	explicit list(size_type count, const value_type& value, const Allocator& alloc = Allocator());
	explicit list(size_type count, const Allocator& alloc = Allocator());
	template<class InputIt, typename Enabled = IsInputIterator<InputIt>>
	list(InputIt first, InputIt last, const Allocator& alloc = Allocator());
	list(const list& other);
	list(const list& other, const Allocator& alloc);
	list(list&& other);
	list(list&& other, const Allocator& alloc);
	list(std::initializer_list<T> init, const Allocator& alloc = Allocator());
	~list();

	// Assignments and allocator getter
	list& operator=(const list& other);
	list& operator=(list&& other);
	list& operator=(std::initializer_list<T> init);
	void assign(size_type count, const T& value);
	template<class InputIt, typename Enabled = IsInputIterator<InputIt>>
	void assign(InputIt first, InputIt last);
	void assign(std::initializer_list<T> init);
	allocator_type get_allocator() const;

	// Element access
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	
	// Iterators
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

	// Capacity
	bool empty() const noexcept;
	size_type size() const noexcept;
	size_type max_size() const noexcept;

	// Modifiers
	void clear() noexcept;
	iterator insert(const_iterator pos, const value_type& value);
	iterator insert(const_iterator pos, value_type&& value);
	iterator insert(const_iterator pos, size_type count, const value_type& value);
	template<class InputIt, typename Enabled = IsInputIterator<InputIt>>
	iterator insert(const_iterator pos, InputIt first, InputIt last);
	iterator insert(const_iterator pos, std::initializer_list<T> init);
	template<class... Args>
	iterator emplace(const_iterator pos, Args&&... args);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	void push_front(const value_type& value);
	void push_front(value_type&& value);
	void push_back(const value_type& value);
	void push_back(value_type&& value);
	template<class... Args>
	reference emplace_back(Args&&... args);
	template<class... Args>
	reference emplace_front(Args&&... args);
	void pop_back();
	void pop_front();
	void resize(size_type count);
	void resize(size_type count, const value_type& value);
	void swap(list& other);

	// Operations
	void merge(list& other);
	void merge(list&& other);
	template <class Compare>
	void merge(list& other, Compare comp);
	template <class Compare>
	void merge(list&& other, Compare comp);
	void splice(const_iterator pos, list& other);
	void splice(const_iterator pos, list&& other);
	void splice(const_iterator pos, list& other, const_iterator it);
	void splice(const_iterator pos, list&& other, const_iterator it);
	void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);
	void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last);
	void remove(const value_type& value);
	template<class UnaryPredicate>
	void remove_if(UnaryPredicate p);
	void reverse() noexcept;
	void unique();
	template<class BinaryPredicate>
	void unique(BinaryPredicate p);
	void sort();
	template<class Compare>
	void sort(Compare comp);

private:
	using node_type = ListNode<T>;
	using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;

	void unique_equal(std::function<bool(const T& left, const T& right)> equalFunc);
	node_type* allocateNode(node_type* prev, node_type* next);
	node_type* allocateHeadNode();
	node_type* insertNode(node_type* prev, node_type* next);
	node_type* destroyNode(node_type* node);
	void commonSplice(const_iterator pos, list& other);
	void commonSplice(const_iterator pos, list& other, const_iterator it);
	void commonSplice(const_iterator pos, list& other, const_iterator first, const_iterator last);
	void sort(node_type*& first, node_type*& last, std::function<bool(const T& left, const T& right)> lessFunc);

	allocator_type m_alloc;
	node_type* m_headNode;
	size_type m_size;
};

template<class T, class Alloc>
bool operator==(const list<T, Alloc>& left, const list<T, Alloc>& right);
template<class T, class Alloc>
bool operator!=(const list<T, Alloc>& left, const list<T, Alloc>& right);
template<class T, class Alloc>
bool operator<(const list<T, Alloc>& left, const list<T, Alloc>& right);
template<class T, class Alloc>
bool operator<=(const list<T, Alloc>& left, const list<T, Alloc>& right);
template<class T, class Alloc>
bool operator>(const list<T, Alloc>& left, const list<T, Alloc>& right);
template<class T, class Alloc>
bool operator>=(const list<T, Alloc>& left, const list<T, Alloc>& right);

}

namespace std
{
	template<class T, class Alloc>
	void swap(blk::list<T, Alloc>& left, blk::list<T, Alloc>& right);
}

#include "../src/list.cpp"
