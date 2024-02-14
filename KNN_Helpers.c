#include "KNN_Helpers.h"

// Define a function that splits a data frame into training and testing data frames based on a given ratio and a label column
void split_samples(DataFrame* df, DataFrame **training_df, DataFrame **testing_df, double test_data_ratio, int labelcol)
{
    // Create a new data frame for the testing data with the number of rows and columns based on the ratio and store the pointer in *testing_df
    *testing_df = new_dataframe(df->Shape.RowsC * test_data_ratio, df->Shape.ColumnsC);
    // Create a new data frame for the training data with the number of rows and columns based on the ratio and store the pointer in *training_df
    *training_df = new_dataframe(df->Shape.RowsC - (df->Shape.RowsC * test_data_ratio) + 1, df->Shape.ColumnsC);
    // Allocate memory for an array of column names for the testing data and store the pointer in (*testing_df)->columns
    (*testing_df)->columns = malloc(sizeof(char *) * df->Shape.ColumnsC);
    // Loop through each column of the original data frame
    for (int i = 0; i < df->Shape.ColumnsC; i++)
    {
        // Allocate memory for a copy of the column name and store the pointer in (*testing_df)->columns[i]
        (*testing_df)->columns[i] = malloc(sizeof(char) * (strlen(df->columns[i]) + 1));
        // Copy the column name from the original data frame to the testing data frame
        strcpy((*testing_df)->columns[i], df->columns[i]);
        // Add a null terminator to the testing column name
        (*testing_df)->columns[i][strlen(df->columns[i])] = '\0';  
    }
    // Allocate memory for an array of column names for the training data and store the pointer in (*training_df)->columns
    (*training_df)->columns = malloc(sizeof(char *) * df->Shape.ColumnsC);
    // Loop through each column of the original data frame
    for (int i = 0; i < df->Shape.ColumnsC; i++)
    {
        // Allocate memory for a copy of the column name and store the pointer in (*training_df)->columns[i]
        (*training_df)->columns[i] = malloc(sizeof(char) * (strlen(df->columns[i]) + 1));
        // Copy the column name from the original data frame to the training data frame
        strcpy((*training_df)->columns[i], df->columns[i]);
        // Add a null terminator to the training column name
        (*training_df)->columns[i][strlen(df->columns[i])] = '\0';  
    }
    // Declare and initialize a pointer to an array of labels
    char **labels = NULL;
    // Declare and initialize a variable to store the number of labels
    int lc = 0;
    // Loop through each row of the original data frame
    for (int i = 0; i < df->Shape.RowsC; i++)
    {
        // If the labels array is NULL
        if (!labels)
        {
            // Allocate memory for the first label and store the pointer in labels
            labels = malloc(sizeof(char*));
            // Allocate memory for a copy of the label data and store the pointer in labels[0]
            labels[0] = malloc(sizeof(char) * (strlen((char*)df->data[i][labelcol]->data) + 1));
            // Copy the label data from the original data frame to the labels array
            strcpy(labels[0], (char*)df->data[i][labelcol]->data);
            // Add a null terminator to the label data
            labels[0][strlen((char*)df->data[i][labelcol]->data)] = '\0';
            // Increment the number of labels
            lc++;
        }
        // Else
        else
        {
            // Declare and initialize a flag to indicate if the label is found in the labels array
            int found = 0;
            // Loop through each label in the labels array
            for (int j = 0; j < lc; j++)
            {
                // If the label matches the label data from the original data frame
                if (!strcmp(labels[j], (char*)df->data[i][labelcol]->data))
                {
                    // Set the flag to 1
                    found = 1;
                    // Break the inner loop
                    break;
                }
            }
            // If the flag is 0, it means the label is not found in the labels array
            if (!found)
            {
                // Increment the number of labels
                lc++;
                // Reallocate memory for the labels array to accommodate the new label
                labels = realloc(labels, sizeof(char*) * (lc));
                // Allocate memory for a copy of the label data and store the pointer in labels[lc - 1]
                labels[lc - 1] = malloc(sizeof(char) * (strlen((char*)df->data[i][labelcol]->data) + 1));
                // Copy the label data from the original data frame to the labels array
                strcpy(labels[lc - 1], (char*)df->data[i][labelcol]->data);
                // Add a null terminator to the label data
                labels[lc - 1][strlen((char*)df->data[i][labelcol]->data)] = '\0';
            }
        }
    }
        // Allocate memory for an array of label counts for the testing data and store the pointer in labelcounts
    int *labelcounts = malloc(sizeof(int) * lc);
    // Allocate memory for an array of label counts for the training data and store the pointer in labelcounts2
    int *labelcounts2 = malloc(sizeof(int) * lc);
    // Loop through each label
    for (int i = 0; i < lc; i++)
    {
        // Initialize the label counts to zero
        labelcounts[i] = 0;
    }
    // Loop through each row of the original data frame
    for (int i = 0; i < df->Shape.RowsC; i++)
    {
        // Loop through each label
        for (int j = 0; j < lc; j++)
        {
            // If the label matches the label data from the original data frame
            if (!strcmp(labels[j], (char*)df->data[i][labelcol]->data))
            {
                // Increment the label count for the testing data
                labelcounts[j]++;
                // Break the inner loop
                break;
            }
        }
    }
    // Loop through each label
    for (int i = 0; i < lc; i++)
    {
        // Calculate the label count for the training data by subtracting the label count for the testing data multiplied by the ratio
        labelcounts2[i] = labelcounts[i] - labelcounts[i] * test_data_ratio;
        // Calculate the label count for the testing data by multiplying the label count by the ratio
        labelcounts[i] = labelcounts[i] * test_data_ratio;
        // If the label is the last one
        if (i == lc - 1)
        {
            // Declare and initialize variables to store the total number of rows for the testing and training data
            int n = 0, n2 = 0;
            // Loop through each label
            for (int j = 0; j < lc; j++)
            {
                // Add the label counts to the total number of rows
                n += labelcounts[j];
                n2 += labelcounts2[j];
            }
            // Adjust the label count for the testing data by adding the difference between the actual number of rows and the total number of rows
            labelcounts[i] += (*testing_df)->Shape.RowsC - n;
            // Adjust the label count for the training data by adding the difference between the actual number of rows and the total number of rows
            labelcounts2[i] += (*training_df)->Shape.RowsC - n2;
        }
    }
    // Seed the random number generator with the current time
    srand(time(NULL));
    // Allocate memory for an array of chosen row indices and store the pointer in chosen
    int *chosen = malloc(sizeof(int) * df->Shape.RowsC);
    // Declare and initialize a variable to store the number of chosen rows
    int c = 0;
    // Start an infinite loop
    while (1)
    {
        // If the number of chosen rows is equal to the number of rows for the testing data
        if (c >= (*testing_df)->Shape.RowsC)
        {
            // Break the loop
            break;
        }
        // Generate a random row index between 0 and the number of rows for the original data frame
        int row = rand() % df->Shape.RowsC;
        // Declare and initialize a flag to indicate if the row index is invalid
        int invalid = 0;
        // Loop through each chosen row index
        for (int i = 0; i < c; i++)
        {
            // If the chosen row index matches the random row index
            if (chosen[i] == row)
            {
                // Set the flag to 1
                invalid = 1;
                // Break the inner loop
                break;
            }
        }
        // If the flag is 1
        if (invalid)
        {
            // Continue the outer loop
            continue;
        }
        // Reset the flag to 0
        invalid = 0;
        // Loop through each label
        for (int i = 0; i < lc; i++)
        {
            // If the label matches the label data from the original data frame
            if (!strcmp((char*)df->data[row][labelcol], labels[i]))
            {
                // If the label count for the testing data is positive
                if (labelcounts[i])
                {
                    // Decrement the label count for the testing data
                    labelcounts[i]--;
                }
                // Else
                else
                {
                    // Set the flag to 1
                    invalid = 1;
                }
                // Break the inner loop
                break;
            }
        }
        // If the flag is 1
        if (invalid)
        {
            // Continue the outer loop
            continue;
        }
        // Loop through each column of the original data frame
        for (int i = 0; i < df->Shape.ColumnsC; i++)
        {
            // Copy the cell from the original data frame to the testing data frame and store the pointer in (*testing_df)->data[c][i]
            (*testing_df)->data[c][i] = copy_cell(df->data[row][i]);
        }
        // Increment the number of chosen rows
        c++;
    }
    // Start another infinite loop
    while (1)
    {
        // If the number of chosen rows is equal to the number of rows for the original data frame
        if (c >= df->Shape.RowsC)
        {
            // Break the loop
            break;
        }
        // Generate a random row index between 0 and the number of rows for the original data frame
        int row = rand() % df->Shape.RowsC;
        // Declare and initialize a flag to indicate if the row index is invalid
        int invalid = 0;
        // Loop through each chosen row index
        for (int i = 0; i < c; i++)
        {
            // If the chosen row index matches the random row index
            if (chosen[i] == row)
            {
                // Set the flag to 1
                invalid = 1;
                // Break the inner loop
                break;
            }
        }
        // If the flag is 1
        if (invalid)
        {
            // Continue the outer loop
            continue;
        }
        // Reset the flag to 0
        invalid = 0;
        // Loop through each label
        for (int i = 0; i < lc; i++)
        {
            // If the label matches the label data from the original data frame
            if (!strcmp((char*)df->data[row][labelcol], labels[i]))
            {
                // If the label count for the training data is positive
                if (labelcounts2[i])
                {
                    // Decrement the label count for the training data
                    labelcounts2[i]--;
                }
                // Else
                else
                {
                    // Set the flag to 1
                    invalid = 1;
                }
                // Break the inner loop
                break;
            }
        }
        // If the flag is 1
        if (invalid)
        {
            // Continue the outer loop
            continue;
        }
        // Loop through each column of the original data frame
        for (int i = 0; i < df->Shape.ColumnsC; i++)
        {
            // Copy the cell from the original data frame to the training data frame and store the pointer in (*training_df)->data[c - (*testing_df)->Shape.RowsC][i]
            (*training_df)->data[c - (*testing_df)->Shape.RowsC][i] = copy_cell(df->data[row][i]);
        }
        // Increment the number of chosen rows
        c++;
    }
    // Loop through each label
    for (int i = 0; i < lc; i++)
    {
        // Free the memory allocated for the label data
        free(labels[i]);
        // Set the label data to NULL
        labels[i] = NULL;
    }
    // Free the memory allocated for the labels array
    free(labels);
    // Set the labels array to NULL
    labels = NULL;
    // Free the memory allocated for the label counts for the testing data
    free(labelcounts);
    // Set the label counts for the testing data to NULL
    labelcounts = NULL;
    // Free the memory allocated for the label counts for the training data
    free(labelcounts2);
    // Set the label counts for the training data to NULL
    labelcounts2 = NULL;
    // Free the memory allocated for the chosen row indices
    free(chosen);
    // Set the chosen row indices to NULL
    chosen = NULL;
}

