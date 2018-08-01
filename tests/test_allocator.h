template<class T>
class TestAllocator
{
public:
	using value_type = T;
	using pointer = T*;
	TestAllocator() = default;
	TestAllocator(int val) :m_val(val) {}

	template<class U>
	TestAllocator(const TestAllocator<U>& other) {}
	
	pointer allocate(size_t cnt)
	{
		auto ptr = malloc(cnt * sizeof(value_type));
		auto res = static_cast<pointer>(ptr);
		if (!res)
			throw std::bad_alloc();
		return res;
	}

	void deallocate(pointer ptr, size_t n)
	{
		free(ptr);
	}

	int getValue() const
	{
		return m_val;
	}

private:
	int m_val;
};

template<class T, class U>
bool operator==(const TestAllocator<T>& a1, const TestAllocator<U>& a2) 
{
	return a1.getValue() == a2.getValue();
}

template<class T, class U>
bool operator!=(const TestAllocator<T>& a1, const TestAllocator<U>& a2) 
{
	return a1.getValue() != a2.getValue();
}