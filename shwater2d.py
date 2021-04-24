import subprocess
import pickle

size = [200, 400, 600, 800, 1000, 1200, 1400, 1600]
thread = list(range(1, 17))

res = {i: {j: -1 for j in size} for i in thread}

for i in thread:
    for j in size:
        res[i][j] = float(subprocess.run(['srun', '-n', '1', './shwater2d.out', str(i), str(j), str(j)], stdout=subprocess.PIPE).stdout.decode('utf-8'))
        print('finished: {}, {}, {}'.format(i, j, res[i][j]))

with open('./result.pkl', 'wb') as f:
    pickle.dump(res, f)