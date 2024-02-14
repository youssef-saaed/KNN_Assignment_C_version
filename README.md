# KNN Assignment C version

This is a machine learning assignment that implements the K-Nearest Neighbors (KNN) algorithm on the Iris dataset. The Iris dataset contains 150 samples of three different species of iris flowers, with four features each: sepal length, sepal width, petal length, and petal width. The goal is to classify each sample into one of the three species based on the features.

## Dataframe.h

This is a header file that defines the data frame structure and functions. A data frame is a two-dimensional array of cells, where each cell can store different types of data, such as integers, doubles, or strings. The data frame also has an array of column names and a shape attribute that stores the number of rows and columns. The data frame functions include:

- `new_dataframe`: Creates a new data frame with the given number of rows and columns and returns a pointer to it.
- `read_csv`: Reads a CSV file and stores the data in a data frame and returns a pointer to it.
- `copy_dataframe`: Copies an existing data frame and returns a pointer to the new data frame.
- `copy_cell`: Copies an existing cell and returns a pointer to the new cell.
- `print_cell`: Prints the data of a cell to the standard output.
- `print_dataframe`: Prints the data of a data frame to the standard output.
- `delete_dataframe`: Deletes a data frame and frees the memory allocated for it.
- `mean`: Calculates the mean of a column in a data frame and returns it as a double.
- `stdev`: Calculates the standard deviation of a column in a data frame and returns it as a double.
- `standardize`: Standardizes a data frame by subtracting the mean and dividing by the standard deviation for the given columns and returns a pointer to the standardized data frame.

## KNN_Helpers.h

This is a header file that defines the K-Nearest Neighbors helper functions. The KNN algorithm is a supervised learning method that classifies a sample based on the majority vote of its k nearest neighbors in the feature space. The helper functions include:

- `split_samples`: Splits a data frame into training and testing data frames based on a given ratio and a label column and stores the pointers in the given parameters.
- `ecludian_dist`: Calculates the Euclidean distance between two points of the same length and returns the result as a double.
- `KNN_training_res`: Performs the KNN algorithm on the training and testing data frames and prints the results.

## main.c

This is the main file that executes the program. It does the following steps:

- Reads the Iris.csv file and stores the pointer to the data frame in `df`.
- Prints the data frame to the standard output.
- Declares and initializes an array of column names to be standardized: `stdcols`.
- Standardizes the data frame by subtracting the mean and dividing by the standard deviation for the given columns and stores the pointer to the standardized data frame in `standardized_df`.
- Declares pointers to the training and testing data frames and initializes them to NULL: `training_df` and `testing_df`.
- Splits the standardized data frame into training and testing data frames based on a 0.2 ratio and the label column (5) and stores the pointers in `training_df` and `testing_df`.
- Performs the KNN algorithm on the training and testing data frames with k = 3, 5, and 7 and prints the results.
- Deletes the dataframes and frees the memory allocated for them.
