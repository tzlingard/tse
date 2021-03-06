# ENGS 50 Module 6 Testing Script F20
# Author Brandon Guzman


./query ./ # number of arg test 

./query ./ ../idxfiles/trueIndex # not valid crawler dir test

./query ./ file   # unreadable file test 

valgrind ./query ../pages ../trueIndex < error-queries   # prof's error tests

valgrind ./query ../pages ../trueIndex -q good-queries.txt myoutput  # run quietly test with professor's queries 



