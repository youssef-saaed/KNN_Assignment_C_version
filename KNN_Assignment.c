// Include the standard input/output library
#include <stdio.h>
// Include the header file that defines the data frame functions
#include "Dataframe.h"
// Include the header file that defines the K-Nearest Neighbors helper functions
#include "KNN_Helpers.h"

// Define the main function
int main(void)
{
    // Read the Iris.csv file and store the pointer to the data frame in df
    DataFrame *df = read_csv("Iris.csv");
    // Print the data frame to the standard output
    print_dataframe(df);
    // Declare and initialize an array of column names to be standardized
    char *stdcols[4] = {"SepalLengthCm", "SepalWidthCm", "PetalLengthCm", "PetalWidthCm"};
    // Standardize the data frame by subtracting the mean and dividing by the standard deviation for the given columns and store the pointer to the standardized data frame in standardized_df
    DataFrame *standardized_df = standardize(df, stdcols, 4);
    // Declare pointers to the training and testing data frames and initialize them to NULL
    DataFrame *training_df = NULL, *testing_df = NULL;
    // Split the standardized data frame into training and testing data frames based on a 0.2 ratio and the label column (5) and store the pointers in training_df and testing_df
    split_samples(standardized_df, &training_df, &testing_df, 0.2, 5);
    // Perform the K-Nearest Neighbors algorithm on the training and testing data frames with k = 3 and print the results
    KNN_training_res(training_df, testing_df, 3);
    // Perform the K-Nearest Neighbors algorithm on the training and testing data frames with k = 5 and print the results
    KNN_training_res(training_df, testing_df, 5);
    // Perform the K-Nearest Neighbors algorithm on the training and testing data frames with k = 7 and print the results
    KNN_training_res(training_df, testing_df, 7);
    // Delete the original data frame and free the memory allocated for it
    delete_dataframe(&df);
    // Delete the standardized data frame and free the memory allocated for it
    delete_dataframe(&standardized_df);
    // Delete the training data frame and free the memory allocated for it
    delete_dataframe(&training_df);
    // Delete the testing data frame and free the memory allocated for it
    delete_dataframe(&testing_df);
    // Return 0 to indicate successful execution
    return 0;
}
