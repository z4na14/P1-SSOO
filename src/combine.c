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
} alumno;

// Store in a global variable the amount of students from each file
// as they will be used in all functions
int num_alumnos1;
int num_alumnos2;

int create_csv(int countM, int countS, int countN, int countA, int countF);
alumno* fetch_alumno(char* filename, int num_file);
alumno* join_alumnos(alumno* alumnoarr1, alumno* alumnoarr2);
int classify_alumnos(alumno* alumnos);
int output_new_data(alumno* alumnos, char* filename);
void merge(alumno arr[], int left, int mid, int right);
void merge_sort(alumno arr[], int left, int right);


int main(const int argc, const char *argv[]){

        // Check if the correct amount of parameters is passed to the program
	if (argc != 4)
	{
            perror("Wrong number of arguments\n");
	    return -1;
	}

        // Fetch stundents and join them in a sorted array
	alumno* sorted_alumnos = join_alumnos(fetch_alumno(argv[1], 0), fetch_alumno(argv[2], 1));

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
int create_csv(const int countM, const int countS, const int countN, const int countA, const int countF)
{
	int total = countM+countS+countN+countA+countF;
	char csv_str[70];
	sprintf(csv_str, "M;%d;%.2f%%\nS;%d;%.2f%%\nN;%d;%.2f%%\nA;%d;%.2f%%\nF;%d;%.2f%%", countM, ((float) countM/total)*100,
												    countS, ((float) countS/total)*100,
												    countN, ((float) countN/total)*100,
												    countA, ((float) countA/total)*100,
												    countF, ((float) countF/total)*100);

	int csv_file = creat("estadisticas.csv", 0644);

	if (write(csv_file, csv_str, strlen(csv_str)) == -1)
	{
		perror("Error writing to file");
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
alumno* fetch_alumno(const char* filename, int num_file) {

    int count_alumnos = 0;
    if (strlen(filename) == 0) {
        return NULL;
    }

    switch (num_file) {
        case 0: num_alumnos1 = count_alumnos; break;
        case 1: num_alumnos2 = count_alumnos; break;
        default: perror("Wrong parameter of argument on fetch_alumno\n"); break;
    }
}


/* Function: join_alumnos
 *
 * Arguments:
 *  - const alumno* alumnoarr1: First array of stundents
 *  - const alumno* alumnoarr2: Second array of students
 *
 * Returns:
 *  - Sorted array of the students from the two initial files
 */
alumno* join_alumnos(const alumno* alumnoarr1, const alumno* alumnoarr2)  {
    alumno alumnos[num_alumnos1 + num_alumnos2];
    int i, j;
    int k = 0;
    /* join both arrays of students into a single one*/
    for (i = 0; i < num_alumnos1; i++) {
        alumnos[k] = alumnoarr1[i];
        k++;
    }
    for (j = 0; j < num_alumnos2; j++) {
        alumnos[k] = alumnoarr2[j];
        k++;
    }
    /*We will use a Merge Sort algorithm for sorting the students by their marks in descending order*/
    int n = num_alumnos1 + num_alumnos2;
    merge_sort(alumnos, 0, n - 1);
    return alumnos;
}

void merge(alumno arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    alumno leftArr[n1], rightArr[n2];

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
void merge_sort(alumno arr[], int left, int right) {
    if (left < right) {
        /* Calculate midpoint */
        int mid = left + (right - left) / 2;
        /* sort first and second halves*/
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

/* Function: classify_alumnos
 *
 * Arguments:
 *  - const alumno* alumno: Array with the students to classify depending on their marks
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int classify_alumnos(const alumno* alumnos) {
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
 *  - const alumno* alumnos: Array with the students to output in the new file
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int output_new_data(const alumno* alumnos) {}
