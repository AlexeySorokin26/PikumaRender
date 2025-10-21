#include <vector>

// Bubble
template<typename T>
void BubleSort(std::vector<T>& data) {
	for (int i = 0; i < data.size() - 1; ++i) {
		for (int j = 0; j < data.size() - i - 1; ++j) {
			if (data[j] > data[j + 1]) {
				MySwap(std::move(data[j]), std::move(data[j + 1]));
			}
		}
	}
}

// Quick

// Merge

// Radix

// Swap
// TODO universal refs 
template<typename T>
void MySwap(T&& l, T&& r) {
	T tmp = std::move(l);
	l = std::move(r);
	r = std::move(tmp);
}

// Харьянов nice analogy with army and прапорщик
// https://rutube.ru/video/1686237108e527d42ee91b59f7a42b14/?playlist=296938
// Insert 
template<typename T>
void InsertSort(std::vector<T>& data) {
	for (int i = 1; i < data.size(); ++i) {
		for (int j = i; j > 0 && data[j] < data[j - 1]; --j) {
			MySwap(std::move(data[j]), std::move(data[j - 1]));
		}
	}
}