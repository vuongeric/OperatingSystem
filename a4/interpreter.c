#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "shellmemory.h"
#include "shell.h"
#include "kernel.h"
#include "memorymanager.h"
#include "DISK_driver.h"
#include "IOSCHEDULER.h"

int run(char *filename) {
	FILE *ptr;
	char buffer[1000], buf0[100], buf1[100], buf2[100];
	int result = 0;

	ptr = fopen(filename,"rt");
	if (ptr == NULL) return 4; // file not found

	fgets(buffer,999,ptr);
	while(!feof(ptr)) {
		if (strlen(buffer)>1) result = prompt(buffer);
		if (result == 99) break;
		fgets(buffer,999,ptr);
	}

	fclose(ptr);

	if (result == 99) return 99;

	return 0;
}

int exec(char *file1, char *file2, char *file3) {
	int result = 0;
	FILE *p1, *p2, *p3;

	if (strlen(file1)<1) file1 = NULL;
	if (strlen(file2)<1) file2 = NULL;
	if (strlen(file3)<1) file3 = NULL;

	// Determine if there are duplicates and remove
	// if (file1!=NULL && file2!=NULL && strcmp(file1,file2)==0) file2=NULL;
	// if (file1!=NULL && file3!=NULL && strcmp(file1,file3)==0) file3=NULL;
	// if (file2!=NULL && file3!=NULL && strcmp(file2,file3)==0) file3=NULL;

	printf("f1:%s f2:%s f3:%s\n", file1, file2, file3);

	// Add to ready queue each unique programs
	if (file1!=NULL) {
		p1 = fopen(file1,"rt");
		// result = myinit(p1);
		result = launcher(p1, "P1.txt");
	}
	if (file2!=NULL) {
		p2 = fopen(file2,"rt");
		// result = myinit(p2);
		result = launcher(p2, "P2.txt");
	}
	if (file3!=NULL) {
		p3 = fopen(file3,"rt");
		// result = myinit(p3);
		result = launcher(p3, "P3.txt");
	}

	if (result > 0) {
		scheduler();
	}

	return 0;
}

int interpreter(char buf0[], char buf1[], char buf2[], char buf3[]) {
	int result = 0; // no errors

	if (strcmp(buf0,"help")==0) {
		printf("Legal commands:\n");
		printf("help              display this help\n");
		printf("quit              exits the shell\n");
		printf("set VAR STRING    assign STRING to VAR\n");
		printf("print VAR         display contents of VAR\n");
		printf("run SCRIPT.TXT    interpret SCRIPT.TXT\n");
		printf("exec P1 P2 P2     can run up to 3 distinct programs\n");
		printf("Mount number_of_blocks block_size	mount partition\n");
		printf("Write filename [a bunch of words]	write to a file\n");
		printf("Read filename variable	read a file from beginning\n");

		result = 0;
	}
	else if (strcmp(buf0,"quit")==0) {
		printf("Bye.\n");
		result = 99; // exit shell code
	}
	else if (strcmp(buf0,"set")==0) {
		if (strlen(buf1)<1 || strlen(buf2)<1) return 1; // set error

		add(strdup(buf1), strdup(buf2));
	}
	else if (strcmp(buf0,"print")==0) {
		if (strlen(buf1)<1) return 2; // print error

		printf("%s\n", get(buf1));
	}
	else if (strcmp(buf0,"run")==0) {
		if (strlen(buf1)<1) return 3; // run error

		result = run(buf1);
	}
	else if (strcmp(buf0,"exec")==0) {
		if (strlen(buf1)<1) return 5; // exec error

		result = exec(buf1, buf2, buf3);
	}
	else if (strcmp(buf0, "Mount")==0) {
		if (strlen(buf1)<1 || strlen(buf2)<1 || strlen(buf3)<1) return 6; // Mount error
		int blockSize = atoi(buf2);
		int blockLength = atoi(buf3);
		if (blockSize == 0 || blockLength == 0) {
			return 6; // mount error
		}
		result = partition(buf1, blockSize, blockLength);
		if(!result) return 1;
		result = mount(buf1);
		if(result) result = 0;
	}
	else if (strcmp(buf0, "Write")==0) {
		if(strlen(buf1)<1 || strlen(buf2)<1) return 7; // Write error
		int file = openfile(strdup(buf1));
		fileToUse = file;
		IOscheduler(strdup(buf2), NULL, 1);
		//result = writeBlock(file, strdup(buf2));
		if(result) result = 0;
	}
	else if (strcmp(buf0, "Read")==0) {
		if (strlen(buf1)<1 || strlen(buf2)<1) return 8; // Read error
		int file = openfile(strdup(buf1));
		fileToUse = file;
		char * data = IOscheduler(NULL, NULL, 0);
		add(strdup(buf2), strdup(data));
		result = 0;
	}
	else {
		result = 98; // command does not exist
	}

	return result;
}

