

int f(int t[4][2])
{
	return t[3][0] + t[1][1];
}


int g(int x, int y, int z)
{
	return x * y;
}

int main() {
	int t[4][2];

	int res = f(t);

	int r, s;
	r = s = 3 * (6 - 2) / (33 + 3);

	res = g(1, 2, 3);
	write(res);

	return 0;
}
