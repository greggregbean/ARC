import matplotlib.pyplot as plt

import subprocess

c = 100 # capacity 
n = 100 # num of elements
r = 100 # range of elements

while (c <= 10000):
    while (n <= 10000):        
        while (r <= 10000):
            file = "test" + "_" + str (c) + "_" + str (n) + "_" + str (r)

            subprocess.run(["python3", "create_input.py", file, str (c), str (n), str (r)])
            print (file)

            r *= 10
        
        n *= 10
        r = 100
    
    n = 100
    c *= 10