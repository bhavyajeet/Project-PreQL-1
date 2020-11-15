import numpy
import random

num_col = 5
num_rows = 10000
string = "a,b,c,d,e\n"
for i in range(num_rows):
	for j in range(num_col):
		string += str(i) 
		if j != num_col - 1:
			string += ","
	string += "\n"

fil = open("../data/MYDATA.csv","w")
fil.write(string)
fil.close()
