# Interpolant-Generator
## introdution to interpolation
Given two logic formula A and B, if A ^ B = false, then there must exist an interpolant I for A and B such that:
1. A => I (A imply I)
2. I ^ B = false 
3. I refers only to common variable of A and B
![](https://i.imgur.com/fGx00Ac.png)
Reference: Interpolation and SAT-based Model Checking, K. L. McMillan

## Algorithm Description
```
// this is pseudo code
itp(c) {
    if c is a root, then
        if c ∈ A then
            itp(c) = the disjunciton of the global literals in c
        else itp(c) = constant True
    else, let c1, c2 be the predecessors of c and let v be their pivot variable 
        if v is lcoal to A
            then itp(c) = itp(c1) ∨ itp(c2)
        else itp(c) = itp(c1) ∧ itp(c2)
}
```

## Usage 
```
make
cd bin
./itp ../input/<input file> ../output/<output file>
```

## Input format
```
// resolution proof
3 2 2   // 3 is total variable num, 2 is clause num of A, 2 is clause num of A
1 1 0 0   // (x1)
2 -1 2 0 0   // (-x1+x2)
3 -2 3 0 0   // (-x2+x3)
4 -3 0 0      // (-x3)
5 2 0 1 2 0    // (x1)(-x1+x2) => (x2)
6 3 0 5 3 0    // (x2)(-x2+x3) => (x3)
7 0 6 4 0    // (x3)(-x3) => ( ) => UNSAT
```

## output format
```
1 false 0 0
2 OR 2 0 0
3 true 0 0
4 true 0 0
5 OR 0 1 2 0
6 AND 0 5 3 0
7 AND 0 6 4 0
```
![](https://i.imgur.com/iX4OLUj.png)

## Others
- Most Satsolver doesn't support resolution proof. Obtain resolution proof is not that easy.
- There exists better ways to generate interpolant.
- The interpolant generated from the algoithm is highly redundant, which requires futher processing. 

