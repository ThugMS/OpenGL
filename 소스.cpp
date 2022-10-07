#include <vector>
#include <iostream>

using namespace std;

int main() {
	vector<string> v(20);

	v.push_back("12031203");

	cout << v[0] << endl;

	cout << v.size() << endl;

	cout << v.capacity() << endl;
}