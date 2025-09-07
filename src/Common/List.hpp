#pragma once

namespace Qybercom {
	template<typename T>
	class List {
		private:
			struct Node {
				T Data;
				Node* Prev;
				Node* Next;
				int Index;

				Node (const T &data) : Data(data), Prev(0), Next(0), Index(-1) {}
			};

			class Iterator {
				private:
					Node* _node;

				public:
					Iterator (Node* node) : _node(node) {}

					T &operator* () { return _node->Data; }

					Iterator &operator++ () {
						_node = _node->Next;

						return *this;
					}

					bool operator!= (const Iterator &other) const {
						return _node != other._node;
					}
			};

			Node* _head;
			Node* _tail;
			int _count;

			Node* _iNode;
			bool _iStarted;

			void _reindex () {
				Node* cur = _head;
				int idx = 0;

				while (cur) {
					cur->Index = idx++;
					cur = cur->Next;
				}
			}

		public:
			List () : _head(0), _tail(0), _count(0), _iNode(0), _iStarted(false) {}

			Node* Head () { return _head; }
			T &First () { return _head->Data; }
			Node* Tail () { return _tail; }
			T &Last () { return _tail->Data; }

			int Count () const { return _count; }

			Node* INode () { return _iNode; }

			List<T> &Add (const T &data, bool before = false) {
				Node* node = new Node(data);

				if (!_head) _head = _tail = node;
				else if (before) {
					node->Next = _head;
					_head->Prev = node;
					_head = node;
				}
				else {
					node->Prev = _tail;
					_tail->Next = node;
					_tail = node;
				}

				_count++;
				_reindex();

				return *this;
			}

			List<T> &Insert (const T &data, int index) {
				if (index <= 0) return Add(data, true);
				if (index >= _count) return Add(data, false);

				Node* cur = _head;
				while (cur && cur->Index != index) cur = cur->Next;

				if (cur) {
					Node* node = new Node(data);
					node->Prev = cur->Prev;
					node->Next = cur;

					if (cur->Prev)
						cur->Prev->Next = node;

					cur->Prev = node;

					if (cur == _head)
						_head = node;

					_count++;
					_reindex();
				}

				return *this;
			}

			List<T> &Remove (int index) {
				if (index < 0 || index >= _count) return *this;

				Node* cur = _head;
				while (cur && cur->Index != index) cur = cur->Next;

				if (cur) {
					if (cur->Prev) cur->Prev->Next = cur->Next;
					if (cur->Next) cur->Next->Prev = cur->Prev;
					if (cur == _head) _head = cur->Next;
					if (cur == _tail) _tail = cur->Prev;

					delete cur;
					_count--;
					_reindex();
				}

				return *this;
			}

			List<T> &Clear () {
				Node* cur = _head;

				while (cur) {
					Node* next = cur->Next;
					delete cur;
					cur = next;
				}

				_head = _tail = 0;
				_count = 0;
				_iNode = 0;
				_iStarted = false;

				return *this;
			}

			int IndexOf (const T &data) {
				Node* cur = _head;

				while (cur) {
					if (cur->Data == data)
						return cur->Index;

					cur = cur->Next;
				}

				return -1;
			}

			bool Exists (const T &data) {
				return IndexOf(data) != -1;
			}

			T &Get (int index) {
				Node* cur = _head;

				while (cur) {
					if (cur->Index == index)
						return cur->Data;

					cur = cur->Next;
				}

				if (_head)
					return _head->Data;

				return *(T*) 0;
			}

			Node* GetNode (unsigned int index) {
				if (index >= _count) return 0;

				Node* cur = _head;
				for (unsigned int i = 0; i < index; i++) cur = cur->Next;

				return cur;
			}

			T PopFirst () {
				if (!_head) return T();

				Node* node = _head;
				T data = node->Data;
				_head = _head->Next;

				if (_head) _head->Prev = nullptr;
				else _tail = nullptr;

				delete node;
				_count--;

				return data;
			}

			T PopLast () {
				if (!_tail) return T();

				Node* node = _tail;
				T data = node->Data;
				_tail = _tail->Prev;

				if (_tail) _tail->Next = nullptr;
				else _head = nullptr;

				delete node;
				_count--;

				return data;
			}

			T PopAt (int index) {
				if (index < 0 || index >= _count) return T();
				if (index == 0) return PopFirst();
				if (index == _count - 1) return PopLast();

				Node* node = _head;
				for (int i = 0; i < index; i++)
					node = node->Next;

				T data = node->Data;
				node->Prev->Next = node->Next;
				node->Next->Prev = node->Prev;

				delete node;
				_count--;

				return data;
			}

			List<T> &Reset () {
				_iNode = 0;
				_iStarted = false;

				return *this;
			}

			T &Current () {
				return _iNode->Data;
			}

			T* Next () {
				_iNode = _iNode ? _iNode->Next : _head;

				return _iNode ? &_iNode->Data : nullptr;
			}

			bool Iterate () {
				if (!_iStarted) {
					_iNode = _head;
					_iStarted = true;
				}
				else if (_iNode) {
					_iNode = _iNode->Next;
				}

				return _iNode != nullptr;
			}

			// `for (item : list) {...}`
			Iterator begin () { return Iterator(_head); }

			Iterator end () { return Iterator(0); }

			T& operator[](unsigned int index) {
				Node* node = GetNode(index);

				return node == nullptr ? (*(T*) 0) : node->Data;
			}

			const T& operator[](unsigned int index) const {
				Node* node = const_cast<List*>(this)->GetNode(index);

				return node == nullptr ? (*(T*) 0) : node->Data;
			}

			~List () { Clear(); }
	};
}