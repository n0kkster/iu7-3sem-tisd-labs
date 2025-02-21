from random import shuffle, choice, uniform

prefix = 'test_data'

def fill_matrix(percent, n, m):
	matrix = [[0] * m for _ in range(n)]
	need_to_fill = int(n * m * percent / 100)

	el_in_row = need_to_fill // n

	for i in range(n):
		for j in range(el_in_row):
			matrix[i][j] = round(choice((uniform(-1000, -1), uniform(1, 1000))), 6)
		shuffle(matrix[i])

	shuffle(matrix)

	return matrix

def write_data(filename, matrix, vector, n, m):
	with open(filename, 'w') as f:
		f.write(f'{n} {m}\n')

		for i in range(n):
			for j in range(m):
				f.write(str(matrix[i][j]))
				f.write(' ')
			f.write('\n')

		f.write(f'{m}\n')
		for x in vector[0]:
			f.write(str(x))
			f.write(' ')


for size in (100, 500, 1000):
	for percent in range(0, 101, 1):
		matrix = fill_matrix(percent, size, size)
		vector = fill_matrix(percent, 1, size)
		write_data(f'{prefix}/{size}.{percent}.txt', matrix, vector, size, size)
