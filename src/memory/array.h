#pragma once

#include "data_iterator.h"
#include "data_array.h"
#include "datum_index.h"

namespace Yelo::Memory {
	struct s_dynamic_array {
		size_t element_size;
		long   count;
		void   *elements;
	};

	STAT_ASSERT(s_dynamic_array, 0xC);

	template <typename DatumT>
	struct DataArrayIteratorResult {
		datum_index index;
		DatumT      *datum;

		DataArrayIteratorResult(datum_index i, DatumT *d) : index(i), datum(d) {}

		DatumT *operator ->() const { return datum; }

		DatumT &operator *() const { return *datum; }

		datum_index::index_t GetAbsoluteIndex() const { return index.index; }
	};

	template <typename DatumT, size_t MaxDatumCount, size_t MaxDatumCountUpgrade = MaxDatumCount>
	class DataArray {
		typedef DataArray<DatumT, MaxDatumCount, MaxDatumCountUpgrade> DataArrayT;

	public:

		s_data_array Header;

	public:
		class Iterator {
			s_data_iterator m_iterator;
			DatumT          *m_current_instance;

		public:
			/**
			 * @function Iterator - Makes new iterator
			 * @param data DataArrayT
			 */
			Iterator(DataArrayT &data) {
				blam::data_iterator_new(m_iterator, &data.Header);
				m_current_instance = nullptr;
			}

			DatumT *Next() {
				return m_current_instance = reinterpret_cast<DatumT *>(blam::data_iterator_next(m_iterator));
			}

			datum_index Current() const { return this->m_iterator.index; }

			/// 1231223
			/// Get the EndHack from a DataArrayT
			/// @param DataArrayT &data - The Data Array to do the End Hack
			/// @returns - An iterator based off the end Hack
			static const Iterator GetEndHack(DataArrayT &data) {
				auto hack = Iterator(data);
				hack.m_iterator.SetEndHack();
				return hack;
			}

			bool operator !=(const Iterator &other) const {
				return m_iterator != other.m_iterator;
			}

			Iterator &operator ++() {
				Next();
				return *this;
			}

			DataArrayIteratorResult<DatumT> operator *() const {
				return DataArrayIteratorResult<DatumT>(m_iterator.index, m_current_instance);
			}
		};

		Iterator &IteratorNew(Iterator &iter) {
			auto item = Iterator::GetEndHack(*this);

			return iter = Iterator(*this);
		}

		DatumT *Datums() {
			return reinterpret_cast<DatumT *>(this->Header.base_address);
		}

		operator s_data_array *() {
			return &this->Header;
		}

		DatumT *operator [](datum_index handle) {
			return &reinterpret_cast<DatumT *>(this->Header.base_address)[handle.index];
		}

		Iterator begin() {
			auto iter = Iterator(*this);
			iter.Next();
			return iter;
		}

		const Iterator end() {
			return Iterator::GetEndHack(*this);
		}

		void Dispose() { blam::data_dispose(&Header); }

		void MakeValid() { blam::data_make_valid(&Header); }

		void MakeInvalid() { blam::data_make_invalid(&Header); }

		void DeleteAll() { blam::data_delete_all(&Header); }

		datum_index NextIndex(datum_index cursor) { return blam::data_next_index(&Header, cursor); }

		datum_index NewAtIndex(datum_index index) { return blam::datum_new_at_index(&Header, index); }

		datum_index New() { return blam::datum_new(&Header); }

		void Delete(datum_index index) { return blam::datum_delete(&Header, index); }
	};
};
