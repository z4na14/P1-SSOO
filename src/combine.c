#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct{
	char nombre[50];
	int nota;
	int convocatoria;
} alumno;

int create_csv(int countM, int countS, int countN, int countA, int countF);
alumno* fetch_alumno(char* filename);
alumno* join_alumnos(alumno* alumnoarr1, alumno* alumnoarr2, int argc1, int argc2);
int classify_alumnos(alumno* alumnos);
int output_new_data(alumno* alumnos, char* filename);
void merge(alumno arr[], int left, int mid, int right);
void merge_sort(alumno arr[], int left, int right);


int main(const int argc, const char *argv[]){

        // Check if the correct amount of parameters is passed to the program
	if (argc != 3)
	{
            perror("Wrong number of arguments\n");
	    return -1;
	}

        // Fetch stundents and join them in a sorted array
	alumno* sorted_alumnos = join_alumnos(fetch_alumno(argv[1]), fetch_alumno(argv[2]));

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
 *  - const int countN: Number of students with a score of 8 or 7
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
alumno* fetch_alumno(const char* filename) {}


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
  alumno* alumnos[sizeof(alumnoarr1) + sizeof(alumnoarr2)];
  
  int i, j;
  int k = 0;
  for (i = 0; i < num_alumnos1; i++) {
    alumnos[k] = alumnoarr1[i];
    k++;
  }
  for (j = 0; j < )
}


int classify_alumnos(const alumno* alumnos) {}


/* Function: create_csv
 *
 * Arguments:
 *  - const alumno* alumnos: Array with the students to output in the new file
 *
 * Returns:
 *  - Will always return 0, in case of error the program will be terminated with -1
 */
int output_new_data(const alumno* alumnos) {}
