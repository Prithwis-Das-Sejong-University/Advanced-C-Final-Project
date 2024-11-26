#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

const int size = 50;

typedef char* string;

typedef struct node
{
    string l;

    char c;

    struct node *next;

} node;

typedef struct Details
{
    int day;

    int month;

    char *currency;

    float value;

} Details;

void SelectionSort (float arr[], int size_of_array, int left);

void sort_values(int month, string months[], string values[], int size);

float search_value(int day, int month, const char* currency, Details dataset[], int size);

void SearchandPrint_DayMonth(const char* currency, Details dataset[], int size, float value);

int iscurrency(const char *currency, Details details[], int size);

void findMeanMedianMode_Month (int month, Details dataset[], int size);

void findMeanMedianMode_Currency (char *currency, Details dataset[], int size);

void ChangeOfValueInPeriod (const char *currency, Details dataset[], int day_i, int month_i, int day_f, int month_f, int size);

int main(void)
{
    FILE *file = fopen("test_data.txt", "r");

    if (file == NULL)
    {
        return 1;
    }

    FILE *file2 = fopen("output.txt", "w");

    if (file2 == NULL)
    {
        return 1;
    }

    node *list_of_unique_lines = NULL;

    // number of entries in the file

    int count = 0;

    char line[size];
    
    while (fgets(line, size, file))
    {
        count++;
    }
    
    printf("The number of data entries in the file: %i\n", count);

    printf("---------------------------------------------------------------\n");

    fprintf(file2, "The number of data entries in the file: %i\n", count);

    fprintf(file2, "---------------------------------------------------------------\n");

    // number of unique entries

    int unique_count = 0;

    char **lines = malloc(count * sizeof(string));

    if (lines == NULL)
    {
        fclose(file);

        return 1;
    }

    // reset file pointer and fill the "lines" array with every line in the file

    rewind(file);

    int index = 0;

    while (fgets(line, size, file))
    {

        lines[index] = malloc((strlen(line) + 1) * sizeof(char)); // an additional one for the '\0'

        if (lines[index] != NULL)
        {
            strcpy(lines[index], line);
        }

        index++;
    }

    // checking for the unique lines

    for (int i = 0; i < index; i++)
    {
        int is_unique = 1;

        for (int j = 0; j < i; j++)
        {
            if (strcmp(lines[i], lines[j]) == 0)
            {
                is_unique = 0;

                break;
            }
        }

        if (is_unique)
        {
            unique_count++;

            //prepending the unique lines into the linked list

            node *n = malloc(sizeof(node));

            n->l = lines[i];

            n->next = NULL;

            if (list_of_unique_lines == NULL)
            {
                list_of_unique_lines = n;
            }

            else
            {
                n->next = list_of_unique_lines;

                list_of_unique_lines = n;
            }            
        }
    }

    printf("The number of unique data entries in the file: %i\n", unique_count);

    printf("---------------------------------------------------------------\n");

    fprintf(file2, "The number of unique data entries in the file: %i\n", unique_count);

    fprintf(file2, "---------------------------------------------------------------\n");

    // number of duplicates

    printf("The number of duplicates in the data are: %i\n", count - unique_count);
    
    printf("---------------------------------------------------------------\n");

    fprintf(file2, "The number of duplicates in the data are: %i\n", count - unique_count);
    
    fprintf(file2, "---------------------------------------------------------------\n");
    
    // extracting the individual data from each line and computing error lines

    char **currency = malloc(unique_count * sizeof(char *));

    char **value = malloc(unique_count * sizeof(char *));

    char **day = malloc(unique_count * sizeof(char *));

    char **month = malloc(unique_count * sizeof(char *));

    Details *details = malloc(unique_count * sizeof(Details));

    int ind = 0, error_lines = 0;

    for (node *ptr = list_of_unique_lines; ptr != NULL; ptr = ptr->next)
    {
        currency[ind] = malloc(4 * sizeof(char));
        
        value[ind] = malloc(10 * sizeof(char));
        
        day[ind] = malloc(3 * sizeof(char));
        
        month[ind] = malloc(3 * sizeof(char));

        string token, tokens[10];

        int counter = 0;

        token = strtok(ptr->l, " ");

        while (token != NULL)
        {
            tokens[counter++] = token;

            token = strtok(NULL, " ");
        }

        strcpy(currency[ind], tokens[2]);

        strcpy(value[ind], tokens[3]);

        strcpy(day[ind], tokens[0]);

        strcpy(month[ind], tokens[1]);

        details[ind].day = atoi(day[ind]);

        details[ind].month = atoi(month[ind]);

        details[ind].currency = currency[ind];

        details[ind].value = atof(value[ind]);

        if (atof(value[ind]) <= 0 || (atoi(day[ind]) <= 0 || atoi(day[ind]) > 31) || (atoi(month[ind]) <= 0 || atoi(month[ind]) > 12) || (atoi(month[ind]) % 2 == 0 && atoi(day[ind]) > 30) || (atoi(month[ind]) == 2 && atoi(day[ind]) > 29))
        {
            error_lines++;
        }

        ind++;

    }

    // extracting the currencies

    char **unique_currencies = malloc(unique_count * sizeof(char *));

    printf("The currencies in the data are: \n");

    printf("---------------------------------------------------------------\n");

    fprintf(file2, "The currencies in the data are: \n");

    fprintf(file2, "---------------------------------------------------------------\n");

    int ctr = 0;

    for (int i = 0; i < unique_count; i++)
    {
        bool is_unique = true;

        for (int j = 0; j < i; j++) 
        {
            if (strcmp(currency[i], currency[j]) == 0) 
            {
                is_unique = false;
                
                break;
            }
        }

        if (is_unique) 
        {
            unique_currencies[ctr] = currency[i];

            printf("%s\n", unique_currencies[ctr]);

            ctr++;
        }
    }

    // printing the number of error lines

    printf("The number of error lines are: %i\n", error_lines);

    printf("---------------------------------------------------------------\n");

    fprintf(file2, "The number of error lines are: %i\n", error_lines);

    fprintf(file2, "---------------------------------------------------------------\n");

    // min and max value for a given currency

    for (int i = 0; i < ctr; i++)
    {
        float *values = malloc(unique_count * sizeof(float));

        int ctr_2 = 0;

        for (int j = 0;  j < ind; j++)
        {
            if (strcmp(unique_currencies[i], currency[j]) == 0)
            {
                values[ctr_2] = atof(value[j]);

                ctr_2++;
            }
        }

        float min = values[0], max = values[0];

        for (int k = 0; k < ctr_2; k++)
        {
            if (values[k] < min)
            {
                min = values[k];
            }

            if (values[k] > max)
            {
                max = values[k];
            }
        }

        printf("The min and max values for currency %s: %.5f, %.5f\n", unique_currencies[i], min, max);

        printf("---------------------------------------------------------------\n");

        fprintf(file2, "The min and max values for currency %s: %.5f, %.5f\n", unique_currencies[i], min, max);

        fprintf(file2, "---------------------------------------------------------------\n");

        free(values);
    }

    // Change of value withing given period (starting, ending, min and max points)

    printf("Detection of Value change within a given period: \n");

    //printf("---------------------------------------------------------------\n");

    fprintf(file2, "Detection of Value change within a given period: \n");

    //fprintf(file2, "---------------------------------------------------------------\n");

    fclose(file2);
    
    int dayi, dayf, monthi, monthf;

    char *curr6 = malloc(4 * sizeof(char));

    if (curr6 == NULL)
    {
        return 1;
    }

    printf("Enter the starting Day and Month: ");

    scanf("%i %i", &dayi, &monthi);

    printf("Enter the ending Day and Month: ");

    scanf("%i %i", &dayf, &monthf);

    printf("Enter the currency: ");

    scanf("%s", curr6);

    ChangeOfValueInPeriod (curr6, details, dayi, monthi, dayf, monthf, ind);

    free(curr6);

    printf("---------------------------------------------------------------\n");

    // create sorted linked list for currencies according to alphabetical order

    node *sorted = NULL;

    for (int i = 0; i < ctr; i++)
    {
        node *n = malloc(sizeof(node));

        n->l = unique_currencies[i];

        n->c = unique_currencies[i][0];

        n->next = NULL;

        if (sorted == NULL)
        {
            sorted = n;
        }

        //prepending

        else if (n->c < sorted->c)
        {
            n->next = sorted;

            sorted = n;
        }

        else
        {
            for (node *ptr = sorted; ptr != NULL; ptr = ptr->next)
            {
                //appending

                if (ptr->next == NULL)
                {
                    ptr->next = n;

                    break;
                }

                if (n->c < ptr->next->c)
                {
                    n->next = ptr->next;

                    ptr->next = n;

                    break;
                }
            }
        }
    }
    FILE *file3 = fopen("output.txt", "a");

    printf("The currencies, alphabetically sorted are: \n");

    fprintf(file3, "The currencies, alphabetically sorted are: \n");

    for (node *ptr = sorted; ptr != NULL; ptr = ptr->next)
    {
        printf("%s\n", ptr->l);

        fprintf(file3, "%s\n", ptr->l);
    }

    printf("---------------------------------------------------------------\n");

    fprintf(file3, "---------------------------------------------------------------\n");

    fclose(file3);

    //sorting values in ascending and descending orders based on given month and printing the result

    int mon;
    
    printf("Enter the month number for sorting: ");
    
    scanf("%i", &mon);

    sort_values(mon, month, value, unique_count);

    printf("---------------------------------------------------------------\n");

    //search values based on given day, month and currency

    FILE *file4 = fopen("output.txt", "a");

    printf("Enter the day, month and currency to search the value: \n");

    int d, m;

    char* curr1 = malloc(4 * sizeof(char));

    scanf("%i %i", &d, &m);

    scanf("%s", curr1);

    printf("\nValue Retrieved: %f\n", search_value(d, m, curr1, details, ind));

    fprintf(file4, "\nValue Retrieved: %f\n", search_value(d, m, curr1, details, ind));

    free(curr1);

    printf("---------------------------------------------------------------\n");

    fprintf(file4, "---------------------------------------------------------------\n");

    fclose(file4);

    //search Day/Month based on given currency and value

    printf("Enter the value and currency to search the value: \n");

    float val;

    char* curr2 = malloc(4 * sizeof(char));

    scanf("%f", &val);

    scanf("%s", curr2);

    FILE *file7 = fopen("output.txt","a");


    SearchandPrint_DayMonth(curr2, details, ind, val);

    for (int i = 0; i < ind; i++)
    {
        if (details[i].value == val && strcmp(details[i].currency, curr2) == 0)
        {
            fprintf(file7, "Day/Month Retrieved: %i/%i\n", details[i].day, details[i].month);
        }
    }

    fprintf(file7, "---------------------------------------------------------------\n");

    fclose(file7);

    free(curr2);

    printf("---------------------------------------------------------------\n");

    //verification of currency
    
    printf("Enter the currency for verification: \n");

    FILE *file5 = fopen("output.txt", "a");

    char *curr3 = malloc(4 * sizeof(char));

    if (curr3 == NULL)
    {
        return 1;
    }

    scanf("%s", curr3);

    if (iscurrency(curr3, details, ind) == 0)
    {
        printf("Currency %s exists in the dataset!\n", curr3);

        fprintf(file5, "Currency %s exists in the dataset!\n", curr3);
    }

    free(curr3);

    printf("---------------------------------------------------------------\n");

    fprintf(file5, "---------------------------------------------------------------\n");
    
    fclose(file5);

    //find the mean, median and mode based on given month

    printf("Enter month to find the mean, median and mode values: ");

    int m2;

    scanf("%i", &m2);

    findMeanMedianMode_Month(m2, details, ind);

    printf("---------------------------------------------------------------\n");

    //find mean, median and mode based on given currency

    char *curr4 = malloc(4 * sizeof(char));

    if (curr4 == NULL)
    {
        return 1;
    }

    printf("Enter the currency to find the mean, median and mode values: ");

    scanf("%s", curr4);

    findMeanMedianMode_Currency(curr4, details, ind);

    free(curr4);

    printf("---------------------------------------------------------------\n");

    // percentage change during whole period of given currency (as far as I can contemplate to what it means in reality)

    FILE *file6 = fopen("output.txt", "a");

    char *curr5 = malloc(4 * sizeof(char));

    if  (curr5 == NULL)
    {
        return 1;
    }

    printf("Enter the currency to find the percentage change during the entire period: ");
    
    scanf("%s", curr5);

    int counter = 0;

    for (int i = 0; i < ind; i++)
    {

        if (strcmp(details[i].currency, curr5) == 0)
        {
            counter++;
        }
    }

    float *arr_curr = malloc(counter * sizeof(float));

    int index_alpha = 0;

    if (arr_curr == NULL)
    {
        return 1;
    }

    for (int i = 0; i < ind; i++)
    {
        if (strcmp(details[i].currency, curr5) == 0)
        {
            arr_curr[index_alpha] = details[i].value;

            index_alpha++;
        }   
    }

    if (counter > 0)
    {
        float start = arr_curr[0];

        float end = arr_curr[counter - 1];

        printf("The percentage change for currency %s: %f \% \n", curr5, ((end - start) / start) * 100.0);

        fprintf(file6, "The percentage change for currency %s: %f \% \n", curr5, ((end - start) / start) * 100.0);
    }
    
    else
    {
        printf("No data found for currency %s\n", curr5);

        fprintf(file6, "No data found for currency %s\n", curr5);
    }
    
    free(arr_curr);

    free(curr5);

    fclose(file6);


    // free allocated memory

    for (int i = 0; i < index; i++)
    {
        free(lines[i]);
    }

    free(lines);

    for (node *ptr = sorted; ptr != NULL; ptr = ptr->next)
    {
        node *nxt = ptr->next;

        free(ptr);

        ptr = nxt;
    }

    fclose(file);

    return 0;
}

