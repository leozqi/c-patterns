#include <iostream>

char *concatenate(char *s_s1, char *s_s2);

char *concatenate(char *s_s1, char *s_s2)
{
	std::size_t len_s1{ 0 };
	std::size_t len_s2{ 0 };
	char *itr{ s_s1 };

	for (itr = s_s1; *itr != '\0'; ++itr) { ++len_s1; }
	for (itr = s_s2; *itr != '\0'; ++itr) { ++len_s2; }

	++len_s1;
	++len_s2;

	std::cout << "s_s1: " << len_s1 << " s_s2: " << len_s2 << std::endl;

	char *p_ret{ new char[len_s1 + len_s2]{} };

	for (itr = s_s1; *itr != '\0'; ++itr) {
		(*p_ret) = (*itr);
		++p_ret;
		std::cout << p_ret << std::endl;
		std::cout << itr << std::endl;
	}

	for (itr = s_s2; *itr != '\0'; ++itr) {
		(*p_ret) = (*itr);
		++p_ret;
	}

	return p_ret;
}

int main()
{
	char str1[6] = { 'a', 'b', 'c', 'd', 'e', '\0' };
	char str2[6] = { 'f', 'g', 'h', 'i', 'j', '\0' };

	char *result = concatenate(str1, str2);

	std::cout << result << std::endl;

	return 0;
}
