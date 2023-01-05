#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* loadstr(FILE * file)
{
    char *aux;
    char a;
	fread(&a, sizeof(char), 1, file);
    int cont = 1;
	while (a != '\0')
	{
		cont++;
		fread(&a, sizeof(char), 1, file);
	}
	aux = malloc(cont);
	fseek(file, -cont, SEEK_CUR);
	fread(aux, sizeof(char), cont, file);
	return aux;
}

student_t* parse_records(char* records[], int nr_records)
{
    student_t* aux = (student_t*)malloc(nr_records * sizeof(student_t));
    int i = 0;
    char* strAux;
    for(;i < nr_records; ++i){        
        strAux = strsep(&records[i], ":");
        aux[i].student_id = atoi(strAux);

        strAux = strsep(&records[i], ":");
        strcpy(aux[i].NIF, strAux);

        strAux = strsep(&records[i], ":");
        aux[i].first_name = strAux;

        strAux = strsep(&records[i], ":");
        aux[i].last_name = strAux;
    }
    return aux;
}

int dump_entries(student_t* entries, int nr_entries, FILE* students)
{
    int i = 0;
    for(;i < nr_entries; ++i){
        if(fwrite(&entries[i].student_id, sizeof(int), 1, students) == 0 ||
        fwrite(entries[i].NIF, strlen(entries[i].NIF) + 1, 1, students) == 0 ||
        fwrite(entries[i].first_name, strlen(entries[i].first_name) + 1, 1, students) == 0 ||
        fwrite(entries[i].last_name, strlen(entries[i].last_name) + 1, 1, students) == 0){
            perror("Write failed\n");
            return -1;
        }
    }
    return 0;
}

student_t* read_student_file(FILE* students, int* nr_entries)
{
    fread(nr_entries, sizeof(int), 1, students);
    student_t* aux = (student_t *)malloc(*nr_entries * sizeof(student_t));
    int i = 0;
    for(;i < *nr_entries; ++i){
        fread(&aux[i].student_id, sizeof(int), 1, students);
        strcpy(aux[i].NIF, loadstr(students));
        aux[i].first_name = loadstr(students);
        aux[i].last_name = loadstr(students);
    }
    return aux;
}

int main(int argc, char *argv[])
{
    FILE* file;
    char *fileName;

    int opt;
    while((opt = getopt(argc, argv, "f:hlc:a:qi:n:")) != -1) {
        if(opt == 'f')
            fileName = optarg;
        else if(opt == 'l'){
            if((file = fopen(fileName, "r")) == NULL){
                perror("Open failed\n");
                return -1;
            }

            int n = 0;
            student_t *students = read_student_file(file, &n);

            int i = 0;
            for(;i < n; ++i)
                printf("[Entry#%d]\nstudent_id=%d\nNIF=%s\nfirst_name=%s\nlast_name=%s\n",
                i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);

            free(students);
            free(file);
        }
        else if(opt == 'c'){
            if((file = fopen(fileName, "w")) == NULL){
                perror("Open failed\n");
                return -1;
            }

            int recordsToAdd = argc - (optind - 1);
            char** recordsInfo = malloc(recordsToAdd * sizeof(char*));

            int i = 0;
            for(;i < recordsToAdd; ++i)
                recordsInfo[i] = argv[i + optind - 1];

            student_t* students = parse_records(recordsInfo, recordsToAdd);

            fwrite(&recordsToAdd, sizeof(int), 1, file);
            dump_entries(students, recordsToAdd, file);

            printf("%d records written succesfully\n", recordsToAdd);

            free(students);
            free(recordsInfo);
            fclose(file);
        }
        else if(opt == 'a'){
            if((file = fopen(fileName, "r+")) == NULL){
                perror("Open failed\n");
                return -1;
            }

            int studentCount = 0;
            student_t* students =read_student_file(file, &studentCount);

            int recordsToAdd = argc - (optind - 1);
            char** recordsInfo = malloc(recordsToAdd * sizeof(char*));

            int i = 0;
            for(;i < recordsToAdd; ++i)
                recordsInfo[i] = argv[i + optind - 1];

            student_t* newStudents = parse_records(recordsInfo, recordsToAdd);
            int recordsAdded = 0;
            i = 0;
            int j;
            for(;i < recordsToAdd; ++i){
                j = 0;
                while(j < studentCount && newStudents[i].student_id != students[j].student_id)
                    j++;

                if(j == studentCount){
                    recordsAdded++;
                    dump_entries(&newStudents[i], 1, file);
                }
                else
                    printf("Found duplicate student_id %d\n", newStudents[i].student_id);
            }

            printf("%d records written succesfully\n", recordsAdded);

            fseek(file, 0, SEEK_SET);
            studentCount += recordsAdded;
            fwrite(&studentCount, sizeof(int), 1, file);

            free(students);
            free(newStudents);
            fclose(file);
        }
        else if(opt == 'q'){
            if((file = fopen(fileName, "r")) == NULL){
                perror("Open failed\n");
                return -1;
            }
        }
        else if(getopt(argc, argv, "i") != -1){
            int id;
            sscanf(optarg, "%d", &id);

            int studentCount = 0;
            student_t* students = read_student_file(file, &studentCount);
            int i = 0;                

            while(i < studentCount && id != students[i].student_id)
                i++;

            if(i == studentCount)
                printf("No entry was found\n");
            
            else
                printf("[Entry #%d]\nstudent_id=%d\nNIF=%s\nfirst_name=%s\nlast_name=%s\n",
                    i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);
        
            fclose(file);
            free(students);
        }
        else if(getopt(argc, argv, "n")){
            char* NIF = optarg;

            int studentCount = 0;
            student_t* students = read_student_file(file, &studentCount);
            int i = 0;

            while(i < studentCount && NIF != students[i].NIF)
                i++;

            if(i == studentCount)
                printf("No entry was found\n");

            else
                printf("[Entry #%d]\nstudent_id=%d\nNIF=%s\nfirst_name=%s\nlast_name=%s\n",
                    i, students[i].student_id, students[i].NIF, students[i].first_name, students[i].last_name);
        
            fclose(file);
            free(students);
        }
        else if(opt == 'h'){
            printf("Usage: ./student-records -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n");
        }
    }
    return 0;
}
