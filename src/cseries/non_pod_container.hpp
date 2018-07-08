/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

// http://stackoverflow.com/a/7189821/444977

#include <precompile.h>

// A POD-type that contains a non-POD type's data
// http://stackoverflow.com/questions/146452/what-are-pod-types-in-c
// TODO: verify the type parameters are non-POD types using boost or something
template<typename TBase, typename TImpl = TBase>
class c_non_pod_container
{
public: // Has to be public to retain POD-traits
	char m_instance_data[sizeof(TImpl)];

	typedef TBase base_type_t;
	typedef TImpl type_t;

protected:
	TBase* GetPointer()
	{
		return reinterpret_cast<TBase *>(m_instance_data);
	}
	const TBase* GetPointer() const
	{
		return reinterpret_cast<TBase *>(m_instance_data);
	}

	TBase& GetReference()
	{
		return reinterpret_cast<TBase &>(m_instance_data);
	}
	const TBase& GetReference() const
	{
		return reinterpret_cast<TBase &>(m_instance_data);
	}

public:
	// pointer-to-member convenience operator
	TBase* operator->()				{ return GetPointer(); }
	const TBase* operator->() const	{ return GetPointer(); }

	// cast to pointer convenience operator
	operator TBase*()				{ return GetPointer(); }
	operator const TBase*() const	{ return GetPointer(); }

	// cast to reference convenience operator
	operator TBase&()				{ return *GetReference(); }
	operator const TBase&() const	{ return *GetReference(); }

	// Use this for your non-default inplace new operations
	void* AllocationAddress()
	{
		return reinterpret_cast<void *>(m_instance_data);
	}

	// We may want to provide a Construct() which does 'new TImpl;'
	// http://stackoverflow.com/questions/620137/do-the-parentheses-after-the-type-name-make-a-difference-with-new

	// Construct the non-POD type using TImpl's default constructor
	TBase* ConstructDefault()
	{
		return new (AllocationAddress()) TImpl();
	}

	void Destruct()
	{
		GetPointer()->~T();
	}
};
