import os
from tabulate import tabulate

proj_dir = "../"

if (os.path.isdir ("input_files") == False):
    os.mkdir ("input_files")

c = 100 # capacity 
n = 100 # num of elements
r = 100 # range of elements

table_data = []

while (c <= 100000):
    while (n <= 100000):        
        while (r <= 100000):
            file = proj_dir + "testing/input_files/test" + "_" + str (c) + "_" + str (n) + "_" + str (r)

            create_input = "python3 " + "create_input.py " + file + ' ' + str (c)+ ' ' + str (n) + ' ' + str (r)
            os.system (create_input)

            run_arc = proj_dir + "/build/run < " + file
            arc_output = os.popen(run_arc).read()

            run_belady = proj_dir + "/build/run_belady < " + file
            belady_output = os.popen(run_belady).read()
            
            if (float(arc_output) == float(belady_output)):
                result = "= (GOOD)"
            
            elif (float(arc_output) < float(belady_output)):
                result = "< (USUAL)"
            
            else:
                result = "> (VERY GOOD)"

            table_data.append ([str(c), str(n), str(r), arc_output, belady_output, result])

            r *= 10
        n *= 10
        r = 100
    n = 100
    c *= 10

col_names = ["Capacity", "Number of elements", "Range of elements", "ARC Num of hits", "Belady Num of hits", "Result"]

output_file = open ("result_table", 'w')
output_file.write (tabulate(table_data, headers=col_names, tablefmt="fancy_grid"))