import random
import sys

input_file_name = './input_files/' + str (sys.argv[1])

capacity          = int (sys.argv[2])
num_of_elements   = int (sys.argv[3])
range_of_elements = int (sys.argv[4]) 

input_file = open (input_file_name, 'w')

input = str (capacity) + ' ' + str (num_of_elements) + '\n'

for i in range (num_of_elements):
    input += str (random.randint(0, range_of_elements)) + ' '

input_file.write (input)

