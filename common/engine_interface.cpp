#include "engine_interface.h"
namespace memory {
	bool WriteMemory(void *address, const void *src, size_t size) {
		return address == memcpy(address, src, size);
	}

	template <typename T, size_t size>
	inline bool WriteMemory(void *address, const T (&src)[size]) {
		return WriteMemory(address, src, sizeof(T) * size);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Overwrites a range of foreign memory with a specific byte. </summary>
	///
	/// <param name="address">	Start address of the foreign memory to overwrite. </param>
	/// <param name="value">  	The byte value to use when overwriting. </param>
	/// <param name="count">  	Number of times to write the byte. </param>
	void WriteMemory(void *address, int value, size_t count) {
		memset(address, value, count);
	}

	// makes the 32 bits at [address] equal [value]
	bool WriteMemory(void *address, void *value) {
		*reinterpret_cast<unsigned __int32 *>(address) = reinterpret_cast<uintptr_t>(value);
		return true;
	}

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast

	// At [hook_address] writes 'call [hook]' plus user specified end opcode (total: 6 bytes)
	void CreateHookRelativeCall(void *hook, void *hook_address, byte end) {
		struct relative_call_bytes : Yelo::Memory::Opcode::s_call {
			byte End;
		};
		STAT_ASSERT(relative_call_bytes, 6)
		// call near ....
		// retn\nop
		relative_call_bytes asm_bytes;

		asm_bytes.Op = Enums::_x86_opcode_call_near;

		// relative call to [hook]
		asm_bytes.Address = (reinterpret_cast<intptr_t>(hook)) -
							((reinterpret_cast<intptr_t>(hook_address)) +
							 sizeof(Opcode::s_call));

		asm_bytes.End = end;

		WriteMemory(hook_address, &asm_bytes, sizeof(asm_bytes));
	}

	// At [jmp_address]+1, write the relative address of [to_address] (total: 5 bytes)
	// Writes '0xE9' if [write_opcode]
	// returns the original jmp address (absolute)
	uintptr_t WriteRelativeJmp(void *to_address, void *jmp_address, bool write_opcode) {
		byte real_opcode = Enums::_x86_opcode_jmp_near; // jmp [function]
		if (write_opcode)
			WriteMemory(jmp_address, &real_opcode, sizeof(real_opcode));

		uintptr_t original = (reinterpret_cast<intptr_t>(jmp_address)) + *reinterpret_cast<intptr_t *>( reinterpret_cast<unsigned __int32>(jmp_address) + 1) + sizeof(Opcode::s_call);

		uintptr_t relative = (reinterpret_cast<intptr_t>(to_address)) - ((reinterpret_cast<intptr_t>(jmp_address)) + sizeof(Opcode::s_call));
		WriteMemory((reinterpret_cast<void *>((reinterpret_cast<uintptr_t>(jmp_address)) + 1)), (reinterpret_cast<void *>(relative)));

		return original;
	}

	// At [call_address]+1, write the relative address of [to_address] (total: 5 bytes)
	// Writes '0xE8' if [write_opcode]
	// returns the original call address (absolute)
	uintptr_t WriteRelativeCall(void *to_address, void *call_address, bool write_opcode) {
		byte real_opcode = Enums::_x86_opcode_call_near; // call [function]
		if (write_opcode)
			WriteMemory(call_address, &real_opcode, sizeof(real_opcode));

		uintptr_t original = reinterpret_cast<intptr_t>(call_address) + *reinterpret_cast<intptr_t *>(reinterpret_cast<unsigned long>(call_address) + 1) + sizeof(Opcode::s_call);

		uintptr_t relative = reinterpret_cast<intptr_t>(to_address) - (reinterpret_cast<intptr_t>(call_address) + sizeof(Opcode::s_call));
		WriteMemory(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(call_address) + 1), reinterpret_cast<void *>(relative));

		return original;
	}

}