void sort_values(int month, string months[], string values[], int size)
{
    FILE *file = fopen("output.txt", "a");

    float *arr = malloc(size * sizeof(float));

    int index = 0;

    for (int i = 0; i < size; i++)
    {
        if (month == atoi(months[i]))
        {
            arr[index] = atof(values[i]);

            index++;
        }
    }

    SelectionSort(arr, index, 0);

    printf("The sorted values for the month are (in ascending order): \n");

    fprintf(file, "The sorted values for the month are (in ascending order): \n");

    for (int i = 0; i < index; i++)
    {
        printf("%f ", arr[i]);

        fprintf(file, "%f ", arr[i]);
    }
    printf("\n");
    
    printf("\n");

    fprintf(file, "\n");
    
    fprintf(file, "\n");
    
    printf("The sorted values for the month are (in descending order): \n");

    fprintf(file, "The sorted values for the month are (in descending order): \n");

    for (int i = index - 1; i >= 0; i--)
    {
        printf("%f ", arr[i]);

        fprintf(file, "%f ", arr[i]);
    }

    printf("\n");

    fprintf(file, "\n");

    fprintf(file, "---------------------------------------------------------------\n");

    fclose(file);
}

void SelectionSort (float arr[], int size_of_array, int left)
{   
    if (left >= size_of_array - 1)
    {
        return;
    }    
    
    int min_index = left; 
    
    for (int i = left + 1; i < size_of_array; i++) 
    { 
        if (arr[i] <= arr[min_index]) 
        { 
            min_index = i; 
        } 
    } 
    
    if (min_index != left) 
    { 
        float temp = arr[left]; 
        
        arr[left] = arr[min_index]; 
        
        arr[min_index] = temp; 
    } 
    
    SelectionSort(arr, size_of_array, left + 1);
}

