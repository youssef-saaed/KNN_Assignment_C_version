#ifndef __DATAFRAMESTRUCTS_H__
#define __DATAFRAMESTRUCTS_H__

// Define an enumeration type for the data frame data types
typedef enum DataFrameDT
{
    DF_STRING, // String data type
    DF_INT, // Integer data type
    DF_DOUBLE, // Double data type
    DF_NULL, // Null data type
} DataFrameDT;

// Define a structure type for the data frame cells
typedef struct Cell
{
    void *data; // Pointer to the cell data
    DataFrameDT type; // Data type of the cell data
    
} Cell;

// Define a structure type for the data frame shape
typedef struct ShapeType
{
    int RowsC; // Number of rows in the data frame
    int ColumnsC; // Number of columns in the data frame
} ShapeType;

// Define a structure type for the data frame
typedef struct DataFrame
{
    char **columns; // Pointer to the array of column names
    Cell ***data; // Pointer to the matrix of cell pointers
    ShapeType Shape; // Shape of the data frame
} DataFrame;

#endif