#include <cstdio>
int main()
{
	char c = ' ';
	int a[26] = {};
	while (c != '\n')
	{
		c = getchar();
		c > 96 ? a[c - 97]++ : a[c - 65]++;
	}
	int max = 0;
	for (int i = 0; i < 26; i++)
	{

	}
	return 0;
}