#include "Dataframe.h"

// Define a function that creates a new cell and returns a pointer to it
Cell* new_cell()
{
    // Allocate memory for a cell and store the pointer in cell
    Cell *cell = malloc(sizeof(Cell));
    // Initialize the cell data to NULL
    cell->data = NULL;
    // Initialize the cell type to DF_NULL
    cell->type = DF_NULL;
    // Return the pointer to the cell
    return cell;
}

// Define a function that creates a new data frame with the given number of rows and columns and returns a pointer to it
DataFrame* new_dataframe(int nrows, int ncols)
{
    // Allocate memory for a data frame and store the pointer in df
    DataFrame *df = malloc(sizeof(DataFrame));
    // Allocate memory for an array of cell pointers and store the pointer in df->data
    df->data = malloc(sizeof(Cell*) * nrows);
    // Set the number of columns in df->Shape.ColumnsC
    df->Shape.ColumnsC = ncols;
    // Set the number of rows in df->Shape.RowsC
    df->Shape.RowsC = nrows;
    // Loop through each row of the data frame
    for (int i = 0; i < nrows; i++)
    {
        // Allocate memory for an array of cell pointers and store the pointer in df->data[i]
        df->data[i] = malloc(sizeof(Cell*) * ncols);
    }
    // Return the pointer to the data frame
    return df;
}

// Define a function that parses a string and returns a pointer to a cell with the corresponding data and type
Cell* parse_cell(char *str)
{
    // Create a new cell and store the pointer in cell
    Cell *cell = new_cell();
    // If the string is NULL, return the cell
    if (!str)
    {
        return cell;
    }
    // Declare and initialize variables to check the type of the string
    int isalpha = 0; // Flag for alphabetic characters
    int isnumerical = 0; // Flag for numerical characters
    int isdouble = 0; // Flag for decimal point
    // Declare and initialize a pointer to the string
    char *temp = str;
    // Loop through the string until the end or until an alphabetic character is found
    while (*temp != '\0' && !isalpha)
    {
        // If the character is a decimal point, set isdouble to 1
        if (*temp == '.')
        {
            isdouble = 1;
        }
        // Else if the character is a digit, set isnumerical to 1
        else if (*temp >= '0' && *temp <= '9')
        {
            isnumerical = 1;
        }
        // Else, set isalpha to 1
        else
        {
            isalpha = 1;
        }
        // Move the pointer to the next character
        temp++;
    }
    // If the string contains alphabetic characters, set the cell type to DF_STRING
    if (isalpha)
    {
        cell->type = DF_STRING;
    }
    // Else if the string contains a decimal point, set the cell type to DF_DOUBLE
    else if (isdouble)
    {
        cell->type = DF_DOUBLE;
    }
    // Else if the string contains only numerical characters, set the cell type to DF_INT
    else if (isnumerical)
    {
        cell->type = DF_INT;
    }
    // Switch on the cell type
    switch (cell->type)
    {
        // If the cell type is DF_STRING
        case DF_STRING:
        {
            // Allocate memory for a copy of the string and store the pointer in cpstr
            char *cpstr = malloc(sizeof(char) * (strlen(str) + 1));
            // Declare and initialize a variable to store the index of the string
            int i = 0;
            // Loop through the string until the end
            while (*str != '\0')
            {
                // Copy the character from the string to cpstr
                cpstr[i] = *str;
                // Move the pointer to the next character
                str++;
                // Increment the index
                i++;   
            }
            // Add a null terminator to cpstr
            cpstr[i] = '\0';
            // Set the cell data to cpstr
            cell->data = cpstr;
            // Break the switch
            break;
        }
        // If the cell type is DF_INT
        case DF_INT:
        {
            // Allocate memory for an integer and store the pointer in cell->data
            cell->data = malloc(sizeof(int));
            // Initialize the integer to 0
            *((int*)(cell->data)) = 0;
            // Loop through the string until the end
            while (*str != '\0')
            {
                // Multiply the integer by 10
                *((int*)(cell->data)) *= 10;
                // Add the digit from the string to the integer
                *((int*)(cell->data)) += *str - '0';
                // Move the pointer to the next character
                str++;
            }
            // Break the switch
            break;
        }
        // If the cell type is DF_DOUBLE
        case DF_DOUBLE:
        {
            // Allocate memory for a double and store the pointer in cell->data
            cell->data = malloc(sizeof(double));
            // Initialize the double to 0.0
            *((double*)(cell->data)) = 0.0;
            // Declare and initialize a variable to store the precision of the decimal part
            double prec = 10.0;
            // Declare and initialize a flag to indicate the presence of the decimal point
            int point = 0;
            // Loop through the string until the end
            while (*str != '\0')
            {
                // If the character is a decimal point, set point to 1
                if (*str == '.')
                {
                    point = 1;
                    // Move the pointer to the next character
                    str++;
                    // If the string ends, break the loop
                    if (*str == '\0')
                    {
                        break;
                    }
                }
                // If the decimal point is found
                if (point)
                {
                    // Add the digit from the string to the double divided by the precision
                    *((double*)(cell->data)) += (*str - '0') / prec;
                    // Multiply the precision by 10
                    prec *= 10;
                }
                // Else
                else
                {
                    // Multiply the double by 10
                    *((double*)(cell->data)) *= 10;
                    // Add the digit from the string to the double
                    *((double*)(cell->data)) += *str - '0';
                }
                // Move the pointer to the next character
                str++;
            }
        }
    }
    // Return the pointer to the cell
    return cell;
}

