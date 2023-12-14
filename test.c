// Exam example.

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "LesData.h"

///< Const's
// #define used in global variables.
// const int used inside functions and main.
#define SMAXLEN 50
const int MAXLEN = 50;

// Struct server: servername, location, storages, serverID
struct server {
    char* serverName;
    char* location;
    int storages;
    int serverID;
};

// Struct data: server, numbers.
struct data {
    char* server;
    int n[5];
};

// Create a pointer to the last created data structure.
// Creates a count of how many data and server structs.
struct data* gdata[SMAXLEN];
struct server* gserv[SMAXLEN];
int glastdata = 0;
int glastserv = 0;

void readFile();
void cleanUp();
void cleanUpServ(struct server* s);
void cleanUpData(struct data* d);
void printFileServer(FILE *f, struct server* s);
void printFileData(FILE *f, struct data* d);
void output();
void outputServers(struct server* s);
void outputData(struct data* d, int i);
void new();
void newServ();
void newData();
void writeServ(struct server* s, char* name);
void writeData(struct data* d, char* name);
int findName(const char* name);
int findNameData(const char* name);
void del();


int main () {
    // Reading first servers and data into structs from file
    readFile();
    char command;

    do {
        command = lesChar("\n\nWrite command ");

        switch (command) {
            case 'N':
                new();
                break;
            case 'D':
                del();
                break;
            case 'C':
                //changeData();
                break;
            case 'W':
                output();
                break;
            default:
                if (command != 'Q') {
                    printf("\nWrong Command");
                }
                break;
        }
    } while(command != 'Q');

    // cleanup function that releases assigned data.
    cleanUp();

    return 0;
}

void readFile() {
    // Opens "server.txt" and "testdata.txt" in read mode
    FILE *inputFileServ = fopen("C:\\Users\\lunap\\CLionProjects\\C\\server.txt", "r");
    FILE *inputFileData = fopen("C:\\Users\\lunap\\CLionProjects\\C\\testdata.txt", "r");
    // Checks if inputFileServ || inputFileData != NULL, this ensures that they really got opened.
    if (!inputFileServ) {
        printf("\nServers could not be found");
        return;
    } else if (!inputFileData) {
        printf("\nData could not be opened");
        return;
    }

    printf("\nFiles opened successfully");
    // Gets first line from "server.txt" which tells how many servers there is.
    // Checks if first line is "%d" if not then it is not a number.
    // Puts this number int global variable glastserv.
    if (!fscanf(inputFileServ, "%d", &glastserv)) {
        printf("\nUnrecognized value in line 1");
        return;
    }

    // Catches the '\n' at the end of a string in a file, as the fscanf();
    // takes the string and converts it to int while leaving the '\n' at the end.
    getc(inputFileServ);

    // Creates 3 servers with a for loop using the printFileServer() function.
    for (int i = 0; i < glastserv; i++) {
        // Dynamically allocates space for the struct pointer.
        gserv[i] = (struct server*) malloc(sizeof(struct server));
        printFileServer(inputFileServ, gserv[i]);
    }

    // Temporary variables. dataLen = length of "testdata.txt"
    int dataLen = 0;
    char ch;
    // Goes through file to read every '\n' = newline which +1's the dataLen.
    while((ch = fgetc(inputFileData)) != EOF) {
        if (ch == '\n') {
            dataLen++;
        }
    }
    // +1 since the final line of the file doesn't end with '\n' but 'EOF'
    dataLen++;

    // Since the previous while loop set our positions to the bottom of the file
    // and as to not have to close that last file and open a new "testdata.txt"
    // we set our position to the top again using fseek() function.
    // SEEK_CUR = current position, SEEK_SET = TOF, SEEK_END = EOF.
    // fseek(file, move "x" bytes from:, Position), position = start, end or cur.
    fseek(inputFileData, 0, SEEK_SET);

    // Creates the data structs via the printFileData() function.
    printf("\nPrinting data to servers");
    for (int i = 0; i < dataLen/2; i++) {
        gdata[i] = (struct data*) malloc(sizeof(struct data));
        printFileData(inputFileData, gdata[i]);
    }
    printf("\n%d, Data printed.", dataLen/2);

    // Closes the files.
    fclose(inputFileData);
    fclose(inputFileServ);
}