float search_value(int day, int month, const char* currency, Details dataset[], int size) 
{ 
    for (int i = 0; i < size; i++) 
    { 
        if (dataset[i].day == day && dataset[i].month == month && strcmp(dataset[i].currency, currency) == 0) 
        { 
            return dataset[i].value; 
        } 
    } 
    return -1.0; 
}

void SearchandPrint_DayMonth(const char* currency, Details dataset[], int size, float value)
{
    for (int i = 0; i < size; i++)
    {
        if (dataset[i].value == value && strcmp(dataset[i].currency, currency) == 0)
        {
            printf("Day/Month Retrieved: %i/%i\n", dataset[i].day, dataset[i].month);
        }
    }
    
    return;
}

int iscurrency(const char *currency, Details details[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(details[i].currency, currency) == 0)
        {
            return 0;

            break;
        }
    }

    return 1;
}

void findMeanMedianMode_Month (int month, Details dataset[], int size)
{
    FILE *file = fopen("output.txt", "a");

    float mean = 0.0, median, mode;

    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        if (dataset[i].month == month)
        {
            counter++;
        }
    }

    float *arr = malloc(counter * sizeof(float));

    int index = 0;

    if (arr == NULL)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (dataset[i].month == month)
        {
            arr[index] = dataset[i].value;

            mean += dataset[i].value;

            index++;
        }   
    }

    SelectionSort(arr, counter, 0);

    // the median

    if (counter % 2 == 0)
    {
        median = (arr[counter / 2 - 1] + arr[counter / 2]) / 2.0;
    }

    else
    {
        median = arr[counter / 2];
    }

    //the mean

    mean /= counter;

    // the mode

    int maxCount = 0;

    mode = arr[0];

    int count = 1;

    for (int i = 1; i < counter; i++)
    {
        if (arr[i] == arr[i - 1])
        {
            count++;
        }

        else
        {
            count = 1;
        }

        if (count > maxCount)
        {
            mode = arr[i];
        }
    }

    printf("The mean, median and mode based on the entered month: %f, %f, %f\n", mean, median, mode);

    fprintf(file, "The mean, median and mode based on the entered month: %f, %f, %f\n", mean, median, mode);

    fprintf(file, "---------------------------------------------------------------\n");

    free(arr);

    fclose(file);
}

