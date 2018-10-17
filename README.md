# Feature-Selection-as-Reinforcement-Learning

<img align="right" src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/latt.png" width=400>

Dataset often contains many features that are either redundant or irrelevant, and can thus be removed without incurring much loss of information. Decreasing the number of feature have the advantage of reducing overfitting, simplifying models, and also involve shorter training time, which makes it a key aspect in machine learning. 

This repository contains the source code to perform feature selection with a reinforcement learning approach, where the feature set state space is represented by a Direct Acyclic Graph (DAG). This repository contains the C++ implementation of two Monte Carlo DAG Search algorithms. The first one, FUSE (https://hal.inria.fr/inria-00484049/document), starts from the empty feature subset and relies on UCT to identify the best feature subest in the DAG with a fixed budget, while the second one, BLI-MCDS (https://docdro.id/e09o21k) solve the Best Arm Identification problem at different stage in the DAG until a stopping condition is verified.


&nbsp;



## Running the code

#### Requirements
Both algorithm are available in the `src/` foler, compiling requires the `boost` library (available at https://www.boost.org/) and a `c++14` compiler. To compile the code, you can either open the project file `Feature_Selection.cbp` in Code::Blocks, or run the `Makefile` in a command prompt if you are using Make. 

#### Datasets
The dataset is implemented as a matrix `L[n][f+1]` where *n* is the number of training example and *f* the number of features, the last colomun in the matrix correspond to the labels. The folder contains different functions to read dataset files such as `read_dataset()`, and all the code related to the training set is implemented in `dataset.cpp`.


#### The feature set space and stopping feature
<img align="right" src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/FS.png" width=225>
We define a graph for which each node correspond to a feature set *F*, adding a feature to this feature set lead to a parent node, and removing a feature lead to a child node. The root of the graph is the empty feature subset. For each node, we also consider the stopping feature *fs*, which allows the search to stop at the current node instead of adding new features.



&nbsp;



## FUSE Algorithm details

<img align="right" src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/MCTS.png" width=200>


### UCT phase
for a node *F*, the selected child *f* node is the one maximizing its UCB Score:
<img src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/UCB.png" width=400>  
*TF* is the number of visit of node *F*, and due to the high branching factor of the tree, the exploration is limited to an *Allowed feature* set, which restrict the number of considered child nodes depending of *TF*. A new child node is added whenever int\[*TF*^*b*\] is incremented. 

To know which feature to add, we consider the one maximizing its g-RAVE score.
The g-RAVE score of feature *f* is defined as the average reward over all final node *F* containing *f*


### Random phase

When a node with *TF*=0 is reached, we evaluate the node by performing random exploration until the stopping feature *fs* is added. The probability of chosing  the stopping feature at depth *d* is set to 1-*q*^*d*.

### Reward Calculation

Once the stopping feature has been selected, the exploration stops and the reward is computed based on a k-Nearest-Neighboor (kNN) classifier. The advantage of kNN is that it requires no prerequisite training, and is not too computationally expensive. The complexity of the reward calculation is scaling as O(*n*^2\**f*/*r*) and is limiting the algorithm to dataset with less than 10000 examples.

### Backpropagation phase

For an optimized convergence time, all of the parents nodes are updated. A node at depth *d* has *d* parents, which imply that there is *d*! nodes to be updated. While this scales exponentially and can become very long for high depth, it is in practice not limiting the algorithm.


#### Simulation parameters
The main simulation parameters can be changed in `src/Main.cpp`.

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



&nbsp;

## Results

When the simulation is finished, the program return :

- The best feature subset (considered to be the most visited path at the end of the search)
- The computed g-RAVE score of all the features
- The reward and depth search after each iteration
- The computation time

All the informations are available in output files `Output_Tree.txt`, `Output_Reward.txt` and `Result.txt`.



&nbsp;

### Feature selection on a theoretical training set

As a proof of concept, the algorithm is run on a theoretical dataset with *f* = 500 features and *n* = 600 examples. The features are (*x*, *y*, *z*) + 5 redundant features + 492 random features, and the training set is generated using a linear binary classifier: (a*x* + b*y* + c*z* >? 0). To generate the theoretical set, one can call `L = linear_dataset(n,f)`. 
The simulation is run over 300000 iteration and should couverge to a feature subset of size 3.

#### Interpretation of the results

`
Most visited path:
 [ 4-280235  2-271382  5-208801  fs-118163 ]`
 
 These numbers correspond to the feature selected with the number of time it has been selected. In this case, the most visited path is an accurate feature subset, because it contains 3 relevant features and no redundant features.

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

The fact that the stopping feature *fs* has been selected a high number of times relatively to other features indicate that the algorithm might have converged. One can note that only 12 of the 500 features has been explored in this node, this has been implemented on purpose to strongly limit the exploration due the the high branching factor of the lattice. This can be changed by adjusting the parameter *b*. 

By running `plot_reward.py` (requires `Python 3`), we optain the following graph:

<img align="center" src="https://raw.githubusercontent.com/Aurelien-Pelissier/Feature-Selection-as-Reinforcement-Learning/master/img/theo.png" width=450>

For the first ~10000 iterations, the algorithm kept exploring deeper, but then started to select stopping features and enventually explored at lower depth until it finally converged to depth = 3. One can note that each time the algorithm explored at depth higher than 3, the reward significantly decreased.



&nbsp;

