#pragma once

#include "../../memory/datum_index.h"
#include "../../memory/data_iterator.h"
#include "tag_groups.h"

namespace Yelo::TagGroups {
	class c_tag_iterator {
		s_tag_iterator m_state;
		datum_index    m_tag_index;

		c_tag_iterator(const void *endHackDummy);

	public:
		c_tag_iterator(const tag group_tag_filter);

		template <typename T>
		c_tag_iterator() : m_tag_index(datum_index::null()) {
			blam::tag_iterator_new<T>(m_state);
		}

		// Get an iterator that doesn't have any specific group_tag filter
		static c_tag_iterator all() {
			return c_tag_iterator(NONE);
		}

		datum_index Next();

		bool operator !=(const c_tag_iterator &other) const;

		c_tag_iterator &operator ++() {
			Next();
			return *this;
		}

		datum_index operator *() const {
			return m_tag_index;
		}

		c_tag_iterator &begin() {
			this->Next();
			return *this;
		}

		static const c_tag_iterator end() {
			return c_tag_iterator(nullptr);
		}
	};
};
