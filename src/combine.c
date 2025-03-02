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
alumno_t* join_alumnos(const alumno_t alumnoarr1[], const alumno_t alumnoarr2[], alumno_t sorted_alumnos[]);
int classify_alumnos(const alumno_t alumnos[]);
int output_new_data(alumno_t alumnos[], const char* filename);


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

    // Join the two student arrays into a sorted one depending on the notes
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
 * Creates the CSV file using the data from the marks from the arguments.
 *
 * Arguments:
 *  - const int countM: Number of students with a score of 10
 *  - const int countS: Number of students with a score of 9
 *  - const int countN: Number of students with a score of 8 or 7
 *  - const int countA: Number of students with a score of 6 or 5
 *  - const int countF: Number of students with a score lower than 5
 *
 * Returns:
 *  - -1 in case of error, 0 otherwise
 */
int create_csv(const int countM, const int countS, const int countN, const int countA, const int countF) {

    int total = countM+countS+countN+countA+countF;

    // Store into an array the string to be outputed to the CSV file
    char csv_str[70];
    sprintf(csv_str,
        "M;%d;%.2f%%\nS;%d;%.2f%%\nN;%d;%.2f%%\nA;%d;%.2f%%\nF;%d;%.2f%%\n",
                countM, (((float) countM) / ((float) total))*100,
	        countS, (((float) countS) / ((float) total))*100,
	        countN, (((float) countN) / ((float) total))*100,
	        countA, (((float) countA) / ((float) total))*100,
	        countF, (((float) countF) / ((float) total))*100);


    // Create the CSV file and store the information
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
 * Fetches the data from the file passed as argument and stores it in the array,
 * while also updating the corresponding global variable.
 *
 * Arguments:
 *  - const char* filename: Name of the file where the data is stored
 *  - alumno_t alumnos[]: Array where to store the data found
 *  - int num_file: Respective global variable to store the total count
 *
 * Returns:
 *  - Array with the students found in the file
 */
alumno_t* fetch_alumno(const char* filename, alumno_t alumnos[], int num_file) {

    // If no filename provided, return nothing
    if (strlen(filename) == 0) {return NULL;}

    // Open file on read only mode
    int file_fd;
    if ((file_fd = open(filename, O_RDONLY)) < 0) {
        perror("Error opening file");
        exit(-1);
    }

    // Traverse the whole file, storing in the array the data from the students
    int count_alumnos = 0;
    while (read(file_fd, &alumnos[count_alumnos], sizeof(alumno_t)) != -1 &&
            count_alumnos < MAX_ALUMNOS) {

        if (strcmp(alumnos[count_alumnos].nombre, "") == 0) {
            break;
        }

        // Check if the input is correctly formatted
        if (strlen(alumnos[count_alumnos].nombre) <= 0 || strlen(alumnos[count_alumnos].nombre) > 50 ||
            alumnos[count_alumnos].nota < 0 || alumnos[count_alumnos].nota > 10 ||
            alumnos[count_alumnos].convocatoria < 0) {

            perror("Invalid format inside input file");
            exit(-1);
        }

        count_alumnos++;
    }

    // If no students found, return nothing
    if (count_alumnos == 0) {return NULL;}

    // Depending on which file we are searching, update the corresponding global variable
    switch (num_file) {
        case 0: num_alumnos1 = count_alumnos; break;
        case 1: num_alumnos2 = count_alumnos; break;
        default: perror("Wrong parameter of argument on fetch_alumno"); break;
    }

    return alumnos;
}


/* Function: join_alumnos
 *
 * Joins the two arrays of students and sorts them depending on their notes.
 *
 * Arguments:
 *  - const alumno_t* alumnoarr1: First array of students
 *  - const alumno_t* alumnoarr2: Second array of students
 *  - alumno_t sorted_alumnos[]: Where to store the computed sorted array
 *
 * Returns:
 *  - Sorted array of the students from the two initial files
 */
alumno_t* join_alumnos(const alumno_t alumnoarr1[], const alumno_t alumnoarr2[], alumno_t sorted_alumnos[])  {

    // Join both arrays of students into a single one
    if (alumnoarr1 != NULL) {
        for (int i = 0; i < num_alumnos1; i++) {
            sorted_alumnos[i] = alumnoarr1[i];
        }
    }
    if (alumnoarr2 != NULL) {
        for (int i = num_alumnos1; i < num_alumnos1 + num_alumnos2; i++) {
            sorted_alumnos[i] = alumnoarr2[i - num_alumnos1];
        }
    }


    int total_num_alumnos = num_alumnos1 + num_alumnos2;
    // Bubble sort algorithm to sort students depending on their notes
    for (int i = 0; i < total_num_alumnos; i++) {
        u_int8_t swapped = 0;
        for (int j = 0; j < total_num_alumnos - i - 1; j++) {
            if (sorted_alumnos[j].nota > sorted_alumnos[j+1].nota) {
                // Interchange variables
                alumno_t temp_alumno = sorted_alumnos[j];
                sorted_alumnos[j] = sorted_alumnos[j + 1];
                sorted_alumnos[j + 1] = temp_alumno;

                swapped = 1;
            }
        }

        // If no two elements were swapped by inner loop, then break
        if (swapped == 0){break;}
    }

    return sorted_alumnos;
}


/* Function: classify_alumnos
 *
 * Prepares the data from the array of students to laters pass on the create_csv function.
 *
 * Arguments:
 *  - const alumno_t* alumno_t: Array with the students to classify depending on their marks
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int classify_alumnos(const alumno_t alumnos[]) {

    int count_M = 0, count_S = 0, count_N = 0, count_A = 0, count_F = 0;

    int size = num_alumnos1 + num_alumnos2;
    if (size == 0) {
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
 * Creates the file passed as argument and populates it with the array of students.
 *
 * Arguments:
 *  - const alumno_t alumnos[]: Array with the students to output in the new file
 *  - const char* filename: Name of the file where to output the new data
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int output_new_data(alumno_t alumnos[], const char* filename) {

    int new_file_fd;
    if ((new_file_fd = creat(filename, 0644)) < 0) {
        perror("Error creating new file");
        exit(-1);
    }

    int total_number_alumnos = num_alumnos1 + num_alumnos2;
    for (int i = 0; i < total_number_alumnos; i++) {
        if (write(new_file_fd, &alumnos[i], sizeof(alumno_t)) < 0) {
            perror("Error writing to file");
            exit(-1);
        }
    }

    close(new_file_fd);
    return 0;
}

/*
* ░▄▀▄▀▀▀▀▄▀▄░░░░░░░░░
* ░█░░░░░░░░▀▄░░░░░░▄░
* █░░▀░░▀░░░░░▀▄▄░░█░█
* █░▄░█▀░▄░░░░░░░▀▀░░█
* █░░▀▀▀▀░░░░░░░░░░░░█
* █░░░░░░░░░░░░░░░░░░█
* █░░░░░░░░░░░░░░░░░░█
* ░█░░▄▄░░▄▄▄▄░░▄▄░░█░
* ░█░▄▀█░▄▀░░█░▄▀█░▄▀░
* ░░▀░░░▀░░░░░▀░░░▀░░░
*/