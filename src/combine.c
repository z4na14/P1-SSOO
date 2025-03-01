#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_ALUMNOS 100

typedef struct{
	char nombre[50];
	int nota;
	int convocatoria;
} alumno_t;

// Store in a global variable the amount of students from each file
// as they will be used in all functions
int num_alumnos1;
int num_alumnos2;

int create_csv(int countM, int countS, int countN, int countA, int countF);
alumno_t* fetch_alumno(const char* filename, alumno_t alumnos[], int num_file);
alumno_t* join_alumnos(const alumno_t* alumnoarr1, const alumno_t* alumnoarr2, alumno_t* sorted_alumnos);
int classify_alumnos(const alumno_t* alumnos);
int output_new_data(alumno_t* alumnos, const char* filename);


int main(const int argc, const char *argv[]){

    // Check if the correct amount of parameters is passed to the program
    if (argc != 4)
    {
        perror("Wrong number of arguments");
	return -1;
    }

    // Fetch students from files
    alumno_t alumnoarr1[MAX_ALUMNOS];
    alumno_t alumnoarr2[MAX_ALUMNOS];
    fetch_alumno(argv[1], alumnoarr1, 0);
    fetch_alumno(argv[2], alumnoarr2, 1);

    if (num_alumnos1 + num_alumnos2 > MAX_ALUMNOS) {
        perror("Students limit surpassed");
        return -1;
    }

    // And join them in a sorted array
    alumno_t sorted_alumnos[num_alumnos1 + num_alumnos2];
    join_alumnos(alumnoarr1, alumnoarr2, sorted_alumnos);

    // Classify students depending on their marks and output CSV
    classify_alumnos(sorted_alumnos);

    // Write the newly created array with sorted data into the output file
    output_new_data(sorted_alumnos, argv[3]);

    return 0;
}

/* Function: create_csv
 *
 * Arguments:
 *  - const int countM: Number of students with a score of 10
 *  - const int countS: Number of students with a score of 9
 *  - const int coountN: Number of students with a score of 8 or 7
 *  - const int countA: Number of students with a score of 6 or 5
 *  - const int countF: Number of students with a score lower than 5
 *
 * Returns:
 *  - -1 in case of error, 0 otherwise
 */
int create_csv(const int countM, const int countS, const int countN, const int countA, const int countF) {

    int total = countM+countS+countN+countA+countF;
    char csv_str[70];
    sprintf(csv_str,
        "M;%d;%.2f%%\nS;%d;%.2f%%\nN;%d;%.2f%%\nA;%d;%.2f%%\nF;%d;%.2f%%",
                countM, ((float) countM/total)*100,
	        countS, ((float) countS/total)*100,
	        countN, ((float) countN/total)*100,
	        countA, ((float) countA/total)*100,
	        countF, ((float) countF/total)*100);

    int csv_file = creat("estadisticas.csv", 0644);

    if (write(csv_file, csv_str, strlen(csv_str)) == -1)
    {
	    perror("Error writing to CSV");
	    exit(-1);
    }
    close(csv_file);

    return 0;
}


/* Function: fetch_alumno
 *
 * Arguments:
 *  - const char* filename: Name of the file where the data is stored
 *
 * Returns:
 *  - Array with the students found in the file
 */
alumno_t* fetch_alumno(const char* filename, alumno_t alumnos[], int num_file) {

    if (strlen(filename) == 0) {return NULL;}

    int file_fd;
    if ((file_fd = open(filename, O_RDONLY)) == -1) {
        perror("Error opening file");
        exit(-1);
    }

    int count_alumnos = 0;
    while (count_alumnos < MAX_ALUMNOS &&
            read(file_fd, &alumnos[count_alumnos], sizeof(alumno_t)) != -1) {
        count_alumnos++;
    }

    if (count_alumnos == 0) {return NULL;}

    switch (num_file) {
        case 0: num_alumnos1 = count_alumnos; break;
        case 1: num_alumnos2 = count_alumnos; break;
        default: perror("Wrong parameter of argument on fetch_alumno"); break;
    }

    return alumnos;
}


// Merge sort functions needed
void merge(alumno_t arr[], int left, int mid, int right);
void merge_sort(alumno_t arr[], int left, int right);
/* Function: join_alumnos
 *
 * Arguments:
 *  - const alumno_t* alumnoarr1: First array of students
 *  - const alumno_t* alumnoarr2: Second array of students
 *
 * Returns:
 *  - Sorted array of the students from the two initial files
 */
alumno_t* join_alumnos(const alumno_t* alumnoarr1, const alumno_t* alumnoarr2, alumno_t* sorted_alumnos)  {

    int i, j;
    int k = 0;

    // Join both arrays of students into a single one
    for (i = 0; i < num_alumnos1; i++) {
        sorted_alumnos[k] = alumnoarr1[i];
        k++;
    }
    for (j = 0; j < num_alumnos2; j++) {
        sorted_alumnos[k] = alumnoarr2[j];
        k++;
    }

    // We will use a Merge Sort algorithm for sorting the students by their marks in descending order
    int n = num_alumnos1 + num_alumnos2;
    merge_sort(sorted_alumnos, 0, n - 1);
    return sorted_alumnos;
}

void merge(alumno_t arr[], int left, int mid, int right) {

    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    alumno_t leftArr[n1], rightArr[n2];

    for (i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }
    i = 0;
    j = 0;
    k = 0;
    while (i < n1 && j < n2) {
        if (leftArr[i].nota < rightArr[j].nota) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void merge_sort(alumno_t arr[], int left, int right) {

    if (left < right) {
        // Calculate midpoint
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}


/* Function: classify_alumnos
 *
 * Arguments:
 *  - const alumno_t* alumno_t: Array with the students to classify depending on their marks
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int classify_alumnos(const alumno_t* alumnos) {

    int count_M = 0, count_S = 0, count_N = 0, count_A = 0, count_F = 0;

    int size = num_alumnos1 + num_alumnos2;
    if (size == 0 || alumnos == NULL) {
        create_csv(count_M, count_S, count_N, count_A, count_F);
        return 0;
    }

    // Counting students in each category
    for (int i = 0; i < size; i++) {
        switch (alumnos[i].nota) {
        case 10: count_M++; break;
        case 9: count_S++; break;
        case 8:
        case 7: count_N++; break;
        case 6:
        case 5: count_A++; break;
        default: count_F++; break;
        }
    }

    create_csv(count_M, count_S, count_N, count_A, count_F);
    return 0;
}


/* Function: output_new_data
 *
 * Arguments:
 *  - const alumno_t* alumnos: Array with the students to output in the new file
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int output_new_data(alumno_t* alumnos, const char* filename) {

    int new_file_fd;
    if ((new_file_fd = creat(filename, 0644)) == -1) {
        perror("Error creating new file");
        exit(-1);
    }

    int total_number_alumnos = num_alumnos1 + num_alumnos2;
    for (int i = 0; i < total_number_alumnos; i++) {
        if (write(new_file_fd, &alumnos[i], sizeof(alumno_t)) == -1) {
            perror("Error writing to file");
            exit(-1);
        }
    }

    close(new_file_fd);
    return 0;
}
