import sys
import os
import shutil

if (len(sys.argv) < 3):
    print("Usage: python newproj.py <template project name> <new project name>")
    exit(0)

template_project_name = sys.argv[1]
new_project_name = sys.argv[2]

print(template_project_name, "=>", new_project_name)

source_path = os.path.abspath(template_project_name)
target_path = os.path.abspath(new_project_name)

if os.path.exists(target_path):
    shutil.rmtree(target_path)
'''
if not os.path.exists(target_path):
    os.mkdir(target_path)
'''
shutil.copytree(source_path, target_path)

print('Done!')

os.chdir(target_path)

if os.path.exists("Release"):
    shutil.rmtree("Release")
if os.path.exists("Debug"):
    shutil.rmtree("Debug")

filename_vcproj_old = template_project_name + '.vcproj'
filename_vcproj_new = new_project_name + '.vcproj'

os.rename(filename_vcproj_old, filename_vcproj_new)

file_vcproj_r = open(filename_vcproj_new, "r")
file_vcproj_lines = file_vcproj_r.readlines()
file_vcproj_r.close()

file_vcproj_w = open(filename_vcproj_new, "w")
for line in file_vcproj_lines:
    if "GUID" in line:
        continue
    if template_project_name in line:
        line = line.replace(template_project_name, new_project_name)
    file_vcproj_w.write(line)
file_vcproj_w.close()

os.chdir("..")