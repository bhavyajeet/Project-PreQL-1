# Project Phase 0

## Approach
### Storage of Matrix

To store a matrix of size *N* x *N*, we break the matrix into small matrices of size *k* x *k*, and store each matrix in a page.
Thus the number of pages/blocks required to store the matrix would be (*N*/*k*)x(*N*/*k*). For this use case, we have taken

**k = 32**

The matrix is stored in column-major representation, with each page being a *k*x*k* matrix.
We have taken the block size to be 8Kb, and block count to be 16.

### Number of data block accesses
- LOAD MATRIX : To load a matrix, we need to blockify it. This would need 

    **(N/32) x (N/32)**

    data block accesses.
- TRANSPOSE MATRIX : To transpose a matrix, we need to first, transpose all the inner contents of the matrix
    and then swap the rows of pages which are diagonally mirror of matrix.
    This would require a total of 

    **(N/32) x (N/32)** + **(N/32) x (N/32) - (N/32)**

    data block accesses.
- EXPORT MATRIX: To export a matrix,we need to print row by row.
     We need to first iterate over all blocks and print only that 
     rows which correspond to the row number we are printing.
    This would require a total of 

    **(N/32) x (N/32) * N**

    data block accesses.

## Implementation

### Matrix Access
Matrix is implemented using a separate `Matrix` object having a `MatrixCatalogue` of it's own.
The `bufferManager` stores the pages of matrix in it's deque. To access the matrix, the `bufferManager` gets the Page and returns 
a page of the Matrix asked for.

### Storage of Matrix
This is maintained as **MAX_ROWS_MATRIX** and **MAX_COLS_MATRIX** in the file **`server.cpp`**