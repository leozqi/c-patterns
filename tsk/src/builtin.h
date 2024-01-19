#ifndef TSK_BUILTIN_H
#define TSK_BUILTIN_H

/**
 * Built-in shell commands
 */
int tsk_cd(char **args);
int tsk_help(char **args);
int tsk_exit(char **args);

/**
 * Built-in shell command list
 */
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

#endif /* TSK_BUILTIN_H */
