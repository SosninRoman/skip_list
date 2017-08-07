#include "./tests/unit.h"
#include "slmap.h"

int main() {
	unit();

	typedef slmap<char, int> map;
	typedef slmultimap<char, int> multimap;

	map m;
	m['z'] = 1;
	m.erase('z');
	std::string a = "sweetchocolat";
	for (int i = 0; i < a.size(); i++) {
		m[a.at(i)]++;
	}
	for (map::iterator it = m.begin(); it != m.end(); it++) {
		std::cout << it->first << " -> " << it->second << std::endl;
	}

	std::cout << std::endl;

	multimap mm;
	for (int i = 0; i < a.size(); i++) {
		mm.insert(std::pair<char, int>(a.at(i), i));
	}
	for (multimap::iterator mit = mm.begin(); mit != mm.end(); mit++) {
		std::cout << mit->first << " -> " << mit->second << std::endl;
	}

	return 0;
}