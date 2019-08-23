#include <iostream>
#include <vector>
using namespace std;

//哈希表结点：结点信息，结点状态信息
enum State{EMPTY,EXIST,DELETE};

template <class key,class value>
struct HashNode {
	pair<key, value> _data;
	State _state;

	HashNode(const pair<key, value>& data = pair<key,value>()) 
		:_data(data),
		_state(EMPTY){}
};

template <class key,class value>
class HashTable {
public:
	typedef HashNode<key, value> Node;
	typedef Node* pNode;
	typedef pair<key, value> value_type;
	HashTable( size_t size = 10) {
		_ht.resize(size);
		for (size_t i = 0; i < _ht.size(); i++) {
			_ht[i]._state = EMPTY;
		}
		_size = 0;
	}

	//判断是否需要增容
	void checkCapacity() {
		//如果元素的数目已经与容量相等，或者大于等于容量的80%，则需要进行增容
		if (_size == _ht.size() || _size * 10 / _ht.size() >= 8) {
			//需要进行增容了：
			size_t newC = _ht.capacity() == 0 ? 10 : 2 * _ht.capacity();
			HashTable<key,value> NewT(newC);
			
			//进行数据的搬运
			for (size_t i = 0; i < _ht.size(); ++i) {
				size_t index = _ht[i]._data.first%newC;
				NewT.Insert(_ht[i]._data);
			}
			swap(_ht, NewT._ht);
		}
	}

	//插入结点：线性探测
	bool Insert(const value_type& data) {
		//先检查是否需要增容
		checkCapacity();

		//找到合适的位置去插入结点
		size_t index = data.first%_ht.size();
		//找到一个哈希值匹配的位置，判断该位置的状态信息
		while (_ht[index]._state != EMPTY) {
			//如果存在相同key值的结点，则插入失败
			if (_ht[index]._state == EXIST && _ht[index]._data.first == data.first) {
				return false;
			}
			//要么该状态信息是存在，要么是删除,则继续往后遍历，找到一个空的位置
			++index;
			while (_ht[index]._state != EMPTY && index<_ht.size()) {
				++index;
			}
			//如果找到最后都没有找到，就循环往表首部找
			if (index >= _ht.size()) {
				index = 0;
			}
		}
		//循环结束，一定能找到一个合适的位置来插入元素
		_ht[index]._data = data;
		_ht[index]._state = EXIST;
		++_size;
		return true;
	}

	//删除结点：假删除——将该位置的状态信息标为DELETE即可
	bool Delete(const key& k) {
		//先进行元素的查找
		size_t index = k % _ht.size();
		size_t start = index;
		while (_ht[index]._data.first != k) {
			++index;
			if (_ht[index]._data.first == k) {
				_ht[index]._state = DELETE;
				--_size;
				return true;
			}
			if (index == _ht.size()) {
				index = 0;
			}
			if (index == start) {
				break;
			}
		}
		if (_ht[index]._data.first == k) {
			_ht[index]._state = DELETE;
			--_size;
			return true;
		}

	}

	//查找某个结点
	pNode Find(const key& key) {
		size_t index = key % _ht.size();
		size_t start = index;
		while (_ht[index]._data.first != key) {
			++index;
			if (_ht[index]._data.first == key) {
				return &_ht[index];
			}
			if (index == _ht.size()) {
				index = 0;
			}
			if (index == start) {
				break;
			}
		}
		if (_ht[index]._data.first == key) {
			return &_ht[index];
		}
		return nullptr;
	}


private:
	vector<Node> _ht;
	size_t _size;
};

void test() {
	HashTable<int, int> ht;
	ht.Insert(make_pair(1, 1));
	ht.Insert(make_pair(2, 2));
	ht.Insert(make_pair(3, 3));
	ht.Insert(make_pair(4, 4));
	ht.Insert(make_pair(11, 11));

	//ht.Delete(11);

	ht.Find(3);
	HashNode<int, int>* pnode = ht.Find(11);
	cout << pnode->_data.first << endl;
}


int main() {
	test();
	system("pause");
	return 0;
}