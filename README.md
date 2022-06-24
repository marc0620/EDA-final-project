# EDA-final-project

DIRECTORY:

DrawingOutput/ output visualization
bin/ executable binary
input/ input file
lib/ libraries
obj/ objects binary
output/ output file
src/ source C++ codes

the test cases are files in ./inputs

HOW TO COMPILE:

make

HOW TO RUN:

    ./bin/main input/<input file name> ./output/<output file name> ./DrawingOutput/<output file name>
    For example,
    under EDA-final-project
    ./bin/main input/input1.txt output/output1.txt ./DrawingOutput/output1.txt

HOW TO EVALUATE THE RESULT:

./evaluator input/<input file name> output/<output file name>
For example,
under EDA-final-project
./evaluator input/input1.txt output/output1.txt
HOW TO GENERATE PLOT OF PLACING RESULT

    python drawer.py ./DrawingOutput/<target outfile for drawing>
