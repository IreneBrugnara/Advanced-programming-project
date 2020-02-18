# Advanced-programming-project

Authors: Irene Brugnara, Roberto Corti.

Folder ```bst``` contains all the header files for the binary search tree implementation:

* ```bst.hpp``` defines the ```bst``` class
* ```bst_bits.hpp``` implements the methods of ```bst ```
* ```node.hpp``` defines the ```Node``` class and its methods
* ```iterator.hpp``` defines the ```_iterator``` class and its methods.

File ```test.cpp``` checks if all ```bst``` methods are properly working. File ```benchmark.cpp``` performs a benchmark of our ```bst``` compared to ```std::map```; the result is presented in ```Report.md```.  
A ```Makefile ``` is used to compile automatically.  To obtain the executables ```test``` and ```benchmark```, simply type ```make```. 

To have the plot for the benchmark, we used ```gnuplot``` . After executing ```benchmark```, a file ```data_benchmark.txt``` is generated. To generate the plot of this data, the command ```gnuplot graph_benchmark.gp``` will produce the file ```plot_benchmark.png```.


