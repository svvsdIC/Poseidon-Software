# first param is the old repository e.g. ~/poseidon/
echo $1
# second param is the new repository e.g. ~/Poseidon-Software/
echo $2

# copy the main sets of node_modules
echo "cp -r ${1}/node_modules  ${2}"
      cp -r ${1}/node_modules  ${2}
echo "cp -r ${1}/src/plugins/notifications/node_modules ${2}/src/plugins/notifications"
      cp -r ${1}/src/plugins/notifications/node_modules ${2}/src/plugins/notifications

# copy all the node modules needed for various levels of bower directories using python program
echo "python3 fix_new_dir_for_bower.py ${1}/src  ${2}/src"
      python3 fix_new_dir_for_bower.py ${1}/src  ${2}/src
