
struct rusage
{
	int ru_utime;  // Time spent in user mode
	int ru_stime;  // Time spent in kernel mode
	int ru_maxrss; // Maximum resident set size
};