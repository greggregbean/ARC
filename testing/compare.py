import os

proj_dir = "../"

if (os.path.isdir ("input_files") == False):
    os.mkdir ("input_files")

c = 100 # capacity 
n = 100 # num of elements
r = 100 # range of elements

while (c <= 100000):
    while (n <= 100000):        
        while (r <= 100000):
            file = proj_dir + "testing/input_files/test" + "_" + str (c) + "_" + str (n) + "_" + str (r)

            create_input = "python3 " + "create_input.py " + file + ' ' + str (c)+ ' ' + str (n) + ' ' + str (r)
            os.system (create_input)

            run = proj_dir + "/build/run < " + file
            os.system (run)

            run_belady = proj_dir + "/build/run < " + file
            os.system (run_belady)

            r *= 10
        n *= 10
        r = 100
    n = 100
    c *= 10