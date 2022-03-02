#include <stdlib.h>
#include <stdio.h>

struct book_t
{
    char author[64];
    char publisher[64];
    int year;
    float rating;
};

void print_book(struct book_t book) {
    printf("Author: %s, Publisher: %s, Year: %d, Rating: %f", book.author, book.publisher, book.year, book.rating);
}


int sum_of_array(int* arr, int size);
char* concatenate(char *arr1, char *arr2);

char* concatenate(char *arr1, char *arr2)
{
    int size1 = 0, size2 = 0;
    
    char* tmp_arr1 = arr1;
    while (*tmp_arr1 != '\0') {
        tmp_arr1++;
        size1++;
    }
    char* tmp_arr2 = arr2;
    while (*tmp_arr2 != '\0') {
        tmp_arr2++;
        size2++;
    }

    printf("size1: %d, size2: %d", size1, size2);
    char* res = (char*)malloc(size1 + size2 + 1);
    
    for (int i = 0; i < size1; i++) {
        res[i] = arr1[i];
    }

    for (int i = 0; i < size2; i++) {
        res[i+size1] = arr2[i];
    }
    res[size1+size2] = '\0';
    
    return res;
}

int sum_of_array(int* arr, int size)
{
    int res = 0;

    for (int i = 0; i < size; i++)
        res += arr[i];

    return res;
}

void main()
{
    //1. By using for loop print first 10 even numbers.
    for (int i = 1; i < 20; i+=2) {
        printf("%d ", i);
    }

    printf("\r\n");
    // 2. Define a new character array with “Hello World” and print each character separately utilizing
    // the while loop. Don’t use integer indexes but rather loop through the string by its pointer.
    char hello_world[] = "Hello World";
    char* ptr = hello_world;
    while (*ptr != '\0') {
        printf("%c", *ptr);
        ptr++;
    }

    printf("\n\r");
    //3. Define a function that returns the sum of all integer numbers given as a function input in the
    // form of an array of integers.
    int arr[] = {15, 20, 25, 30, 35};
    sum_of_array(arr, sizeof(arr)/sizeof(int));

    //4. Define a function that concatenates two char arrays given as an input argument to the
    //function and returns a pointer to a dynamically created char array containing the
    //concatenated text.
    printf("\n\r");
    char *concatenated = concatenate("Hello_", "World");
    printf("\n\r");
    while (*concatenated != '\0') {
        printf("%c", *concatenated);
        concatenated++;
    }

    printf("\n\r");
    /*5. Create a new structure containing information about the book, which bears information
    about the author, publisher, year of publishing, and average users rating inf the form of float
    pointer number. Then create a new instance of this structure and fill it with data. Finally,
    create a function for formatted printing of this structure. The function will have a single
    input (book structure) and no return statement.*/
    struct book_t book = {"Author1", "Publisher1", 1988, 5.f};
    print_book(book);
}
