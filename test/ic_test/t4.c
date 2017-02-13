
int fun(int temp[7][5])
{
	return temp[0][3] + temp[1][2];
}

int main()
{
	int res;
	int arr[7][5];

	arr[0][3] = 44;
	arr[1][2] = -4;

	res = fun(arr);

	write(res);

	return 0;
}
