#include "Linked_list.h"

Record* Insert(Record* head, Record new_record_data) {
    Record *new_node = (Record *)malloc(sizeof(Record));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return head;
    }
    *new_node = new_record_data;
    new_node->next = head;
    return new_node;
}

void Free_list(Record *head) {
    Record *current = head;
    Record *next;

    while (current != NULL) {
        next = current->next;
        free(current);  // Release the memory of the current node
        current = next;
    }
}

Record* Find_records(Record *head, FILE *output) {
    char suburb_to_search[100];

    // Sorting a linked list
    head = SortRecord(head);

    while (1) {    
        if (fgets(suburb_to_search, sizeof(suburb_to_search), stdin) == NULL) {
            break;  // if EOF, break
        }

        suburb_to_search[strcspn(suburb_to_search, "\n")] = 0;

        if (strlen(suburb_to_search) == 0) {
            break;
        }

        Record *current = head;
        int found = 0;

        fprintf(output,"%s -->\n", suburb_to_search);

        // Traverse the linked list to search and print
        while (current != NULL) {
            if (strcmp(current->official_name_suburb, suburb_to_search) == 0) {
                fprintf(output, "COMP20003 Code: %d, Official Code Suburb: %d, Official Name Suburb: %s, Year: %d, Official Code State: %s, Official Name State: %s, Official Code Local Government Area: %s, Official Name Local Government Area: %s, Latitude: %.7f, Longitude: %.7f\n",
                        current->comp20003_code,
                        current->official_code_suburb,
                        current->official_name_suburb,
                        current->year,
                        current->official_code_s,
                        current->official_name_s,
                        current->official_code_l,
                        current->official_name_l,
                        current->latitude,
                        current->longitude);
                found++;
            }
            current = current->next;
        }

        // not found situation
        if (found > 0) {
            printf("%s --> %d records found\n", suburb_to_search, found);
        } else {
            printf("%s --> NOTFOUND\n", suburb_to_search);
        }
    }
    return head;
}

void Read_inputCSV(char *line, Record *record) {
    //Initialize lga, because it may store multiple value attributes
    memset(record->official_code_l, 0, sizeof(record->official_code_l));
    memset(record->official_name_l, 0, sizeof(record->official_name_l));

    // Use sscanf to read the first few fields, but leave the LGA and lat/lon parts
    sscanf(line, "%d,%d,%[^,],%d,%[^,],%[^,]",  
           &record->comp20003_code,
           &record->official_code_suburb,
           record->official_name_suburb,
           &record->year,
           record->official_code_s,
           record->official_name_s);

    char *lga_code_start = strchr(line, '"');
    char *lga_code_end = NULL;
    char *lga_name_start = NULL;
    char *lga_name_end = NULL;

    if (lga_code_start) {
        lga_code_start++; // Skip the first quotation mark

        // Find the second quotation mark, which indicates the end of the LGA code section
        lga_code_end = strchr(lga_code_start, '"');
        if (lga_code_end) {
            *lga_code_end = '\0'; 
            strcpy(record->official_code_l, lga_code_start);
        }

        // Find the third quotation mark, which indicates the beginning of the LGA name portion
        lga_name_start = strchr(lga_code_end + 1, '"');
        if (lga_name_start) {
            lga_name_start++; 

            // Find the fourth quotation mark, which indicates the end of the LGA name portion
            lga_name_end = strchr(lga_name_start, '"');
            if (lga_name_end) {
                *lga_name_end = '\0'; 
                strcpy(record->official_name_l, lga_name_start);
            }
        }
        sscanf(lga_name_end + 2, "%lf,%lf", &record->latitude, &record->longitude);
    } else {
        // Handling without double quotes
        char *rest_of_line = strchr(line, ',');
        for (int i = 0; i < 5; i++) {
            rest_of_line = strchr(rest_of_line + 1, ','); 
        }
        sscanf(rest_of_line + 1, "%[^,],%[^,],%lf,%lf",
               record->official_code_l,
               record->official_name_l,
               &record->latitude,
               &record->longitude);
    }

}

Record* SortRecord(Record* head) {
    if (!head || !head->next) {
        return head; 
    }

    Record* sorted = NULL; 
    Record* current = head;
    while (current) {
        Record* next = current->next; 
        if (!sorted || sorted->comp20003_code > current->comp20003_code) {
            current->next = sorted;
            sorted = current;
        } else {
            Record* temp = sorted;
            while (temp->next && temp->next->comp20003_code <= current->comp20003_code) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }

    return sorted; 
}

Record* delete_record(Record *head, FILE *output) {
    char suburb_to_delete[100];

    head = SortRecord(head);

    while (1) {
        if (fgets(suburb_to_delete, sizeof(suburb_to_delete), stdin) == NULL) {
            break;  
        }

        suburb_to_delete[strcspn(suburb_to_delete, "\n")] = 0;  

        if (strlen(suburb_to_delete) == 0) {
            break; 
        }

        Record *current = head;
        Record *prev = NULL;
        int found = 0;

        while (current != NULL) {
            if (strcmp(current->official_name_suburb, suburb_to_delete) == 0) {
                if (prev == NULL) {
                    head = current->next; 
                } else {
                    prev->next = current->next;
                }

                Record *to_free = current;
                current = current->next;
                free(to_free);
                found++;
            } else {
                prev = current;
                current = current->next;
            }
        }

        if (found == 0) {
            printf("%s --> NOTFOUND\n", suburb_to_delete);
        } else {
            printf("%s --> %d records deleted\n", suburb_to_delete, found);
        }
    }

    Record *current = head;
    fprintf(output, "COMP20003 Code,Official Code Suburb,Official Name Suburb,Year,Official Code State,Official Name State,Official Code Local Government Area,Official Name Local Government Area,Latitude,Longitude\n");

    while (current != NULL) {
        char official_code_l[256];
        char official_name_l[256];

        if (strchr(current->official_code_l, ',') != NULL) {
            snprintf(official_code_l, sizeof(official_code_l), "\"%s\"", current->official_code_l);
        } else {
            strncpy(official_code_l, current->official_code_l, sizeof(official_code_l));
        }

        if (strchr(current->official_name_l, ',') != NULL) {
            snprintf(official_name_l, sizeof(official_name_l), "\"%s\"", current->official_name_l);
        } else {
            strncpy(official_name_l, current->official_name_l, sizeof(official_name_l));
        }

        fprintf(output, "%d,%d,%s,%d,%s,%s,%s,%s,%.7f,%.7f\n",
                current->comp20003_code,
                current->official_code_suburb,
                current->official_name_suburb,
                current->year,
                current->official_code_s,
                current->official_name_s,
                official_code_l,
                official_name_l,
                current->latitude,
                current->longitude);
        current = current->next;
    }

    return head; 
}