// Define a function that splits a line by a delimiter and returns the number of tokens and a pointer to an array of strings
int split(char *line, char delimiter, char ***strlist)
{
    // Declare and initialize a variable to store the number of tokens
    int c = 0;
    // Declare and initialize a pointer to the line
    char *temp = line;
    // Loop through the line until the end
    while (*temp != '\0')
    {
        // If the character is the delimiter and the next character is not the end, increment the number of tokens
        if (*temp == delimiter && *(temp + 1) != '\0')
        {
            c++;
        }
        // Move the pointer to the next character
        temp++;
    }
    // Increment the number of tokens to account for the last token
    c++;
    // Allocate memory for an array of strings and store the pointer in *strlist
    *strlist = malloc(sizeof(char *) * c);
    // Reset the pointer to the line
    temp = line;
    // Declare and initialize a variable to store the index of the token
    int i = 0;
    // Loop through the line until the end
    while (*temp != '\0')
    {
        // Declare and initialize a pointer to the current token
        char *temp2 = temp;
        // Declare and initialize a variable to store the length of the token
        int c2 = 0;
        // Loop through the token until the end or the delimiter or the newline
        while (*temp2 != '\0' && *temp2 != '\n' && *temp2 != delimiter)
        {
            // Move the pointer to the next character
            temp2++;
            // Increment the length of the token
            c2++;
        }
        // Increment the length of the token to account for the null terminator
        c2++;
        // Declare and initialize a variable to store the index of the character
        int j = 0;
        // Allocate memory for the token and store the pointer in (*strlist)[i]
        (*strlist)[i] = malloc(sizeof(char) * c2);
        // Loop through the token until the end or the delimiter or the newline
        while (*temp != '\0' && *temp != '\n' && *temp != delimiter)
        {
            // Copy the character from the line to (*strlist)[i][j]
            (*strlist)[i][j] = *temp;
            // Move the pointer to the next character
            temp++;
            // Increment the index of the character
            j++;
        }
        // Add a null terminator to (*strlist)[i][j]
        (*strlist)[i][j] = '\0';
        // Move the pointer to the next character
        temp++;
        // Increment the index of the token
        i++;
    }
    // Return the number of tokens
    return c;
}

