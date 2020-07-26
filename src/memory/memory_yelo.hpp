/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

// NOTE: it is not advisable to actually use the ARRAY variants with types which define their own destructors

// BLAM_* variants directly use the engine's normal C memory functions for the target platform, sans any special ops (eg, auto-nulling)




#define YELO_MALLOC(size, fill_with_garbage)   malloc( size )
#define YELO_NEW_ARRAY(type, count)            new type[count]
#define YELO_NEW(type)                     new type

#define YELO_FREE(ptr)                     Yelo::Memory::FreeWithNull(ptr)
#define YELO_DELETE(ptr)                  Yelo::Memory::DeleteWithNull(ptr)
#define YELO_DELETE_ARRAY(ptr)               Yelo::Memory::DeleteArrayWithNull(ptr)

#define YELO_REALLOC(ptr, new_size)            realloc(ptr, new_size)

//RENEW_ARRAY and RENEW are only defined for the sapien build target.
//TODO: Look into implementing these in platform-agnostic manners
#define YELO_RENEW_ARRAY(type, ptr, count) //
#define YELO_RENEW(type, ptr) //

#define BLAM_MALLOC(size)                  Yelo::blam::system_malloc( (size) )
#define BLAM_FREE(ptr)                     Yelo::blam::system_free( (ptr) )
#define BLAM_REALLOC(ptr, new_size)            Yelo::blam::system_realloc( (ptr), (new_size))

#if defined(YELO_MALLOC)
#define YELO_NEW_CTOR(type, ...)            new ( YELO_NEW(type) ) type(__VA_ARGS__)

#define YELO_MALLOC_UNIQUE(size, fill_with_garbage)::std::unique_ptr<void,   Yelo::Memory::memory_deleter<void>>         ( YELO_MALLOC(size, fill_with_garbage) )
#define YELO_NEW_ARRAY_UNIQUE(type, count)  ::std::unique_ptr<type[],   Yelo::Memory::memory_array_deleter<type>>   ( YELO_NEW_ARRAY(type, count) )
#define YELO_NEW_UNIQUE(type)::std::unique_ptr<type,   Yelo::Memory::memory_deleter<type>>         ( YELO_NEW(type) )
#define YELO_NEW_UNIQUE_CTOR(type, ...)::std::unique_ptr<type,   Yelo::Memory::memory_deleter<type>>         (::std::move( YELO_NEW_CTOR(type, __VA_ARGS__) ))
#endif

namespace Yelo {
	namespace Memory {
		// Nulls [pointer] before returning
		template <typename T>
		void FreeWithNull(T *&pointer) {
			if (pointer != nullptr) {
				free(pointer);
			}

			pointer = nullptr;
		}

		// Nulls [pointer] before returning
		template <typename T>
		void DeleteWithNull(T *&pointer) {
			if (pointer != nullptr) {
				pointer->~T();
				delete pointer;
			}

			pointer = nullptr;
		}

		// Nulls [pointer] before returning
		template <typename T>
		void DeleteArrayWithNull(T *&pointer) {
			if (pointer != nullptr)
				delete[] pointer;

			pointer = nullptr;
		}

		// Primarily a deleter for::std::unique_ptr
		template <typename T>
		struct memory_deleter {
			inline void operator ()(T *ptr) const { YELO_DELETE(ptr); }
		};

		template <typename T>
		struct memory_array_deleter {
			inline void operator ()(T *ptr) const { YELO_DELETE_ARRAY(ptr); }
		};

		// based on http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx
		// TODO: needs to be updated to comply with C++11
		// http://en.cppreference.com/w/cpp/concept/Allocator
		template <typename T>
		struct memory_stl_allocator {
			typedef T         value_type;
			typedef size_t    size_type;
			// typedef ptrdiff_t difference_type;
			typedef T         *pointer;
			typedef const T   *const_pointer;
			typedef T         &reference;
			typedef const T   &const_reference;

			pointer address(reference r) const { return &r; }

			const_pointer address(const_reference r) const { return &r; }

			size_type max_size() const {
				// The following has been carefully written to be independent of
				// the definition of size_t and to avoid signed/unsigned warnings.
				return (static_cast<size_type>(0) - static_cast<size_type>(1)) / sizeof(T);
			}

			// The following must be the same for all allocators.
			template <typename U>
			struct rebind {
				typedef memory_stl_allocator<U> other;
			};

			bool operator !=(const memory_stl_allocator &other) const {
				return !(*this == other);
			}

			// Returns true if and only if storage allocated from *this
			// can be deallocated from other, and vice versa.
			// Always returns true for stateless allocators.
			bool operator ==(const memory_stl_allocator &other) const {
				return true;
			}

			// Default constructor, copy constructor, rebinding constructor, and destructor.
			// Empty for stateless allocators.
			memory_stl_allocator() {}

			memory_stl_allocator(const memory_stl_allocator &) {}

			template <typename U>
			memory_stl_allocator(const memory_stl_allocator<U> &) {}

			~memory_stl_allocator() {}

			void construct(pointer const p, const_reference t) const {
				void *const pv = reinterpret_cast<void *>(p);

				new(pv) T(t);
			}

			void destroy(pointer const p) const {
				p->~T();
			}

			pointer allocate(const size_type n) const {
				// The return value of allocate(0) is unspecified.
				// We return NULL in order to avoid depending
				// on malloc(0)'s implementation-defined behavior
				// (the implementation can define malloc(0) to return NULL,
				// in which case the bad_alloc check below would fire).
				// All allocators can return NULL in this case.
				if (n == 0)
					return nullptr;

				// All allocators should contain an integer overflow check.
				// The Standardization Committee recommends that::std::length_error
				// be thrown in the case of integer overflow.
				if (n > max_size()) {
					throw ::std::length_error(__FUNCTION__); // " - Integer overflow."));
				}

				void *const pv = YELO_MALLOC(n * sizeof(T), false);

				// Allocators should throw::std::bad_alloc in the case of memory allocation failure.
				if (pv == nullptr)
					throw ::std::bad_alloc();

				return static_cast<pointer>(pv);
			}

			void deallocate(pointer p, const size_type n) const {
				Yelo::Memory::FreeWithNull<pointer *>(p);

			}

			// The following will be the same for all allocators that ignore hints.
			template <typename U>
			pointer allocate(const size_type n, const U * /* const hint */) const {
				return allocate(n);
			}

			// Allocators are not required to be assignable
			memory_stl_allocator &operator =(const memory_stl_allocator &) = delete;
		};
	};
};