void findMeanMedianMode_Currency (char *currency, Details dataset[], int size)
{
    FILE *file = fopen("output.txt", "a");
    
    float mean = 0.0, median, mode;

    int counter = 0;

    for (int i = 0; i < size; i++)
    {
        if (strcmp(dataset[i].currency, currency) == 0)
        {
            counter++;
        }
    }

    float *arr = malloc(counter * sizeof(float));

    int index = 0;

    if (arr == NULL)
    {
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (strcmp(dataset[i].currency, currency) == 0)
        {
            arr[index] = dataset[i].value;

            mean += dataset[i].value;

            index++;
        }   
    }

    SelectionSort(arr, counter, 0);

    // the median

    if (counter % 2 == 0)
    {
        median = (arr[counter / 2 - 1] + arr[counter / 2]) / 2.0;
    }

    else
    {
        median = arr[counter / 2];
    }

    //the mean

    mean /= counter;

    // the mode

    int maxCount = 0;

    mode = arr[0];

    int count = 1;

    for (int i = 1; i < counter; i++)
    {
        if (arr[i] == arr[i - 1])
        {
            count++;
        }

        else
        {
            count = 1;
        }

        if (count > maxCount)
        {
            maxCount = count;

            mode = arr[i];
        }
    }

    printf("The mean, median and mode based on the entered currency: %f, %f, %f\n", mean, median, mode);

    fprintf(file, "The mean, median and mode based on the entered currency: %f, %f, %f\n", mean, median, mode);

    fprintf(file, "---------------------------------------------------------------\n");

    free(arr);

    fclose(file);
}        


