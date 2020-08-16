#include "pch.h"
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
	os << "{";
	bool first = true;
	for (const auto& kv : m) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
	if (t != u) {
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw runtime_error(os.str());
	}
}

void Assert(bool b, const string& hint) {
	AssertEqual(b, true, hint);
}

class TestRunner {
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& test_name) {
		try {
			func();
			cerr << test_name << " OK" << endl;
		}
		catch (exception& e) {
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		}
		catch (...) {
			++fail_count;
			cerr << "Unknown exception caught" << endl;
		}
	}

	~TestRunner() {
		if (fail_count > 0) {
			cerr << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}

private:
	int fail_count = 0;
};

class Person {
public:
	// Вы можете вставлять сюда различные реализации класса,
	// чтобы проверить, что ваши тесты пропускают корректный код
	// и ловят некорректный
	void ChangeFirstName(int year, const string& first_name) {
	}
	void ChangeLastName(int year, const string& last_name) {
	}
	string GetFullName(int year) {
	}
};

void incognito( Person& man) {
	AssertEqual(man.GetFullName(1700), "Incognito", "Incognito test 1");

	man.ChangeFirstName(1766, "Taehyung");
	AssertEqual(man.GetFullName(1700), "Incognito", "Incognito test 2");

	man.ChangeLastName(1733, "Kim");
	AssertEqual(man.GetFullName(1700), "Incognito", "Incognito test 3");
}

void unknown_first_name(Person& man) {
	AssertEqual(man.GetFullName(1740), "Kim with unknown first name", "unknown first name 1");

	man.ChangeFirstName(1740, "Taehyung");
	AssertEqual(man.GetFullName(1735), "Kim with unknown first name", "unknown first name 2");

	man.ChangeLastName(1730, "Jeon");
	AssertEqual(man.GetFullName(1731), "Jeon with unknown first name", "unknown first name 3");
}

void unknown_last_name(Person& woman) {
	woman.ChangeFirstName(1998, "Effy");
	AssertEqual(woman.GetFullName(2000), "Effy with unknown last name", "unknown_last_name 1");

	woman.ChangeFirstName(1978, "Alena");
	AssertEqual(woman.GetFullName(1980), "Alena with unknown last name", "unknown_last_name 2");

	woman.ChangeFirstName(2005, "Margo");
	AssertEqual(woman.GetFullName(2007), "Margo with unknown last name", "unknown_last_name 3");
}

void surname_throwing(Person &woman) {
	woman.ChangeLastName(2000, "Kim");
	AssertEqual(woman.GetFullName(2001), "Effy Kim", "surname_throwing 1");
	l

	woman.ChangeLastName(1980, "Stonem");
	AssertEqual(woman.GetFullName(1985), "Alena Stonem", "surname_throwing 2");
	AssertEqual(woman.GetFullName(1999), "Effy Stonem", "surname_throwing 3");

	woman.ChangeLastName(1986, "Min");
	AssertEqual(woman.GetFullName(1990), "Alena Min", "surname_throwing 4");
	AssertEqual(woman.GetFullName(2009), "Margo Kim", "surname_throwing 5");
}

void name_throwing(Person& child) {
	child.ChangeLastName(1700, "Kim");

	child.ChangeFirstName(1799, "Kira");
	AssertEqual(child.GetFullName(2000), "Kira Kim", "name_throwing 1");

	child.ChangeFirstName(1739, "Mila");
	AssertEqual(child.GetFullName(1750), "Mila Kim", "name_throwing 2");

	child.ChangeFirstName(1800, "Arina");
	AssertEqual(child.GetFullName(1801), "Arina Kim", "name_throwing 3");
}

void AllTest(TestRunner& test) {

	Person man;
	incognito(man);
	unknown_first_name(man);

	Person woman;
	unknown_last_name(woman);
	surname_throwing(woman);

	Person child;
	name_throwing(child);
}

int main() {
	TestRunner runner;
	AllTest(runner);
	return 0;
}