#include <vector>

template <class T, size_t InitN, size_t MaxN, class Alloc = std::allocator<T> >
class DynamicSizeVector : public std::vector<T, Alloc> {
public:
	DynamicSizeVector() {
		this->reserve(InitN);
	}

	T& operator[](size_t n) {
		checkCapacity(n);
		return std::vector<T, Alloc>::operator[](n);
	}

    const T& operator[](size_t n) const {
		checkCapacity(n);
		return std::vector<T, Alloc>::operator[](n);
	}

private:
	void checkCapacity(size_t n) {
		size_t capacity = this->capacity();
		if (n >= capacity && capacity < MaxN) {
			while (n >= capacity && capacity < MaxN) {
				if (capacity < InitN) {
					capacity = InitN;
				} else {
					capacity <<= 1;
					if (capacity > MaxN) {
						capacity = MaxN;
					}
				}
			}
			this->reserve(capacity);
		}
	}
};

int main(int argc, char* argv[])
{
    DynamicSizeVector<DynamicSizeVector<int, 2, 4>, 3, 4> v;
	// memset(&v, 0, sizeof(v));
    v[1][100] = 43;
    printf("%d, %d %d\n", v.capacity(), v[1].capacity(), v[1][100]);

    // size_t n = atoi(argv[1]);
    // size_t i = 0;
    // while (n > 0) {
    //     n = (n >> 1);
    //     i++;
    // }
    // n = (1 << i);
    // printf("new_size %lu\n", n);

	std::vector<uint8_t> vec;
	vec.resize(11);
	char s[] = "helloworld";
	memcpy(vec.data(), s, 10);
	printf("vec size %d, content: %s\n", vec.size(), vec.data());
	vec.resize(5);
	printf("vec size %d, content: %s\n", vec.size(), vec.data());
    return 0;
}
