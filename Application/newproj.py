import sys
import os
import shutil
import time

new_project_name = input("New Proect Name:")
template_project_name = "template"

print(template_project_name, "=>", new_project_name)

source_path = os.path.abspath(template_project_name)
target_path = os.path.abspath(new_project_name)

if os.path.exists(target_path):
    #shutil.rmtree(target_path)
    print("New Project already exist")
    time.sleep(1)
    exit(0)

shutil.copytree(source_path, target_path)

os.chdir(target_path)

if os.path.exists("Release"):
    shutil.rmtree("Release")
if os.path.exists("Debug"):
    shutil.rmtree("Debug")
if os.path.exists("x64"):
    shutil.rmtree("x64")

##########################################################
filename_vcproj_old = template_project_name + '.vcxproj.filters'
filename_vcproj_new = new_project_name + '.vcxproj.filters'

os.rename(filename_vcproj_old, filename_vcproj_new)

##########################################################
filename_vcproj_old = template_project_name + '.vcxproj.user'
filename_vcproj_new = new_project_name + '.vcxproj.user'

os.rename(filename_vcproj_old, filename_vcproj_new)

##########################################################
filename_vcproj_old = template_project_name + '.vcxproj'
filename_vcproj_new = new_project_name + '.vcxproj'

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

##########################################################

file_r = open("usage.c", "r")
file_lines = file_r.readlines()
file_r.close()

file_w = open("usage.c", "w")
for line in file_lines:
    if template_project_name in line:
        line = line.replace(template_project_name, new_project_name)
    file_w.write(line)
file_w.close()
