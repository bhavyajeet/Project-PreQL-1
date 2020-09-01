import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-n', type=int, required=True)
args = parser.parse_args()

with open('../data/bar.csv', 'w') as f:
    for i in range(args.n):
        for j in range(args.n):
            f.write(str(j) + ',')
        f.write('\n')


# import numpy as np
# x = np.random.randint(2147483647, size=(10000, 10000))
# np.savetxt("../data/foo.csv", np.array(x).astype(int), fmt='%i', delimiter=",")
