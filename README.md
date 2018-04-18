# Feature-Selection-as-Reinforcement-Learning

<img align="right" src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/latt.png" width=400>

Dataset often contains many features that are either redundant or irrelevant, and can thus be removed without incurring much loss of information. Decreasing the number of feature have the advantage of reducing overfitting, simplifying models, and also involve shorter training time, which makes it a key aspect in machine learning. This repository contains the source code to perform feature selection by reinforcement learning, based on  a Monte carlo lattice search. The algorithm is adapted from a publication presented at international machine learning conference in 2010 (https://hal.inria.fr/inria-00484049/document).


## Algorithm details

#### UCT phase

#### New node phase

#### Random phase

#### Backpropagation phase

#### The stopping feature




## Running the code

#### Requirements
To compile the code, you can either open the project file `src/Feature_Selection.cbp` in Code::Blocks, or run the `src/Makefile` in a command prompt if you are using Make. It requires the `boost` library (available at https://www.boost.org/) and a `c++14` compiler.

#### Datasets
The dataset is implemented as a matrix `L[n][f+1]` where *n* is the number of training example and *f* the number of features, the last colomun in the matrix correspond to the labels. The folder contain different functions to read dataset files such as `read_dataset()`, and all the code related to the training set is implemented in `src/dataset.cpp`.


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
The computation time of the algorithm scale with O(n^2\*f/r), it is dominated by the k nearest neighboor search involved in the reward calculation.


## Results

As a proof of concept, the algorithm is run on a theoretical dataset with *f* = 500 features and *n* = 600 examples. The features are (*x*, *y*, *z*) + 5 redundant features + 492 random features, and the training set is generated using a binary classifier: (a*x* + b*y* + c*z* >? 0). The simulation is run over 300000 iteration and should couverge to a feature subset of size 3. To generate the theoretical set, one can call `L = linear_dataset(n,f)`. When the simulation is finished, the program return :

- The best feature subset 
- The computed g-RAVE score of all the features
- The reward and depth search after each iteration
- The computation time

All the informations are available in output files `Output_Tree.txt`, `Output_Reward.txt` and `Result.txt`.

### Interpretation of the result

- The best not is considered to be the most visited path at the end of the search:

`
Most visited path:
 [ 4-280235  2-271382  5-208801  fs-118163 ]`
 
 These numbers correspond to the feature selected with the number of time it has been selected.

```c++
Node T[795] with feature subset:
 0010110000000000000000000...
 Features  2  4  5
 Feature subset size = 3
 Node Average = 0.9915
 Node Variance = 0.0149
 Node Final = 118163
 T_F = 208801

Child Nodes:
feature     t_f       mu_f     sg_f      lRAVE_f    tl_f
   076      014323    0.9812   0.0244    0.9817     23885
   126      033734    0.9840   0.0216    0.9837     35844
   171      022876    0.9800   0.0244    0.9807     24590
   226      013478    0.9795   0.0243    0.9794     13856
   270      009983    0.9778   0.0268    0.9799     18828
   280      014602    0.9798   0.0245    0.9800     15132
   290      008297    0.9737   0.0284    0.9741     9303
   298      013559    0.9848   0.0225    0.9827     14965
   358      022042    0.9844   0.0230    0.9844     22045
   447      025873    0.9857   0.0208    0.9847     30149
   482      015783    0.9789   0.0250    0.9794     16173
   486      016675    0.9879   0.0181    0.9852     18322

   fs       118163    0.9949   0.0100    0.9915     208801

   fr       000001    0.9962   0.0000

============================================================================================
```

The fact that the stopping feature *fs* has been selected a high number of times indicate that the algorithm might have converged. One can note that only 12 of the 500 features has been explored in this node, this has been implemented on purpose to strongly limit the exploration due the the high branching factor of the lattice. This can be changed by adjusting the parameter *b*. 
By running `plot_reward.py` (require `Python 3`), we optain the following graph.

<img align="center" src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/theo.png" width=450>


### Feature selection on benchmark dataset and comparison with CFS
