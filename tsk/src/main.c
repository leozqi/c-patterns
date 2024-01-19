// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * TSK Shell.
 *
 * (C) 2022 leozqi
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pwd.h>

void tsk_err_alloc(void)
{
	fprintf(stderr, "tsk: allocation error\n");
	exit(EXIT_FAILURE);
}

char *tsk_read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;

	if (getline(&line, &bufsize, stdin) == -1) {
		if (feof(stdin)) {
			exit(EXIT_SUCCESS);
		} else {
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}


#define TSK_TOK_BUFSIZE 64
#define TSK_TOK_DELIM " \t\r\n\a"
char **tsk_split_line(char *line)
{
	int bufsize = TSK_TOK_BUFSIZE;
	int pos = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) tsk_err_alloc();

	token = strtok(line, TSK_TOK_DELIM);
	while (token != NULL) {
		tokens[pos] = token;
		pos++;

		if (pos >= bufsize) {
			bufsize += TSK_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) tsk_err_alloc();
		}

		token = strtok(NULL, TSK_TOK_DELIM);
	}

	tokens[pos] = NULL;
	return tokens;
}


int tsk_launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("tsk");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("tsk");
	} else {
		// Parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}


/* Builtin shell commands */
int tsk_cd(char **args);
int tsk_help(char **args);
int tsk_exit(char **args);

/* Builtin commands list */
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&tsk_cd,
	&tsk_help,
	&tsk_exit
};

int tsk_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

/* Builtin function implementations. */
int tsk_cd(char **args)
{
	if (args[1] == NULL) {
		fprintf(stderr, "tsk: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("tsk");
		}
	}

	return 1;
}

int tsk_help(char **args)
{
	int i;
	printf("Leo Qi's TSK\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < tsk_num_builtins(); i++) {
		printf("	%s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int tsk_exit(char **args)
{
	return 0;
}


int tsk_execute(char **args)
{
	if (args[0] == NULL) {
		// empty command entered
		return 1;
	}

	for (int i = 0; i < tsk_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return tsk_launch(args);
}


char *tsk_get_username(void)
{
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	if (pw) {
		return pw->pw_name;
	} else {
		return "";
	}
}

#define TSK_GP_BUFSIZE 1024
char *tsk_get_prompt(void)
{
	int bufsize = TSK_GP_BUFSIZE;
	char *buffer = malloc(sizeof(char) * bufsize);
	char hostname[bufsize];
	char cwd[bufsize];

	gethostname(hostname, bufsize);
	getcwd(cwd, bufsize);

	snprintf(buffer, bufsize, "\033[1;32m%s@%s\033[0m:\033[1;34m%s\033[0m$ ",
		tsk_get_username(),
		hostname,
		cwd);

	return buffer;
}


void tsk_loop(void)
{
	char *prompt;
	char *line;
	char **args;
	int status;

	do {
		printf("%s", tsk_get_prompt());

		line = tsk_read_line();
		args = tsk_split_line(line);
		status = tsk_execute(args);

		free(line);
		free(args);
	} while (status);
}


int main(int argc, char **argv)
{
	// Load config files, if any

	// Run command loop.
	tsk_loop();

	// Perform shutdown/cleanup
	
	return EXIT_SUCCESS;
}
