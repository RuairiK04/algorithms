//author: Ruairí Kilgallon
//student code: C23428822


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/////////////////////////////////////////////#defines/////////////////////////////////////////////////////////////////////////////////////
#define LOGSIZE 5
#define LINESIZE 4

// Structure to represent a log entry
struct Log {
    int line_code;
    int batch_code;
    int batch_date_time;
    int product_id;
    int issue_code;
    char issue_description[100];
    int resolution_code;
    char resolution_description[100];
    int reporting_employee_id;
};


///////////////////////////////////////////////functions//////////////////////////////////////////////////////////////////////////////////

//log functions
// Function to generate unsorted logs
void generateLogs(struct Log logs[LINESIZE][LOGSIZE]);

// Function to print logs
void printLogs(struct Log arr[], int size);

//task1 functions
// Comparison function used by qsort to compare two log entries
int compare(const void *a, const void *b);


//task 2 functions
// Function to prepare Task 2 report using sorted logs from Task 1
void Task2Report(struct Log logs[LINESIZE][LOGSIZE]);

//task 3 functions
// Function to search for the earliest occurrence of an issue code for a product ID across all production lines
void searchEarliestOccurrence(struct Log logs[LINESIZE][LOGSIZE], int productID);


//task 4 function
// Function to summarize the number of issues reported for each product across all production lines
void summarizeIssues(struct Log logs[LINESIZE][LOGSIZE]);



int main() {

    //declaring structs
    struct Log logs[LINESIZE][LOGSIZE];
    struct Log sortedLogs[LINESIZE][LOGSIZE];
    
    // Initialize sorted logs array
    for (int i = 0; i < LINESIZE; i++) {
        for (int j = 0; j < LOGSIZE; j++) {
            sortedLogs[i][j].line_code = i + 1;
        }
    }

    // Generate unsorted logs
    generateLogs(logs);

    // Print unsorted logs uses the printlogs() function
    printf("Unsorted Logs:\n");
    for (int i = 0; i < LINESIZE; i++) {
        printf("Production Line %d Logs:\n", i+1);
        printLogs(logs[i], LOGSIZE);//printlogs() function used
        printf("\n");
    }

    //task 1
    // Sort logs into separate arrays based on line code
    for (int i = 0; i < LINESIZE; i++) {
        int count = 0;//count to keep track of of logs for each line code 
        for (int j = 0; j < LINESIZE; j++) {
            if (logs[j][0].line_code == i + 1) {//checking if the logs line code matches the current "i+1" value
                for (int k = 0; k < LOGSIZE; k++) {
                    sortedLogs[i][count++] = logs[j][k];//if they match each log & line code are copied into the sorted logs array & count++;
                }
            }
        }
        // Sort the logs for this line using qsort function as well as the compare function
        qsort(sortedLogs[i], LOGSIZE, sizeof(struct Log), compare);// the qsort sorts trhe logs for each line 
    }

    // Print sorted logs
    printf("Sorted Logs:\n");
    for (int i = 0; i < LINESIZE; i++) {
        printf("Production Line %d Logs:\n", i+1);
        printLogs(sortedLogs[i], LOGSIZE);//prinlogs() function in use 
        printf("\n");
    }

    //function for task 2
    Task2Report(logs);

    //task 3
    //ask user for product code to search
    int productID;
    printf("Enter the product ID to search for the earliest occurrence: ");
    scanf("%d", &productID);
    searchEarliestOccurrence(logs, productID);

    //task 4
    summarizeIssues(logs);

    return 0;
}
///////////////////////////////////////////log functions/////////////////////////////////////////////////////////////////////////////////
// Function to generate unsorted logs
void generateLogs(struct Log logs[LINESIZE][LOGSIZE]) {
    srand(time(0));
    for (int i = 0; i < LINESIZE; i++) {
        for (int j = 0; j < LOGSIZE; j++) {
            logs[i][j].line_code = i + 1;
            logs[i][j].batch_code = 100 + (rand() % 100);
            logs[i][j].batch_date_time = 202304010800 + (rand() % 1000);
            logs[i][j].product_id = (i + 1) * 1000 + (rand() % 20); // First digit matches line number
            logs[i][j].issue_code = 200 + (rand() % 20);
            sprintf(logs[i][j].issue_description, "Issue %d", logs[i][j].issue_code);
            logs[i][j].resolution_code = 300 + (rand() % 20);
            sprintf(logs[i][j].resolution_description, "Resolution %d", logs[i][j].resolution_code);
            logs[i][j].reporting_employee_id = 400 + (rand() % 20);
        }
    }
}

// Function to print logs
void printLogs(struct Log arr[], int size) {
    for (int i = 0; i < size; i++) {
        time_t t = arr[i].batch_date_time;
        struct tm *tm_info = localtime(&t);//functions from the <time.h> library
        printf("Product ID: %d, Issue Code: %d, Date & Time: day: %d time: %d:%02d\n",
            arr[i].product_id, arr[i].issue_code, tm_info->tm_mday, tm_info->tm_hour, tm_info->tm_min);
    }
}

