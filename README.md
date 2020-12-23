# Project PreQL

## Usage

### Compilation Instructions

We use ```make``` to compile all the files and creste the server executable. ```make``` is used primarily in Linux systems, so those of you who want to use Windows will probably have to look up alternatives (I hear there are ways to install ```make``` on Windows). To compile

```cd``` into the SimpleRA directory
```
cd SimpleRA
```
```cd``` into the soure directory (called ```src```)
```
cd src
```
To compile
```
make clean
make
```

### To run

Post compilation, an executable names ```server``` will be created in the ```src``` directory
```
./server
```

## Database Commands
| Task                            	| Command  	| Syntax                                               	| Arguments                                                                                                                                                                   	|
|---------------------------------	|----------	|------------------------------------------------------	|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------	|
| **Assignment Statements**           	|          	|                                                      	|                                                                                                                                                                             	|
| Cartesian Cross Product         	| `CROSS`    	| `CROSS Relation1 Relation2`                            	| Relation 1: Name of Relation 1<br><br>Relation 2: Name of Relation 2                                                                                                        	|
| Distinct rows in a table        	| `DISTINCT` 	| `DISTINCT Relation`                                    	| Relation: Name of Relation                                                                                                                                                  	|
| Join 2 Relations on a condition 	| `JOIN`     	| `JOIN Relation1, Relation2 ON Col1 operator Col2`      	| Relation 1: Name of Relation 1<br>Col1: Column of Relation 1<br>Relation 2: Name of Relation 2<br>Col2: Column of Relation 2<br>operator: One of ==, <, <=, =<,  >, >=, =>  	|
| Project list from relation name 	| `PROJECT`  	| `PROJECT ProjectionList FROM Relation`                 	| Projection List: List of projections<br>Relation: Name of Relation                                                                                                          	|
| Select rows from relation       	| `SELECT`   	| `SELECT Col1 operator Col2/[IntLiteral] FROM Relation` 	| Col1: Column of Relation <br>operator: One of ==, <, <=, =<, >, >=, =><br>Col2: Another Column of Relation<br>IntLiteral: Integer Literal<br>Relation: Name of Relation     	|
| Sort relation by column name    	| `SORT`     	| `SORT Relation BY Col IN Order`                        	| Relation: Name of Relation<br>Col: Name of Column<br>Order: ASC, DESC                                                                                                       	|
| **Non-Assignment Statements**       	|          	|                                                      	|                                                                                                                                                                             	|
| Clear Relation                  	| `CLEAR`    	| `CLEAR Relation`                                       	| Relation: Name of Relation                                                                                                                                                  	|
| Index Relation on an Attribute  	| `INDEX`    	| `INDEX ON Col FROM Relation USING Strategy`            	| Col: Column of Relation<br>Relation: Name of Relation<br>Strategy: HASH (for Hash), <br>BTREE (for B+Tree), NOTHING (for nothing).                                          	|
| List relations                  	| `LIST`     	| `LIST TABLES`                                          	| -                                                                                                                                                                           	|
| Load relation                   	| `LOAD`     	| `LOAD Relation`                                        	| Relation: Name of Relation                                                                                                                                                  	|
| Print relation                  	| `PRINT`    	| `PRINT Relation`                                       	| Relation: Name of Relation                                                                                                                                                  	|
| Quit                            	| `QUIT`     	| `QUIT`                                                 	| -                                                                                                                                                                           	|
| Rename Column Name              	| `RENAME`   	| `RENAME Col1 TO Col2 FROM Relation`                    	| Relation: Name of Relation<br>Col1: One Column<br>Col2: Another Column                                                                                                      	|
| Export Relation into csv        	| `EXPORT`   	| `EXPORT Relation`                                      	| Relation: Name of Relation                                                                                                                                                  	|
| Source file                     	| `SOURCE`   	| `SOURCE Filename`                                      	| Filename: Name of files                                                                                                                                                     	|

## Acknowledgement
Additional Design implemented by Vikrant Dewangan, Bhavyajeet Singh.

Template code provided by [Jayitha C](https://github.com/Jayitha),  [here](https://github.com/SimpleRA/SimpleRA).