// Define a function that calculates the Euclidean distance between two points of the same length and returns the result as a double
double ecludian_dist(double *p1, double *p2, int len)
{
    // Declare and initialize a variable to store the sum of the squared differences
    double sum = 0;
    // Loop through each dimension of the points
    for (int i = 0; i < len; i++)
    {
        // Add the squared difference of the corresponding coordinates to the sum
        sum += pow(p1[i] - p2[i], 2);
    }
    // Return the square root of the sum
    return sqrt(sum);
}

// Define a function that performs the K-Nearest Neighbors algorithm on the training and testing data frames and prints the results
void KNN_training_res(DataFrame *training_df, DataFrame *testing_df, int k)
{
    // Declare and initialize a variable to store the number of correct predictions
    int c = 0;
    // Loop through each row of the testing data frame
    for (int i = 0; i < testing_df->Shape.RowsC; i++)
    {
        // Allocate memory for an array of distances from the testing point to each training point and store the pointer in dists
        double *dists = malloc(sizeof(double) * training_df->Shape.RowsC);
        // Allocate memory for an array of row indices for the training points and store the pointer in rows
        int *rows = malloc(sizeof(int) * training_df->Shape.RowsC);
        // Loop through each row of the training data frame
        for (int j = 0; j < training_df->Shape.RowsC; j++)
        {
            // Store the coordinates of the training point in p1
            double p1[4] = {*((double *)training_df->data[j][1]->data), *((double *)training_df->data[j][2]->data), *((double *)training_df->data[j][3]->data), *((double *)training_df->data[j][4]->data)};
            // Store the coordinates of the testing point in p2
            double p2[4] = {*((double *)testing_df->data[i][1]->data), *((double *)testing_df->data[i][2]->data), *((double *)testing_df->data[i][3]->data), *((double *)testing_df->data[i][4]->data)};
            // Calculate the Euclidean distance between p1 and p2 and store it in dists[j]
            dists[j] = ecludian_dist(p1, p2, 4);
            // Store the row index of the training point in rows[j]
            rows[j] = j;
        }
        // Sort the distances and the row indices in ascending order using bubble sort
        for (int l = 0; l < training_df->Shape.RowsC - 1; l++)
        {
            for (int m = 0; m < training_df->Shape.RowsC - l - 1; m++)
            {
                // If the distance at index m is greater than the distance at index m + 1
                if (dists[m] > dists[m + 1])
                {
                    // Swap the distances and the row indices
                    double t1 = dists[m];
                    dists[m] = dists[m + 1];
                    dists[m + 1] = t1;
                    int t2 = rows[m];
                    rows[m] = rows[m + 1];
                    rows[m + 1] = t2;
                }
            }
        }
        // Declare and initialize variables to store the counts of each label among the k nearest neighbors
        int seto_c = 0, vers_c = 0, virg_c = 0;
        // Loop through the k nearest neighbors or the number of training points, whichever is smaller
        for (int j = 0; j < min(k, training_df->Shape.RowsC); j++)
        {
            // If the label of the training point is Iris-setosa
            if (!strcmp((char *)training_df->data[rows[j]][5]->data, "Iris-setosa"))
            {
                // Increment the count of Iris-setosa
                seto_c++;
            }
            // Else if the label of the training point is Iris-versicolor
            else if (!strcmp((char *)training_df->data[rows[j]][5]->data, "Iris-versicolor"))
            {
                // Increment the count of Iris-versicolor
                vers_c++;
            }
            // Else if the label of the training point is Iris-virginica
            else if (!strcmp((char *)training_df->data[rows[j]][5]->data, "Iris-virginica"))
            {
                // Increment the count of Iris-virginica
                virg_c++;
            }
        }
        // Find the maximum count among the three labels and store it in max_vote
        int max_vote = max(seto_c, max(vers_c, virg_c));
        // If the maximum count matches the label of the testing point
        if ((max_vote == seto_c && !strcmp((char *)testing_df->data[i][5]->data, "Iris-setosa")) || (max_vote == vers_c && !strcmp((char *)testing_df->data[i][5]->data, "Iris-versicolor")) || (max_vote == virg_c && !strcmp((char *)testing_df->data[i][5]->data, "Iris-virginica")))
        {
            // Increment the number of correct predictions
            c++;
        }
        // Free the memory allocated for the distances array
        free(dists);
        // Set the distances array to NULL
        dists = NULL;
        // Free the memory allocated for the row indices array
        free(rows);
        // Set the row indices array to NULL
        rows = NULL;
    }
    // Print the accuracy of the K-Nearest Neighbors model with the given k value
    printf("K-NearNeighbour Model with K = %d => Acuraccy: %lf%%\n", k, c * 1.0 / testing_df->Shape.RowsC * 100);
}