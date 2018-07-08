/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <limits>
#include <type_traits>

namespace Yelo
{
	template<typename TEnumClass>
	class c_enum_default_traits
	{
		static_assert(std::is_enum<TEnumClass>::value, "TEnumClass should be an enum (specifically an enum-class)");

	public:
		enum {
			k_number_of = TEnumClass::k_number_of,

			// Is the NONE constant a valid state?
			none_is_valid_state = false,
		};
	};

	template<
		typename TEnumClass,
		typename TStorage, 
		class TTraits = c_enum_default_traits<TEnumClass>
	>
	class c_enum
	{
		static_assert(std::is_enum<TEnumClass>::value, 
			"TEnumClass should be an enum (specifically an enum-class)");
		static_assert(std::is_integral<TStorage>::value, 
			"TStorage should be an integer type");
		static_assert(std::is_signed<TStorage>::value,
			"TStorage should be a signed integer type");

		static_assert(TTraits::k_number_of <= std::numeric_limits<TStorage>::max(),
			"TEnumClass has too many value members for the requested underlying storage");

	public:
		TStorage m_value;

		operator TEnumClass() const
		{
			return static_cast<TEnumClass>(m_value);
		}
		void operator=(TEnumClass e)
		{
			m_value = static_cast<TStorage>(e);
		}
		bool operator==(TEnumClass e) const	{ return m_value == static_cast<TStorage>(e); }
		bool operator!=(TEnumClass e) const	{ return m_value != static_cast<TStorage>(e); }
		bool operator>(TEnumClass e) const	{ return m_value >  static_cast<TStorage>(e); }
		bool operator<(TEnumClass e) const	{ return m_value <  static_cast<TStorage>(e); }
		bool operator>=(TEnumClass e) const	{ return m_value >= static_cast<TStorage>(e); }
		bool operator<=(TEnumClass e) const	{ return m_value <= static_cast<TStorage>(e); }

		c_enum& operator++(TEnumClass e) ///< pre-increment
		{
			// we should never increment -past- the NUMBEROF constant
			assert(m_value < TTraits::k_number_of);

			++m_value;
			return *this;
		}
		c_enum& operator--(TEnumClass e) ///< pre-decrement
		{
			// we should never decrement -below- NONE or ZERO
			if (TTraits::none_is_valid_state)
			{
				assert(m_value > NONE);
			}
			else
			{
				assert(m_value > 0);
			}

			--m_value;
			return *this;
		}
	};

	// Type safe wrapper for an enum-class representing a series of bits
	// TEnumBitsClass: the enum-class type
	// TStorage: the underlying integer type to use for representing the bits as flags
	template<
		typename TEnumBitsClass,
		typename TStorage,
		class TTraits = c_enum_default_traits<TEnumClass>
	>
	class c_flags
	{
		static_assert(std::is_enum<TEnumBitsClass>::value, 
			"TEnumBitsClass should be an enum (specifically an enum-class)");
		static_assert(std::is_integral<TStorage>::value, 
			"TStorage should be an integer type");
		static_assert(std::is_unsigned<TStorage>::value,
			"TStorage should be a unsigned integer type");

		static_assert(TTraits::k_number_of <= BIT_COUNT(TStorage),
			"TEnumBitsClass has too many bit members for the requested underlying storage");

	public:
		TStorage m_flags;

		void Clear()									{ m_flags = 0; }
		void Clear(TStorage mask)						{ m_flags &= ~mask; }

		void Set(TStorage s)							{ m_flags = s; }
		TStorage Get() const							{ return m_flags; }
		TStorage Get(TStorage mask) const				{ return m_flags & mask; }

		bool HasNoFlags(TStorage flags) const			{ return (m_flags & flags) == 0; }
		bool HasAnyFlags(TStorage flags) const			{ return (m_flags & flags) != 0; }
		bool HasAllFlags(TStorage flags) const			{ return (m_flags & flags) == flags; }

		bool HasNoFlags(TEnumBitsClass flags) const		{ return HasNoFlags( static_cast<TStorage>(flags)); }
		bool HasAnyFlags(TEnumBitsClass flags) const	{ return HasAnyFlags(static_cast<TStorage>(flags)); }
		bool HasAllFlags(TEnumBitsClass flags) const	{ return HasAllFlags(static_cast<TStorage>(flags)); }

		bool HasBit(TEnumBitsClass bit_member) const
		{
			return TEST_FLAG(m_flags, static_cast<TStorage>(bit_member));
		}

		bool operator==(const c_flags& f) const
		{
			return m_value == f.m_flags;
		}
		bool operator!=(const c_flags& f) const
		{
			return m_value != f.m_flags;
		}

		static TStorage Flag(TEnumBitsClass bit_member)
		{
			return FLAG(bit_member);
		}
	};
};