void printFileServer(FILE *f, struct server* s) {
    char ph[MAXLEN];

    // Gets string from file, puts it in variable ph.
    // Changes last line in string from '\n' to '\0'.
    // Dynamically allocates space for s->serverName.
    // Copies ph into s->serverName.
    fgets(ph, MAXLEN, f);
    ph[strlen(ph)-1] = '\0';
    s->serverName = (char*) malloc((strlen(ph)+1) * sizeof(char));
    strcpy(s->serverName, ph);

    fgets(ph, MAXLEN, f);
    ph[strlen(ph)-1] = '\0';
    s->location = (char*) malloc((strlen(ph)+1) * sizeof(char));
    strcpy(s->location, ph);

    // Gets ID via fscanf, this leaves '\n' at the end of string
    // therefore getc(f); which gets the '\n'.
    fscanf(f,"%d",&s->serverID);
    getc(f);

    printf("\nServer : %s added to list\n", s->serverName);
    printf("\tID : %d", s->serverID);
}

void printFileData(FILE *f, struct data* d) {
    char ph[MAXLEN];

    // Gets the server name, same way as in printFileServer() function.
    fgets(ph, MAXLEN, f);
    ph[strlen(ph)-1] = '\0';
    d->server = (char*) malloc((strlen(ph)+1) * sizeof(char));
    strcpy(d->server, ph);

    // Gets line of numbers in form of string, and converts to ints.
    // fgetc(f); for the last '\n' that's left behind.
    fscanf(f, "%d %d %d %d %d", &d->n[0],&d->n[1],&d->n[2],&d->n[3],&d->n[4]);
    fgetc(f);

    // Since one more data struct added, glastdata++.
    glastdata++;
}

void cleanUp() {
    // Clears upp all assigned data.
    for (int i = 0; i < glastserv; i++) {
        // Clears up all malloc's in struct,
        cleanUpServ(gserv[i]);
        // Clear up Malloc for struct
        free(gserv[i]);
    }
    for (int i = 0; i < glastdata; i++) {
        cleanUpData(gdata[i]);
        free(gdata[i]);
    }
}

void cleanUpServ(struct server* s) {
    // Frees char pointers, since they are dynamically assigned space.
    s->serverID= 123123;
    free(s->serverName);
    free(s->location);
    glastserv--;
}

void cleanUpData(struct data* d) {
    // Frees char pointers, since they are dynamically assigned space.
    free(d->server);
    glastdata--;
}

void output() {
    // Outputs servers or data information.
    char command;
    do {
        command = lesChar("\n\nOutput data or serv (d/s/n)");
        switch (command) {
            case 'D':
                for (int i = 0; i < glastdata; i++) {
                    outputData(gdata[i], i);
                }
                break;
            case 'S':
                for (int i = 0; i < glastserv; i++) {
                    outputServers(gserv[i]);
                }
                break;
            case 'N':
                printf("Process Ended.");
                break;
            default:
                printf("\nNo command %c", command);
                break;
        }
    } while(command != 'N');
}

void outputServers(struct server* s) {
    // Writes servers, location and ID.
    printf("\nServer: %s\t location: %s\t ID: %d", s->serverName, s->location, s->serverID);
}

void outputData(struct data* d, int i) {
    // Writes the data's serverName and Data points.
    printf("\nServer name: %s, data number %i\n", d->server, i+1);

    for (int x = 0; x < 5; x++) {
        printf("Data point: %d\t", d->n[x]);
    }
}

void new() {
    // Create a new data struct or new server struct.
    char command;

    printf("\nNew Data or new Server: (d/s), cancel: (n)");
    do {
        command = lesChar("\n\nWrite command (d/s/n) ");

        switch (command) {
            case 'D':
                newData();
                break;
            case 'S':
                newServ();
                break;
            case 'N':
                printf("Process Ended.");
                break;
            default:
                printf("\nIllegal command %c", command);
                break;
        }
    } while (command != 'N');
}

int findName(const char* name) {
    // Goes through all server structs and compares 'name' to serverName
    for (int i = 0; i < glastserv; i++) {
        // Compares, strcmp compares two strings
        // strcmp returns 0 if two strings are alike.
        if (!strcmp(gserv[i]->serverName, name)) {
            return i;
        }
    }
    return -1;
}

