# Feature-Selection-as-Reinforcement-Learning


Dataset often contains many features that are either redundant or irrelevant, and can thus be removed without incurring much loss of information. Decreasing the number of feature have the advantage of reducing overfitting, simplifying models, and also involve shorter training time, which makes it a key aspect in machine learning. This repository contains the source code to perform feature selection by reinforcement learning, with a Monte carlo search tree. this algorithm is based on the previous publication (https://hal.inria.fr/inria-00484049/document).



## Running the code

#### Requirements
Compiling require the `boost` library (available at https://www.boost.org/) and a `c++14` compiler.
To compile the code, you can either open the project file `src/Feature_Selection.cbp` in Code::Blocks, or run the `src/Makefile` with a command prompt if you are using Make.

#### Datasets
The feature selection algorithm is run on the training set `src/dataset.dat`. It is a matrix L[n][f+1] where n is the number of training example and f the number of features, the last colomun in the file correspond to the labels. Any modification related to the training set can be implemented in `src/dataset.cpp`   


#### Simulation parameters
The main simulation parameters can be changed in `src/Main.cbp`.

```c++
    Nt = 100000;   //Number of iterations of the simulation
    MA = 0;        //Many-Armed behavior, put 0 for Discrete and 1 for Continuous
    q = 0.98;      //Random expansion parameter, used to control the average depth in the random phase, |q|<1
    k = 5;         //Number of nearest neighbors involved in the reward function calculation
    r = 6;         //Ratio of aggressive subsample size / Training set size
    ce = 0.5;      //UCB exploration control parameter (used in both discrete and continuous heuristic)
    c = 20;        //Continuous heuristic exploration parameter (RAVE score weight)
    cl = 200;      //l-RAVE/g-RAVE weight
    b = 0.2;       //Discrete heuristic exploration parameter, |b|<1
    
    L[n][f+1] = read_dataset("dataset.dat");  //Training set matrix
```
For details about the parameters, please refer to the implementation details described in (https://hal.inria.fr/inria-00484049/document).



#### Complexity
The computation time of the algorithm scale with O(n^2\*f/r) dur to the k nearest neighboor search involved in the reward calculation.


## Results

As a proof of concept, the algorithm is run on a linear dataset with 3 good features, 5 redundant features, and 7 random features.
The simulation qhould converge to a feature subset of size 3.
when the simulation is finished, the program return :

- The best feature subset 
- The computed g-RAVE score of all the features
- The reward and depth search after each iteration
- The computation time

All the informations are available in output files `Output_Tree.txt`, `Output_Reward.txt` and `Result.txt`.

#### interpretation of the result

- The best not is considered to be the most visited path at the end of the search:

`
Most visited path:
 [ 2-92182  fs-87739 ]`
 
 This refer to the feature selected, and the number of time this feature has been selected.

```c++
Node T[29] with feature subset:
 00100100000000000000000000
 Feature subset size = 1
 Node Average = 0.8221
 Node Variance = 0.0735
 Node Final = 87739
 T_F = 92182

Child Nodes:
feature     t_f       mu_f     sg_f      lRAVE_f    tl_f
   408      000417    0.7678   0.0878    0.7675     420
   568      001166    0.7788   0.0886    0.7779     1188
   699      000627    0.7827   0.0866    0.7821     635
   707      000504    0.7732   0.0904    0.7729     506
   766      001325    0.7858   0.0873    0.7837     1377
   770      001700    0.7694   0.0914    0.7694     1700
   893      000551    0.7784   0.0894    0.7748     649

   fs       087739    0.8242   0.0720    0.8221     92182

   fr       000031    0.7222   0.0729

============================================================================================
```


The fact that the stopping feature has been selected a the stopping feature *fs* high number of times indicate that the algorithm might have converged.
plot graph


#### Feature selection on benchmark dataset and comparison with CFS
