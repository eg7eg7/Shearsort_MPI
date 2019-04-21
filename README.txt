# ShearSort using MPICH, an implementation of MPI (Message Passing Interface)

Parallel Computation


Put the file pixel.txt in C:\ in order to run Initial.exe



You will have to run the .exe file with the same amount of processes indicated inside the text file and the magic will begin!



The format of the text file should look like this:
**************************************************
16
0  1000  146  0.1  0.13  0.9
1  450   55   0.3  0.12  0.61
2 -12    500  0.2   0.4    0.7
3  107  -203  0.9  0.1   0.45
4  10    446  0.1  0.13  0.9
5  -4   46   0.0  0.0   0.0
6  67    746  0.1  0.13  0.9
7  120   406  0.1  0.13  0.9
8  5     5    0.0  0.0   0.0
9  6     6    0.0  0.0   0.0
10 999   406  0.1  0.13  0.0
11 3    -56   0.0  0.0   0.0
12 -230 -36   0.1  0.0   0.5
13  1000 146  0.1  0.13  0.5
14  450  55   0.3  0.12  0.6
15 -12   600  0.6   0.4    0.4
*************************************************


The first number is the number of elements ; should be a square number.

each row is a Pixel element (id:int,x:int,y:int,rgb(float,float,float))

After reading the file the output of the program looks like the following:


********************************************
Before sorting:
Printing matrix..
Printing row 0

pixel id=   0	rgb = 0.10,0.13,0.90	x =   1000	y =    146
pixel id=   1	rgb = 0.30,0.12,0.61	x =    450	y =     55
pixel id=   2	rgb = 0.20,0.40,0.70	x =    -12	y =    500
pixel id=   3	rgb = 0.90,0.10,0.45	x =    107	y =   -203
Printing row 1

pixel id=   7	rgb = 0.10,0.13,0.90	x =    120	y =    406
pixel id=   6	rgb = 0.10,0.13,0.90	x =     67	y =    746
pixel id=   5	rgb = 0.00,0.00,0.00	x =     -4	y =     46
pixel id=   4	rgb = 0.10,0.13,0.90	x =     10	y =    446
Printing row 2

pixel id=   8	rgb = 0.00,0.00,0.00	x =      5	y =      5
pixel id=   9	rgb = 0.00,0.00,0.00	x =      6	y =      6
pixel id=  10	rgb = 0.10,0.13,0.00	x =    999	y =    406
pixel id=  11	rgb = 0.00,0.00,0.00	x =      3	y =    -56
Printing row 3

pixel id=  15	rgb = 0.60,0.40,0.40	x =    -12	y =    600
pixel id=  14	rgb = 0.30,0.12,0.60	x =    450	y =     55
pixel id=  13	rgb = 0.10,0.13,0.50	x =   1000	y =    146
pixel id=  12	rgb = 0.10,0.00,0.50	x =   -230	y =    -36
********************************************
********************************************
After sorting:
Printing matrix..
Printing row 0

pixel id=   3	rgb = 0.90,0.10,0.45	x =    107	y =   -203
pixel id=   0	rgb = 0.10,0.13,0.90	x =   1000	y =    146
pixel id=   6	rgb = 0.10,0.13,0.90	x =     67	y =    746
pixel id=   4	rgb = 0.10,0.13,0.90	x =     10	y =    446
Printing row 1

pixel id=   7	rgb = 0.10,0.13,0.90	x =    120	y =    406
pixel id=   2	rgb = 0.20,0.40,0.70	x =    -12	y =    500
pixel id=  15	rgb = 0.60,0.40,0.40	x =    -12	y =    600
pixel id=  12	rgb = 0.10,0.00,0.50	x =   -230	y =    -36
Printing row 2

pixel id=   1	rgb = 0.30,0.12,0.61	x =    450	y =     55
pixel id=  14	rgb = 0.30,0.12,0.60	x =    450	y =     55
pixel id=  13	rgb = 0.10,0.13,0.50	x =   1000	y =    146
pixel id=  10	rgb = 0.10,0.13,0.00	x =    999	y =    406
Printing row 3

pixel id=  11	rgb = 0.00,0.00,0.00	x =      3	y =    -56
pixel id=   5	rgb = 0.00,0.00,0.00	x =     -4	y =     46
pixel id=   9	rgb = 0.00,0.00,0.00	x =      6	y =      6
pixel id=   8	rgb = 0.00,0.00,0.00	x =      5	y =      5
********************************************
