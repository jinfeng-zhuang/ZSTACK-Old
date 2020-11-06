import os
import sys

count = 0
len_max = 0

def findAllFile(base, supply, max_len):
    global  count;
    for root, ds, fs in os.walk(base):
        for f in fs:
            if f.endswith('.cpp') or f.endswith('.c'):
                fullname = os.path.join(root, f)
                fullname = fullname.replace("\\", "_")
                fullname = fullname.replace("/", "_")
                fullname = fullname.replace(".", "_")
                fullname = fullname.strip('_')
                fullname = fullname.upper();
                fullname = "#define " + fullname;
                if (supply):
                    fullname = fullname.ljust(max_len)
                    fullname = fullname + "\t(" + str(count) + ")"
                    count = count + 1
                yield fullname

def main():
    global  len_max;
    global  count;
    last_line = "#define C_FILE_INDEX_MAX "
    
    if (len(sys.argv) != 2):
        print("Usage: python generate_file_index.py <folder>")
        return

    base = sys.argv[1]
    for i in findAllFile(base, 0, 0):
        if (len(i) > len_max):
            len_max = len(i)

    count = 0;
    
    print("#ifndef ZSTACK_FILE_INDEX_H")
    print("#define ZSTACK_FILE_INDEX_H\n")
    for i in findAllFile(base, 1, len_max):
        print(i)
    last_line = base + "FILE_INDEX_MAX";
    last_line = last_line.replace("\\", "_")
    last_line = last_line.replace("/", "_")
    last_line = last_line.replace(".", "_")
    last_line = last_line.strip('_')
    last_line = last_line.upper();
    last_line = "#define " + last_line;
    last_line = last_line.ljust(len_max)
    last_line = last_line + "\t(" + str(count) + ")"
    print(last_line)
    print("\n#endif\n")

if __name__ == '__main__':
    main()
    
