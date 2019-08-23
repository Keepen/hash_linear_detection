#include <iostream>
#include <vector>
using namespace std;

//��ϣ���㣺�����Ϣ�����״̬��Ϣ
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

	//�ж��Ƿ���Ҫ����
	void checkCapacity() {
		//���Ԫ�ص���Ŀ�Ѿ���������ȣ����ߴ��ڵ���������80%������Ҫ��������
		if (_size == _ht.size() || _size * 10 / _ht.size() >= 8) {
			//��Ҫ���������ˣ�
			size_t newC = _ht.capacity() == 0 ? 10 : 2 * _ht.capacity();
			HashTable<key,value> NewT(newC);
			
			//�������ݵİ���
			for (size_t i = 0; i < _ht.size(); ++i) {
				size_t index = _ht[i]._data.first%newC;
				NewT.Insert(_ht[i]._data);
			}
			swap(_ht, NewT._ht);
		}
	}

	//�����㣺����̽��
	bool Insert(const value_type& data) {
		//�ȼ���Ƿ���Ҫ����
		checkCapacity();

		//�ҵ����ʵ�λ��ȥ������
		size_t index = data.first%_ht.size();
		//�ҵ�һ����ϣֵƥ���λ�ã��жϸ�λ�õ�״̬��Ϣ
		while (_ht[index]._state != EMPTY) {
			//���������ͬkeyֵ�Ľ�㣬�����ʧ��
			if (_ht[index]._state == EXIST && _ht[index]._data.first == data.first) {
				return false;
			}
			//Ҫô��״̬��Ϣ�Ǵ��ڣ�Ҫô��ɾ��,���������������ҵ�һ���յ�λ��
			++index;
			while (_ht[index]._state != EMPTY && index<_ht.size()) {
				++index;
			}
			//����ҵ����û���ҵ�����ѭ�������ײ���
			if (index >= _ht.size()) {
				index = 0;
			}
		}
		//ѭ��������һ�����ҵ�һ�����ʵ�λ��������Ԫ��
		_ht[index]._data = data;
		_ht[index]._state = EXIST;
		++_size;
		return true;
	}

	//ɾ����㣺��ɾ����������λ�õ�״̬��Ϣ��ΪDELETE����
	bool Delete(const key& k) {
		//�Ƚ���Ԫ�صĲ���
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

	//����ĳ�����
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