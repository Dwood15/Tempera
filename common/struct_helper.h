#pragma once
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Class for defining an interface for blocks of data whose memory layout is not entirely mapped out.
	/// </summary>
	///
	/// <typeparam name="K_SIZE">	Size of the memory block. </typeparam>
	template <const size_t K_SIZE>
	struct TStruct {
		enum { k_size = K_SIZE };

	protected:
		// NOTE: I would use::std::array here, but I have yet to tested how well it plays with xbox modules (ie, Halo2_Xbox)
		byte m_data[K_SIZE];

		template <typename T, const size_t k_offset>
		T GetData() { return *(reinterpret_cast<T *>(&m_data[k_offset])); }

		template <typename T, const size_t k_offset>
		T GetData() const { return *(reinterpret_cast<const T *>(&m_data[k_offset])); }

		template <typename T, const size_t k_offset>
		T *GetDataPtr() { return reinterpret_cast<T *>(&m_data[k_offset]); }

		template <typename T, const size_t k_offset>
		const T *GetDataPtr() const { return reinterpret_cast<const T *>(&m_data[k_offset]); }

		// Usage - "struct s_some_object : TStructImpl(0x40) {};"
#define TStructImpl(size) public TStruct< size >

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>
#define TStructGetImpl(type, name, offset) type Get##name() { return GetData<type, offset>(); } type Get##name() const \
		{ return GetData<type, offset>(); } __cdecl( ( offset + sizeof( type )) <= k_size );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>

		/*   ^ use const here, instead of before the type, in case [type] is defined as something like "long*" */
#define TStructGetPtrImpl(type, name, offset) type const* Get##name() const   { return GetDataPtr<type, offset>(); } __cdecl( ( offset + sizeof( type )) <= k_size );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter for fake TStruct sub-classes. </summary>
		///
		/// <remarks>
		/// 	Requires an 'DATA_OFFSET' constant to be defined, representing the start of the fake sub-class.
		/// </remarks>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the fake sub-class to treat as the get result. </param>
#define TStructSubGetImpl(type, name, offset)      TStructGetImpl(type, name, offset - DATA_OFFSET)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter for fake TStruct sub-classes. </summary>
		///
		/// <remarks>
		/// 	Requires an 'DATA_OFFSET' constant to be defined, representing the start of the fake sub-class.
		/// </remarks>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the fake sub-class to treat as the get result. </param>
#define TStructSubGetPtrImpl(type, name, offset)   TStructGetPtrImpl(type, name, offset - DATA_OFFSET)
	};
