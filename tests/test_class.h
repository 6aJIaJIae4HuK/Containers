class TestClass
{
public:
	TestClass() = delete;
	TestClass(int i) :
		m_ptr(new int(i)) {}
	TestClass(const TestClass& other)
	{
		assign(other);
	}

	TestClass& operator=(const TestClass& other)
	{ 
		assign(other);
		return *this;
	}

	~TestClass()
	{
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