int findNameData (const char* name) {
// Goes through all data structs and compares 'name' to server
    for (int i = 0; i < glastdata; i++) {
        // Compares, strcmp compares two strings
        // strcmp returns 0 if two strings are alike.
        if (!strcmp(gdata[i]->server, name)) {
            return i;
        }
    }
    return -1;
}

void newServ() {
    char name[MAXLEN];

    printf("\nCreating new Server.");
    lesText("\nWrite server name ", name, MAXLEN);

    // Checking if the input name is equal to one of the prior created ones.
    if (findName(name) != -1) {
        printf("\nServer \"%s\" Already Exists", name);
        return;
    }

    // Dynamically allocating space with malloc
    gserv[glastserv] = (struct server*) malloc(sizeof(struct server));
    writeServ(gserv[glastserv], name);

    // Created a new server, so the total is now +1
    glastserv++;
}

void writeServ(struct server* s, char* name) {
    // Dynamically allocates space for char*, and copies name to char*
    // done the same way as with printFileServer() function.
    s->serverName = (char*) malloc((strlen(name)+1) * sizeof(char));
    strcpy(s->serverName, name);

    lesText("\nWrite server location ", name, MAXLEN);
    s->location = (char*) malloc((strlen(name)+1)* sizeof(char));
    strcpy(s->location, name);

    // ID has to be 6 figures long so id has to be inbetween
    // 100000 and 999999
    int id;
    do {
        id = lesInt("\nWrite ID (6 numbers) ", 100000, 999999);
    } while (id < 100000 || id > 999999);

    // No need to allocate space for ID as it is not a pointer.
    s->serverID = id;
}

void newData() {
    char servName[MAXLEN];
    printf("\nCreating new Data.");
    printf("\nData must have existing server to store it in.");

    // Checking if data's server exists if not, return.
    lesText("\nWrite ServerName ", servName, MAXLEN);
    if (findName(servName)) {
        printf("\nNo current server named %s", servName);
        return;
    }

    // Same as in writeServ() function.
    gdata[glastdata] = (struct data*) malloc(sizeof(struct data));
    writeData(gdata[glastdata], servName);

    // Created a new data, so the total is now +1
    glastdata++;
}

void writeData(struct data* d, char* name) {
    // Same as in writeServ.
    d->server = (char *) malloc((strlen(name)+1) * sizeof(char));
    strcpy(d->server, name);

    // Asks 5 times to input a data point (int) from 0 to 999999.
    // No need to allocate space as it is not a pointer.
    printf("\nWrite 5 data points (int),0 -> 999999");
    for (int i = 0; i < 5; i++) {
        d->n[i] = lesInt("\nWrite Data ", 0, 999999);
    }
}

void del() {
    char name[MAXLEN];
    char command;
    int num;
    int nr;
    int tot = 0;

    do {
        command = lesChar("\nDelete server or data (d/s/n)");

        switch (command) {
            case 'D':
                num = lesInt("\nWhich data ", 0, glastdata);
                nr = num;
                for (int i = (num-glastdata); i < 0; i++) {
                    gdata[nr-1] = gdata[nr];
                    nr++;
                }
                cleanUpData(gdata[glastdata-1]);
                free(gdata[glastdata]);
                printf("\nDeleted data num: %d", num);
                break;
            case 'S':
                printf("\n!All data related to server will also be deleted!");
                lesText("\nWrite server name ", name, MAXLEN);

                int num = findName(name);
                if (num == -1) {
                    printf("\nServer \"%s\" Doesn't Exists", name);
                    return;
                }

                cleanUpServ(gserv[num]);
                free(gserv[num]);

                for (int i = num; i < glastdata; i++) {
                    gserv[i] = gserv[i+1];
                }

                int numIndex;
                while ((numIndex = findNameData(name)) != -1) {
                    cleanUpData(gdata[numIndex]);
                    free(gdata[numIndex]);

                    for (int i = numIndex; i < glastdata-1; i++) {
                        gdata[i] = gdata[i+1];
                    }
                }

                printf("\t3");
                printf("\nDeleted serv num: %s", name);
                printf("\nAnd %d data associated to %s", tot, name);
                break;
            case 'N':
                printf("Process Ended.");
                break;
            default:
                printf("\nIllegal command %c", command);
                break;
        }

    } while (command != 'N');
}