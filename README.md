# APRIL

Code for **APRIL: Approximating Polygons as Raster Intervals Lists**

Please read the instructions in their entirety before running tests.


## Ιncludes
- simple and 2-grid rasterization algorithms with decisioning
- option for compression/no-compression
- option for joining sets of different granularity (implemented only for 2 datasets: T3NA, O6_Oceania)
- option for # of partitions
- option for pipeline timing, where times are an average of 10 executions
- option for within join OR selection query (default is intersection join)
- option for polygon-linestring intersection join

## Dependencies
- g++/gcc 
- Boost library
- OpenMP


## DATASETS
### Download

In this Google Drive link (https://drive.google.com/drive/folders/1AMOy2q9NGFnJ1eSXXJU82enFmTYXxryV?usp=share_link) there are 2 text files that describe the binary format we used for our binary input datasets. You can also find the modified binary TIGER datasets that we used for our experiments there. Each data set is accompanied by a binary offset map, which is necessary to reduce the geometry retrieval cost from disk during the refinement phase. If the user wishes to use custom datasets, they have to create both the binary geometry files and their map using the above formats. Then, they have to edit the `dataset_data.cpp` file in order for the program to locate the new files.

*T1NA*, *T2NA* & *T3NA* contain 123K, 2.2M and 3K **polygons**, respectively. *T8NA* contains 16.9M **linestrings** and **QUERIES** contains 1000 randomly selected query polygons from the T3NA dataset.

Due to space limitations, we can not provide the modified Open Street Map (OSM) datasets we used for our paper. 

You can find both the original TIGER & OSM datasets here: http://spatialhadoop.cs.umn.edu/datasets.html

### Instructions

Before running, you must download the data files you wish to use along with their offset map files and place them in the `data` directory. The `data` directory has to be in the same directory as the README file (the main folder). If the `data` directory does not exist, create it.

## Compile & Run

Tested on Linux and Mac.

***To compile:*** 

First, use `make` inside the `libvbyte-master/` directory, to create the code needed for the compression (libvbyte).
Then use `make` from within the source code's directory (same as README file) to create the executable (`sj`). 

***To run:***

`./sj -p 1000 <arguments> <dataset R> <dataset S>`

## ARGUMENTS

#### for MBR_join (Two layer filter) & refinement:
- `-p X` : X partitions (MBR-join)
	- mandatory argument, use X = 1000 (the same we used for our experiments)
- `-q` : add refinement stage
	- optional argument, needed for accurate results

#### for APRIL
- `-c` : create APRIL
	- can be skipped if APRIL already exists for both datasets on disk (in the `APRIL/interval_data/` directory)	
- `-f` : use the APRIL intermediate filter
	- can be skipped to avoid using an intermediate filter
- `-n X`: use X partitions
	- optional argument (default 1 partition, polygon-polygon only)
- `-z` : use compression
	- optional argument (default no, polygon-polygon only)
- `-e` : experimental run for time measurements
	- optional argument (performs 10 complete iterations of the pipeline and prints the average time breakdown)
- `-l` : second dataset is linestrings
	- indicates that the **SECOND INPUT DATASET (S)** represents linestrings and not polygons
- `-d X` : use X order for dataset S
	- optional argument (sets granularity order to X for all of **S**, which must be either **T3NA** or **O6_Oceania**; not implemented for others, polygon-polygon only)
- `-s` : selection query
	- use for selection query. One of the R or S MUST BE inputed as 'Q', and the program will locate the queries file on disk. It is handled as an intersection join between a set and a query file (polygon-polygon or polygon-rectangle only).
- `-w` : within join
	- use to detect pairs of polygons r,s from R,S that fulfill the following predicate: "r within s" (polygon-polygon only).

## EXAMPLES
**1. Create APRIL (-c) for polygon datasets T1NA T3NA and perform a spatial intersection query on them, with an MBR-join, intermediate filter (-f) and refinement stage (-q)**

`./sj -p 1000 -c -f -q T1NA T3NA`

**2. Perform a spatial intersection query on T1NA and T3NA, with an MBR-join, intermediate filter and refinement stage. Their compressed (-z) APRIL approximations must already exist on disk. Run the pipeline 10 times (-e) to get the average runtime.**

`./sj -p 1000 -f -q -e -z T1NA T3NA`

## TESTS

### Script permissions and execution

There are 4 testing scripts to re-create some of the paper's experiments. We couldn't cover all of the paper's experiments in the tests, since we were unable to provide all of the datasets we used (due to space restrictions). In order to be able to run the scripts, you need execute permissions on them.

To give permission on Linux or Mac, run the `chmod 755 test1.sh` command (do this for all of the .sh files).

To run the tests, open a terminal in the same directory as the scripts and type `./test1.sh` (select the test you want).

All scripts write their output to their respective file in the `script-output/` directory.

### Script info

For each test, we mention the tables in the paper which contain that experiment's results.

1. test1.sh - **T1NA & T3NA** must be in the `data/` directory - performs the following 3 tests:
	1. creates the **uncompressed** APRIL approximation for both T1NA & T3NA (Table 9) and runs 10 iterations of the **polygon-polygon** spatial **intersection join** query (Table 5), prints the average time.
	2. same as 1. but with **compressed** APRIL (Table 5).
	3. performs 10 iterations of the **polygon-polygon** spatial **within join** query *T1NA within T3NA* (Table 7) and prints the average time.
2. test2.sh - **T1NA & T2NA** must be in the `data/` directory - performs the following 4 tests:
	1. creates the **uncompressed** APRIL approximation for both T1NA & T2NA (Table 9) and runs 10 iterations of the **polygon-polygon** spatial **intersection join** query (Table 5), prints the average time.
	2. same as 1. but with **compressed** APRIL (Table 5).
	3. performs 10 iterations of the **polygon-polygon** spatial **within join** query (Table 7) *T2NA within T1NA* and prints the average time.
	4. creates the **uncompressed** APRIL approximation for both T1NA & T2NA in **4 partitions** and runs 10 iterations of the **polygon-polygon** spatial **intersection join** query (Table 13), prints the average time.
3. test3.sh - **T1NA, T2NA & QUERIES** must be in the `data/` directory - performs the following 2 tests:
	1. creates the **uncompressed** APRIL approximation for both T1NA & the 1000 polygonal range queries and runs 10 iterations of the **range query** (Table 6), prints the average time.
	2. same as 1. but on **T2NA** (Table 6).
4. test4.sh - **T1NA, T2NA, T3NA & T8NA** must be in the `data/` directory - performs the following 3 tests:
	1. creates the **uncompressed** APRIL approximation for both T1NA & T8NA and runs 10 iterations of the **polygon-linestring** spatial **intersection join** query (table 8), prints the average time.
	2. same as 1. but with **T2NA** instead of T1NA (Table 8).
	2. same as 1. but with **T3NA** instead of T1NA (Table 8).

### General testing info

You can create the APRIL approximation of each dataset only once and then use it for any number of queries you need. As mentioned before, the `-c` command line argument directs the program to create the APRIL approximation for the 2 input datasets. You can create new tests (or modify the existing) by removing the `-c` argument, given that you already have created the APRIL binary files. However, keep in mind that the compressed and uncompressed APRILs are used differently by the intermediate filter, so in order to switch from compressed to uncompressed and back, you need to have both of them already created on disk (`-z` for the compressed argument). The same goes for the number of partitions used to create APRIL. For example, the file `T1NA_binary_interval_A_2.dat` (located in `APRIL/interval_data/`) indicates that this binary file contains the **A-lists** of the objects of **T1NA**, rasterized using *2* partitions, so it has to be joined with APRILs that were created using the same number of partitions as well.

There is no option to universally create all APRIL approximations (compressed, uncompressed, X partitions etc.) for all datasets and then run the preferred queries. 

## Libraries

For the MBR-join, we used the *Two-Layer partitioning* code of D. Tsitsigkos and P. Bouros (https://ieeexplore.ieee.org/document/9458667)

For the compression, we used the *libvbyte* library (https://github.com/cruppstahl/libvbyte)

For the flood filling, we drew from this *Scanline Floodfill Algorithm With Stack* implementation (https://lodev.org/cgtutor/floodfill.html#Scanline_Floodfill_Algorithm_With_Stack)

For the polygon triangulation needed for the OpenGL experiments (not included, tested in controlled environment), we used the *clip2tri* library (https://github.com/raptor/clip2tri)


## Copyright

Full