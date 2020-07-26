#pragma once

namespace Yelo {
	namespace Memory {
		class c_physical_memory_allocation {
			void   *m_base_address;
			size_t m_size;
			struct s_flags {
				unsigned valid : 1;
				unsigned allocated : 1;
			}      m_flags;
			uint m_error_code;
		public:
			// Check whether this allocation is both valid and the memory is allocated
			bool IsValid() const { return m_flags.valid; }

			// Get the base address of this allocation
			void *GetBaseAddress() const { return m_base_address; }

			// Get the size of this allocation
			size_t GetSize() const { return m_size; }

			// Get the platform-specific error code gathered from a bad allocation
			uint GetErrorCode() const { return m_error_code; }

			// Initializes an instance with the given values
			// If the values are invalid, operations will safely fail and IsValid() will return false
			c_physical_memory_allocation(void *base_address, size_t size) : m_base_address(base_address), m_size(size), m_flags(), m_error_code(0) {
				m_flags.valid = base_address != nullptr && size > 0;
			}

			// Renders the instance invalid, Free'ing the memory as well
			~c_physical_memory_allocation() {
				this->Free();

				m_base_address = nullptr;
				m_size         = 0;
				m_flags        = s_flags();
			}

			// Allocate the memory at the given base address. Returns false if an error occurs
			bool Allocate() {
				void *address = VirtualAlloc(m_base_address, m_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

				if (address == nullptr || address != m_base_address)
					m_error_code = GetLastError();
				else
					m_error_code = ERROR_SUCCESS;

				return m_error_code == ERROR_SUCCESS;
			}

			// Frees the allocated memory at the given base address. Returns false if an error occurs
			bool Free() {
				bool result = true;

				if (m_flags.valid && m_flags.allocated) {
					result       = VirtualFree(m_base_address, 0, MEM_RELEASE) != FALSE;
					m_error_code = result ? ERROR_SUCCESS : GetLastError();
				}

				return result;
			}
		};
	};
};
