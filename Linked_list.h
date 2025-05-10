#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {
    int comp20003_code;
    int official_code_suburb;
    char official_name_suburb[128];
    int year;
    char official_code_s[128];
    char official_name_s[128];
    char official_code_l[128]; 
    char official_name_l[128]; 
    double latitude;
    double longitude;
    struct Record *next;
} Record;




Record* Insert(Record *head, Record new_record);
void Free_list(Record *head);
void Read_inputCSV(char *line, Record *record);
Record* Find_records(Record *head, FILE *output);
Record* delete_record(Record *head, FILE *output);
Record* SortRecord(Record* head);

#endif
