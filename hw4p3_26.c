#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int main(int argc, char* argv[]){
    int p1[2]; // C => P
    int p2[2]; // P => C

    if (pipe(p1) == -1){
        return 1;
    }

    if (pipe(p2) == -1){
        return 1;
    }

    int pid = fork();

    if (pid == -1){
        return 2;
    }

    if (pid == 0) { // Child Process
        close(p1[0]);
	close(p2[1]);

	char text[10];
        
	if (read(p2[0], &text, sizeof(text)) == -1) {
            return 3;
        }

        printf("Child received %s\n", text);

        for(int i = 0; i < sizeof(text)/sizeof(text[0]); i++){
            if (isupper(text[i])) {
                text[i] = tolower(text[i]);
                continue;
            }

            if (islower(text[i])) {
                text[i] = toupper(text[i]);
                continue;
            }
        }

        if (write(p1[1], &text, sizeof(text)) == -1) {
            return 4;
        }

        printf("Child wrote %s\n", text);

	close(p1[1]);
        close(p2[0]);
    } else { // Parent Process
        close(p1[1]);
	close(p2[0]);
	srand(time(NULL));
	
        char text[] = "Hi There";

        if(write(p2[1], &text, sizeof(text)) == -1) {
            return 5;
        }

        printf("Parent wrote %s\n", text);

        if(read(p1[0], &text, sizeof(text)) == -1) {
            return 6;
        }

        printf("Result in the parent is %s\n", text);
	close(p1[0]);
	close(p2[1]);
	wait(NULL);
    }

    return 0;
}