// Define a function that reads a CSV file and returns a pointer to a data frame with the data from the file
DataFrame* read_csv(const char *filename)
{
    // Open the file in read mode and store the pointer in file
    FILE *file = fopen(filename, "r");
    // If the file is NULL, return NULL
    if (file == NULL)
    {
        return NULL;
    }
    // Declare and initialize a pointer to a line
    char *line = NULL;
    // Declare and initialize a variable to store the length of the line
    size_t len = 0;
    // Declare and initialize a variable to store the number of rows
    int nrows = 0;
    // Loop through the file until the end
    while(getline(&line, &len, file) != -1)
    {
        // Increment the number of rows
        nrows++;
    }
    // Rewind the file pointer to the beginning
    rewind(file);
    // Declare and initialize a pointer to a data frame
    DataFrame *df = NULL;
    // Declare and initialize a variable to store the index of the row
    int row = -1;
    // Loop through the file until the end
    while(getline(&line, &len, file) != -1)
    {
        // Declare and initialize a pointer to an array of strings
        char **splitedline = NULL;
        // Split the line by comma and store the number of tokens in len and the pointer to the array of strings in splitedline
        len = split(line, ',', &splitedline);
        // If the row is -1, it means it is the first line
        if (row == -1)
        {
            // Create a new data frame with the number of rows minus one and the number of tokens and store the pointer in df
            df = new_dataframe(nrows - 1, len);
            // Set the columns of the data frame to splitedline
            df->columns = splitedline;
        }
        // Else, it means it is a data line
        else
        {
            // Loop through each column of the data frame
            for (int i = 0; i < df->Shape.ColumnsC; i++)
            {
                // If the index is less than the number of tokens
                if (i < len)
                {
                    // Parse the string from splitedline[i] and store the pointer to the cell in df->data[row][i]
                    df->data[row][i] = parse_cell(splitedline[i]);
                }
                // Else
                else
                {
                    // Parse a NULL string and store the pointer to the cell in df->data[row][i]
                    df->data[row][i] = parse_cell(NULL);
                }
                // Free the memory allocated for splitedline[i]
                free(splitedline[i]);
                // Set splitedline[i] to NULL
                splitedline[i] = NULL;
            }
            // Free the memory allocated for splitedline
            free(splitedline);
            // Set splitedline to NULL
            splitedline = NULL;
        }
        // Increment the index of the row
        row++;
    }
    // Free the memory allocated for line
    free(line);
    // Set line to NULL
    line = NULL;
    // Close the file
    fclose(file);
    // Return the pointer to the data frame
    return df;
}

// Define a function that prints a cell to the standard output
void print_cell(Cell *c)
{
    // If the cell type is DF_STRING
    if (c->type == DF_STRING)
    {
        // Print the string data of the cell
        printf("%s", (char*)c->data);
    }
    // Else if the cell type is DF_INT
    else if (c->type == DF_INT)
    {
        // Print the integer data of the cell
        printf("%d", *((int*)c->data));
    }
    // Else if the cell type is DF_DOUBLE
    else if (c->type == DF_DOUBLE)
    {
        // Print the double data of the cell
        printf("%lf", *((double*)c->data));
    }
    // Else
    else
    {
        // Print NULL
        printf("NULL");
    }
}

// Define a function that copies a cell and returns a pointer to the copy
Cell* copy_cell(Cell *c)
{
    // Create a new cell and store the pointer in new_c
    Cell *new_c = new_cell();
    // Set the type of the new cell to the type of the original cell
    new_c->type = c->type;
    // If the cell type is DF_STRING
    if (c->type == DF_STRING)
    {
        // Allocate memory for a copy of the string data and store the pointer in new_c->data
        new_c->data = malloc(sizeof(char) * (strlen((char*)c->data) + 1));
        // Copy the string data from the original cell to the new cell
        strcpy((char*)new_c->data, (char*)c->data);
        // Add a null terminator to the new cell data
        ((char*)new_c->data)[strlen((char*)c->data)] = '\0';
    }
    // Else if the cell type is DF_INT
    else if (c->type == DF_INT)
    {
        // Allocate memory for an integer and store the pointer in new_c->data
        new_c->data = malloc(sizeof(int));
        // Copy the integer data from the original cell to the new cell
        *((int*)new_c->data) = *((int*)c->data);
    }
    // Else if the cell type is DF_DOUBLE
    else if (c->type == DF_DOUBLE)
    {
        // Allocate memory for a double and store the pointer in new_c->data
        new_c->data = malloc(sizeof(double));
        // Copy the double data from the original cell to the new cell
        *((double*)new_c->data) = *((double*)c->data);
    }
    // Else
    else
    {
        // Set the new cell data to NULL
        new_c->data = NULL;
    }
    // Return the pointer to the new cell
    return new_c;
}

