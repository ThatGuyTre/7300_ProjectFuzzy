Dataset taken from:
https://www.kaggle.com/datasets/kazanova/sentiment140

## Implementing fuzzy search to search the dataset
We will be using multiple algorithm implementations for this fuzzy search.

Brute force algorithm is simply permuting and searching multiple times based on the search term given. This is expected to perform the worst.

Elegant solution is using Hamming Distance and Levenshtein Distances to intelligently spend resources on searches while parsing the dataset. These are expected to perform better.


An example of inverse indexing is this:

### Dataset:
```
Tweet 1: "apple orange banana"
Tweet 2: "banana apple apple"
Tweet 3: "grape banana orange"
```

### Example Inputs and Outputs:
```
{
  "apple": [1, 2, 2],   # "apple" appears once in Document 1 and twice in Document 2
  "orange": [1, 3],     # "orange" appears in Documents 1 and 3
  "banana": [1, 2, 3],  # "banana" appears in all three documents
  "grape": [3]          # "grape" appears only in Document 3
}
```
### Example FUZZY Inputs and Outputs:
### Dataset:
```
Tweet 1: "appel orange banana"
```

### Example Inputs and Outputs:
```
{
  "apple": [1],   # "appel" appears in Document 1 which is a fuzzy version of "apple"
}
```

The main goal of searching will be parsing all these data entries and accumulating inverse indices of the search term. The result of the searches will be an array of all tweets where the string appears. In terms of index size versus query efficiency, a longer query will result in a smaller index.

## Running the project
`fuzzy.c` includes all the fuzzy searching files.
`test.c` includes the testing functionality.

To run the program, first run the `make` command, which will make the `fuzzy_test` executable. Then you can run it using `./fuzzy_test`
