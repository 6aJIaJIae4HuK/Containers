class TestClass
{
public:
	TestClass() = delete;
	explicit TestClass(int i) :
		m_ptr(new int(i)) {}

	TestClass(const TestClass& other)
	{
		assign(other);
	}

	TestClass(TestClass&& other) :
		m_ptr(other.m_ptr)
	{
		other.m_ptr = nullptr;
	}
	
	TestClass& operator=(TestClass&& other)
	{
		if (this == &other)
			return *this;

		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
	}

	TestClass& operator=(const TestClass& other)
	{
		delete m_ptr;
		assign(other);
		return *this;
	}

	~TestClass()
	{
		if (m_ptr)
			delete m_ptr;
	}
	
	int getValue() const
	{
		return *m_ptr;
	}

private:

	void assign(const TestClass& other)
	{
		m_ptr = new int(other.getValue());
	}
	int* m_ptr;
};