// Define a function that copies a data frame and returns a pointer to the copy
DataFrame* copy_dataframe(DataFrame* df)
{
    // Allocate memory for a data frame and store the pointer in new_df
    DataFrame *new_df = malloc(sizeof(DataFrame));
    // Set the number of columns in new_df->Shape.ColumnsC to the number of columns in df->Shape.ColumnsC
    new_df->Shape.ColumnsC = df->Shape.ColumnsC;
    // Set the number of rows in new_df->Shape.RowsC to the number of rows in df->Shape.RowsC
    new_df->Shape.RowsC = df->Shape.RowsC;
    // Allocate memory for an array of column names and store the pointer in new_df->columns
    new_df->columns = malloc(sizeof(char *) * new_df->Shape.ColumnsC);
    // Loop through each column of the data frame
    for (int i = 0; i < new_df->Shape.ColumnsC; i++)
    {
        // Allocate memory for a copy of the column name and store the pointer in new_df->columns[i]
        new_df->columns[i] = malloc(sizeof(char) * (strlen(df->columns[i]) + 1));
        // Copy the column name from the original data frame to the new data frame
        strcpy(new_df->columns[i], df->columns[i]);
        // Add a null terminator to the new column name
        new_df->columns[i][strlen(df->columns[i])] = '\0';
    }
    // Allocate memory for a matrix of cell pointers and store the pointer in new_df->data
    new_df->data = malloc(sizeof(Cell*) * new_df->Shape.RowsC);
    // Loop through each row of the data frame
    for (int i = 0; i < new_df->Shape.RowsC; i++)
    {
        // Allocate memory for an array of cell pointers and store the pointer in new_df->data[i]
        new_df->data[i] = malloc(sizeof(Cell*) * new_df->Shape.ColumnsC);
        // Loop through each column of the data frame
        for (int j = 0; j < new_df->Shape.ColumnsC; j++)
        {
            // Copy the cell from the original data frame to the new data frame and store the pointer in new_df->data[i][j]
            new_df->data[i][j] = copy_cell(df->data[i][j]);
        }
    }
    // Return the pointer to the new data frame
    return new_df;
}

// Define a function that calculates the mean of a column in a data frame and returns the result as a double
double mean(DataFrame *df, int col)
{
    // Declare and initialize a variable to store the result
    double res = 0.0;
    // Declare and initialize a variable to store the number of valid cells
    int c = 0;
    // Loop through each row of the data frame
    for (int i = 0; i < df->Shape.RowsC; i++)
    {
        // If the cell type is DF_INT
        if (df->data[i][col]->type == DF_INT)
        {
            // Add the integer data of the cell to the result
            res += *((int*)(df->data[i][col]->data));
            // Increment the number of valid cells
            c++;
        }
        // Else if the cell type is DF_DOUBLE
        else if (df->data[i][col]->type == DF_DOUBLE)
        {
            // Add the double data of the cell to the result
            res += *((double*)(df->data[i][col]->data));
            // Increment the number of valid cells
            c++;
        }
    }
    // Divide the result by the number of valid cells
    return res / c;
}

// Define a function that calculates the standard deviation of a column in a data frame and returns the result as a double
double stdev(DataFrame *df, int col)
{
    // Calculate the mean of the column and store it in u
    double u = mean(df, col);
    // Declare and initialize a variable to store the result
    double res = 0.0;
    // Declare and initialize a variable to store the number of valid cells
    int c = 0;
    // Loop through each row of the data frame
    for (int i = 0; i < df->Shape.RowsC; i++)
    {
        // If the cell type is DF_INT
        if (df->data[i][col]->type == DF_INT)
        {
            // Add the square of the difference between the integer data of the cell and the mean to the result
            res += pow((*((int*)(df->data[i][col]->data)) - u), 2);
            // Increment the number of valid cells
            c++;
        }
        // Else if the cell type is DF_DOUBLE
        else if (df->data[i][col]->type == DF_DOUBLE)
        {
            // Add the square of the difference between the double data of the cell and the mean to the result
            res += pow((*((double*)(df->data[i][col]->data)) - u), 2);
            // Increment the number of valid cells
            c++;
        }
    }
    // Divide the result by the number of valid cells
    res = res / c;
    // Return the square root of the result
    return sqrt(res);
}

