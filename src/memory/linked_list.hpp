#pragma once

#include <precompile.h>
namespace Yelo
{
	// Use as a base class only
	template<typename T>
	class LinkedListNode
	{
		T* m_next;
		T* m_prev;

	protected:
		void ClearNodeData()
		{
			m_next = nullptr;
			m_prev = nullptr;
		}

	public:
		T* GetNext() const { return m_next; }
		T* GetPrevious() const { return m_prev; }

		void SetNext(T* next) { m_next = next; }
		void SetPrevious(T* previous) { m_prev = previous; }
	};

	template<typename TNode>
	class LinkedListIterator
	{
		TNode*& m_list_head;
		TNode* m_current_node;

	public:
		TNode* Current() { return m_current_node; }

		bool MoveNext()
		{
			if(m_current_node && !m_current_node->GetNext())
				return false;
			else if(m_current_node && m_current_node->GetNext())
				m_current_node = m_current_node->GetNext();
			else
				m_current_node = m_list_head;
			return m_current_node != nullptr;
		}

		bool MovePrevious()
		{
			if(!m_current_node)
				return false;

			if(m_current_node->GetPrevious())
			{
				m_current_node = m_current_node->GetPrevious();
				return true;
			}
			else
				return false;
		}

		void Reset() { m_current_node = nullptr; }

		LinkedListIterator(TNode*& list_head) : m_list_head(list_head), m_current_node(nullptr)
		{ }
	};

	// TNode's are LinkedListNode's

	template<typename TNode>
	size_t GetListLength(TNode*& list_head)
	{
		size_t count = 0;
		for(const TNode* cur = reinterpret_cast<TNode *>(list_head); cur != nullptr; cur = reinterpret_cast<TNode *>(cur->GetNext()), count++)
			;

		return count;
	}

	template<typename TNode>
	void DeleteLinkedList(TNode*& list_head)
	{
		YELO_ASSERT_DISPLAY(list_head != nullptr, "attepting to delete a linked list with a NULL list head");

		for(TNode* cur = reinterpret_cast<TNode *>(list_head->GetNext()); cur != nullptr; )
		{
			TNode* next = reinterpret_cast<TNode *>(cur->GetNext());
			delete cur;
			cur = next;
		}

		delete list_head;
		list_head = nullptr;
	}

	template<typename TNode>
	void AppendLinkedListNode(TNode*& list_head, TNode* object)
	{
		if(list_head == nullptr)
		{
			list_head = object;
			return;
		}

		TNode* cur, * next;
		for(cur = list_head, next = reinterpret_cast<TNode *>(cur->GetNext()); next != nullptr; cur = next, next = reinterpret_cast<TNode *>(cur->GetNext()))
			;

		cur->SetNext(object);
		object->SetPrevious(cur);
	}

	template<typename TNode>
	void RemoveLinkedListNode(TNode*& list_head, const TNode* object)
	{
		YELO_ASSERT_DISPLAY(list_head != nullptr, "attepting to remove from a linked list with a NULL list head");

		TNode* next = reinterpret_cast<TNode *>(object->GetNext());
		TNode* prev = reinterpret_cast<TNode *>(object->GetPrevious());

		if(list_head == object)
		{
			if(next != nullptr)
				next->SetPrevious(nullptr);
			list_head = next;
			return;
		}

		if(prev != nullptr) prev->SetNext(next);
		if(next != nullptr) next->SetPrevious(prev);
	}

	template<typename TNode>
	void InsertAfterLinkedListNode(TNode* previous, TNode* object)
	{
		TNode* next = previous->GetNext();

		previous->SetNext(object);
		object->SetPrevious(previous);

		object->SetNext(next);
		if(next != nullptr)
			next->SetPrevious(object);
	}

	template<typename TNode>
	TNode* GetNodeByIndex(TNode*& list_head, long index)
	{
		long i = 0;

		TNode* cur, * next;
		for(cur = list_head; cur != nullptr && (next = reinterpret_cast<TNode *>(cur->GetNext())) != nullptr && i < index; cur = next, i++)
			;

		if(i != index)
			return nullptr;

		return cur;
	}
};
