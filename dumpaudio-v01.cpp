#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include <chrono>


using namespace std;

const int MAX = 10*8000*2;
unsigned char buffer[MAX];


int read = 0;
int min_read = 3*8000*2;  // seconds * rate * float16_size
int max_read = MAX;
int eof_received = false;


bool continue_reading() {
    if (read < min_read) return true;
    if (read >= MAX) return false;
    if (eof_received) return false;
    return true;
}


char filename[100];

int main()
{

    using namespace std::chrono;
    milliseconds ms = duration_cast< milliseconds >(
            system_clock::now().time_since_epoch()
            );


    sprintf(filename, "/root/%lld.audio-raw.dump", ms.count());

    FILE *file = fdopen(3, "rb");
    FILE *file_out = fopen(filename, "wb");
    printf("Output file name: %s\n", filename);

    int last_printed = 0;
    while (continue_reading()) {
        int cur_read = fread(buffer + read, 1, 1, file);
        eof_received = cur_read == 0;
        if (not eof_received) {
            read++;
        }
        if (read % 100 == 0) {
            fwrite(buffer + last_printed, read - last_printed, 1, file_out);
            last_printed = read;
            fflush(file_out);
        }
    }
    fwrite(buffer + last_printed, read - last_printed, 1, file_out);
    last_printed = read;

    printf("I've read %d bytes\n", read);
    fclose(file);
    fclose(file_out);


    /*

    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    string s = "";

    while((ch = fgetc(fp)) != EOF) s += ch;
    printf("%d\n", s.size());
*/
    return 0;
}
