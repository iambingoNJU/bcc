int f(int x, int y, int z)
{
	return x * y;
}

int main()
{
	int x;

	x = f(1, 2, 3);
	write(x);

	x = f(3, 2, 1);
	write(x);

	return 0;
}
