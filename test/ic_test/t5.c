int f(int a, int b[4])
{
	return a + b[3];
}

int main()
{
	int x = 0;
	int res;
	int arr[4];

	arr[3] = 2;
	res = f(x, arr);

	return 0;
}
	
