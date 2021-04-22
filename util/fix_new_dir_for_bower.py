'''
move bower_components for to where the belong
in new directory sturcutre.

argv[1]  --  the full path to the 'src' directory for cockpit
argv[2]  --  the full path to the new direcotry 'src' directory
INCLUDING "/src"
'''
import sys
import os
from pprint import pprint
from subprocess import check_output

if not (len(sys.argv) == 2):
    inPath = '/home/pi/oldMyRov/src'
    outPath = '/home/pi/Poseidon/rover/Pi/src'

print('cwd: ', os.getcwd())
os.chdir(inPath)
print('cwd: ', os.getcwd())

print('paths:')
print('\tinPath:  ' + inPath)
print('\toutPath: ' + outPath)
print()

 
dirStr = check_output( ['find', inPath, '-name', 'bower_components',]
                      ).decode().strip()
dirList =  dirStr.split('\n')
subDirList = [ d[22:] for d in dirList] 

pprint(subDirList)
    



cmdList = []
for d in subDirList:
    cmdList.append(['cp', '-r', inPath + "/" + d,  outPath + '/' + d])

for cmd in cmdList:

#     cmd[0] = 'echo'
#     cmd[1] = ''
     print(check_output(cmd).decode())
      