// Define a function that standardizes a data frame by subtracting the mean and dividing by the standard deviation for the given columns and returns a pointer to the standardized data frame
DataFrame* standardize(DataFrame *df, char **cols, int cCols)
{
    // Copy the original data frame and store the pointer in new_df
    DataFrame *new_df = copy_dataframe(df);
    // Loop through each column of the new data frame
    for (int i = 0; i < new_df->Shape.ColumnsC; i++)
    {
        // Loop through each column name in the given array
        for (int j = 0; j < cCols; j++)
        {
            // If the column name matches the new data frame column name
            if (!strcmp(new_df->columns[i], cols[j]))
            {
                // Calculate the mean of the column and store it in u
                double u = mean(new_df, i);
                // Calculate the standard deviation of the column and store it in s
                double s = stdev(new_df, i);
                // Loop through each row of the new data frame
                for (int k = 0; k < new_df->Shape.RowsC; k++)
                {
                    // If the cell type is DF_INT
                    if (new_df->data[k][i]->type == DF_INT)
                    {
                        // Store the integer data of the cell in temp
                        double temp = *((int*)(new_df->data[k][i]->data));
                        // Free the memory allocated for the cell data
                        free(new_df->data[k][i]->data);
                        // Allocate memory for a double and store the pointer in the cell data
                        new_df->data[k][i]->data = malloc(sizeof(double));
                        // Set the double data of the cell to the standardized value
                        *((double*)(new_df->data[k][i]->data)) = (temp - u) / s;
                    }
                    // Else if the cell type is DF_DOUBLE
                    else if (new_df->data[k][i]->type == DF_DOUBLE)
                    {
                        // Set the double data of the cell to the standardized value
                        *((double*)(new_df->data[k][i]->data)) = (*((double*)(new_df->data[k][i]->data)) - u) / s;
                    }
                }
                // Break the inner loop
                break;
            }
        }
    }
    // Return the pointer to the standardized data frame
    return new_df;
}

// Define a function that deletes a data frame and frees the memory allocated for it
void delete_dataframe(DataFrame **df)
{
    // Loop through each row of the data frame
    for (int i = 0; i < (*df)->Shape.RowsC; i++)
    {
        // Loop through each column of the data frame
        for (int j = 0; j < (*df)->Shape.ColumnsC; j++)
        {
            // If the row is 0, it means it is the first row
            if (i == 0)
            {
                // Free the memory allocated for the column name
                free((*df)->columns[j]);
                // Set the column name to NULL
                (*df)->columns[j] = NULL;
            }
            // If the cell data is not NULL
            if((*df)->data[i][j]->data)
            {
                // Free the memory allocated for the cell data
                free((*df)->data[i][j]->data);
                // Set the cell data to NULL
                (*df)->data[i][j]->data = NULL;
            }
            // Free the memory allocated for the cell
            free((*df)->data[i][j]);
            // Set the cell to NULL
            (*df)->data[i][j] = NULL;
        }
        // Free the memory allocated for the row
        free((*df)->data[i]);
        // Set the row to NULL
        (*df)->data[i] = NULL;
    }
    // Free the memory allocated for the data
    free((*df)->data);
    // Set the data to NULL
    (*df)->data = NULL;
    // Free the memory allocated for the columns
    free((*df)->columns);
    // Set the columns to NULL
    (*df)->columns = NULL;
    // Free the memory allocated for the data frame
    free(*df);
    // Set the data frame to NULL
    *df = NULL;
}

// Define a function that prints a data frame to the standard output
void print_dataframe(DataFrame *df)
{
    // Loop through each column of the data frame
    for (int j = 0; j < df->Shape.ColumnsC; j++)
    {
        // Print the column name
        printf("%s", df->columns[j]);
        // If the column is not the last one
        if (j != df->Shape.ColumnsC - 1)
        {
            // Print a comma and a space
            printf(", ");
        }
    }
    // Print a newline
    printf("\n");
    // Loop through each row of the data frame
    for (int i = 0; i < df->Shape.RowsC; i++)
    {
        // Loop through each column of the data frame
        for (int j = 0; j < df->Shape.ColumnsC; j++)
        {
            // Print the cell
            print_cell(df->data[i][j]);
            // If the cell type is DF_INT
            if (df->data[i][j]->type == DF_INT)
            {
                // Print :INT
                printf(":INT");
            }
            // Else if the cell type is DF_DOUBLE
            else if (df->data[i][j]->type == DF_DOUBLE)
            {
                // Print :DOUBLE
                printf(":DOUBLE");
            }
            // Else if the cell type is DF_STRING
            else if (df->data[i][j]->type == DF_STRING)
            {
                // Print :STRING
                printf(":STRING");
            }
            // If the column is not the last one
            if (j != df->Shape.ColumnsC - 1)
            {
                // Print a comma and a space
                printf(", ");
            }
        }
        // Print a newline
        printf("\n");
    }
}