////////////////////////////////////////////////////task 1///////////////////////////////////////////////////////////////////////////////
// Comparison function used by qsort to complete task 1
int compare(const void *a, const void *b) {
    
    //comparing 2 log entries & setting structures
    struct Log *logA = (struct Log *)a;
    struct Log *logB = (struct Log *)b;

    if (logA->product_id != logB->product_id) {
        return logA->product_id - logB->product_id; //comparing product IDs
    }
    if (logA->issue_code != logB->issue_code) {
        return logA->issue_code - logB->issue_code; // If product IDs are equal, compare issue code
    }
    return logA->batch_date_time - logB->batch_date_time; // If product IDs and issue codes are equal, compare batch date/time
}

//////////////////////////////////////////////////task 2/////////////////////////////////////////////////////////////////////////////////
// Function to prepare Task 2 report using sorted logs from Task 1
void Task2Report(struct Log logs[LINESIZE][LOGSIZE]) {
    // Print Task 2 report header
    printf("Task 2 Report:\n");
    // Initialize variables to track product ID and line ID
    int currentProductId = -1;
    int currentLineId = -1;

    //loop through each line in the logs array
    for (int i = 0; i < LINESIZE; i++) {
        // loop through each log entry in the current line
        for (int j = 0; j < LOGSIZE; j++) {
            // Get the current log entry
            struct Log currentEntry = logs[i][j];

            // If the product ID or line ID has changed start a new line in the report
            if (currentEntry.product_id != currentProductId || currentEntry.line_code != currentLineId) {
                // If this is not the first line in the report, print a newline
                if (currentProductId != -1)
                    printf("\n");
                // Print the product ID, line ID, and issue code of the current log 
                printf("Product ID: %d, Line ID: %d, Issue Codes: %d", currentEntry.product_id, currentEntry.line_code, currentEntry.issue_code);
            } else {
                // If the product ID and line ID are the same as the previous one print the issue code without starting a new line
                printf(", %d", currentEntry.issue_code);
            }

            // Update current product ID and line ID
            currentProductId = currentEntry.product_id;
            currentLineId = currentEntry.line_code;
        }
    }

    printf("\n");
}

///////////////////////////////////////////////////////task 3/////////////////////////////////////////////////////////////////////////////
// Function to search for the earliest occurrence of an issue code for a product ID across all production lines
void searchEarliestOccurrence(struct Log logs[LINESIZE][LOGSIZE], int productID) {
    int earliestLine = LINESIZE; // Initialize earliest line to the maximum value
    int earliestIndex = LOGSIZE; // Initialize earliest index to the maximum value

    //loop through each production line's sorted logs
    for (int i = 0; i < LINESIZE; i++) {
        int left = 0;//left is first element
        int right = LOGSIZE - 1;//right is the last element

        //Binary search the sorted logs of the current production line
        while (left <= right) {
            int mid = left + (right - left) / 2;//divide and conquor
            if (logs[i][mid].product_id == productID) {//if the product id at mid matches the inputted product id
                // Record the index of the earliest occurrence
                if (i < earliestLine || (i == earliestLine && mid < earliestIndex)) {//checking if the occurance is earlier
                    earliestLine = i;//if it is update values
                    earliestIndex = mid;
                }
                // Continue searching for earlier occurrences
                right = mid - 1;//adjusting the right boundry
            } else if (logs[i][mid].product_id < productID) {//if the product at mid is less than target adjust left
                left = mid + 1;
            } else {//else adjust right
                right = mid - 1;
            }
        }
    }

    // Output the result
    if (earliestLine < LINESIZE && earliestIndex < LOGSIZE) {
        printf("Earliest occurrence: Product ID: %d, Line Code: %d, Issue Code: %d\n",
               logs[earliestLine][earliestIndex].product_id,
               logs[earliestLine][earliestIndex].line_code,
               logs[earliestLine][earliestIndex].issue_code);
    } else {
        printf("Product ID not found\n");
    }
}

    ///////////////////////////////////////////////////////task 4////////////////////////////////////////////////////////////////////////
   // Function to summarize the number of issues reported for each product across all production lines
    void summarizeIssues(struct Log logs[LINESIZE][LOGSIZE]) {
    //array to store the total number of issues reported for each product ID
    int issuesCount[100000] = {0}; //product IDS within the range [1000, 100000]

    //loop through all logs and update the issue counts for each product ID
    for (int i = 0; i < LINESIZE; i++) {
        for (int j = 0; j < LOGSIZE; j++) {
            int productId = logs[i][j].product_id;//extracting the logs into an array
            issuesCount[productId - 1000]++; //updating issue count array
        }
    }

    // Print the summary report
    printf("Summary Report:\n");
    for (int i = 0; i < 100000; i++) { // Assuming product IDs are within the range [1000, 100000]
        if (issuesCount[i] > 0) {
            printf("Product ID: %d, Total Issues: %d\n", i + 1000, issuesCount[i]); // Adjusting product ID for display
        }
    }
}