void ChangeOfValueInPeriod (const char *currency, Details dataset[], int day_i, int month_i, int day_f, int month_f, int size)
{
    FILE *file = fopen("output.txt", "a");

    int reduced_size = 0;

    for (int i = 0; i < size; i++)
    {
        if (strcmp(dataset[i].currency, currency) == 0)
        {
            reduced_size++;
        }
    }

    float *all_values_in_curr = malloc(reduced_size * sizeof(float));

    int *all_days_in_curr = malloc(reduced_size * sizeof(int));

    int *all_months_in_curr = malloc(reduced_size * sizeof(int));

    if (all_values_in_curr == NULL || all_days_in_curr == NULL || all_months_in_curr == NULL) 
    {
        free(all_values_in_curr);
        
        free(all_days_in_curr);
        
        free(all_months_in_curr);

        return;
    }

    int index = 0;

    for (int i = 0; i < size; i++)
    {
        if (strcmp(dataset[i].currency, currency) == 0)
        {
            all_values_in_curr[index] = dataset[i].value;
            
            all_days_in_curr[index] = dataset[i].day;

            all_months_in_curr[index] = dataset[i].month;

            index++;
        }
    }
    int start_index, end_index;

    for (int i = 0; i < reduced_size; i++)
    {
        if ((day_i >= 1 && day_i <= 31) && (day_f >= 1 && day_f <= 31) && (month_i >= 1 && month_i <= 12) && (month_f >= 1 && month_f <= 12))
        {
            if (day_i == all_days_in_curr[i] && month_i == all_months_in_curr[i])
            {
                start_index = i;

                for (int j = i; j < reduced_size; j++)
                {
                    if (day_f == all_days_in_curr[j] && month_f == all_months_in_curr[j])
                    {
                        end_index = j;

                        break;
                    }
                }

                if (end_index == -1)
                {
                    break;
                }
            }
        }

        if (start_index == -1 || end_index == -1) 
        {
            free(all_values_in_curr);

            free(all_days_in_curr);

            free(all_months_in_curr);

            return;
        }
    }

    float starting = all_values_in_curr[start_index], ending = all_values_in_curr[end_index];

    float min = all_values_in_curr[start_index], max = all_values_in_curr[start_index];

    for (int i = start_index; i < end_index + 1; i++)
    {
        if (all_values_in_curr[i] < min)
        {
            min = all_values_in_curr[i];
        }

        if(all_values_in_curr[i] > max)
        {
            max = all_values_in_curr[i];
        }
    }

    printf("Starting Value: %f  Ending Value: %f  Minimum Value: %f  Maximum Value: %f\n", starting, ending, min, max);

    fprintf(file, "Starting Value: %f  Ending Value: %f  Minimum Value: %f  Maximum Value: %f\n", starting, ending, min, max);

    fprintf(file, "---------------------------------------------------------------\n");

    fclose(file);

    free(all_values_in_curr);

    free(all_days_in_curr);

    free(all_months_in_curr);
}