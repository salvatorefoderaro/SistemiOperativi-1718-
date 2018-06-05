#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <sys/sysinfo.h>

#define CICLES 100


void *foo(int time) {
	while(time--);
}

int main(void) {
	pthread_attr_t attr;
	pthread_t tid;
	cpu_set_t cpumask;
	int numcpus, cicles = CICLES;
	
	
	if (pthread_attr_init(&attr) != 0) {
		printf("Failure in creating thread attributes object\n");
		exit(EXIT_FAILURE);
	}
	numcpus = get_nprocs_conf();
	CPU_ZERO(&cpumask);
	for (int i = 0; i < numcpus; i++) {
		CPU_SET(i, &cpumask);
		if (pthread_attr_setaffinity_np(&attr, (size_t) numcpus, &cpumask) != 0) {
			printf("Failure in setting cpu affinity\n");
			exit(EXIT_FAILURE);
		}
		if (pthread_create(&tid, &attr, (void *) foo, &cicles) != 0) {
			printf("Failure in creating new thread\n");
			exit(EXIT_FAILURE);
		}
		CPU_ZERO(&cpumask);
		pthread_join(tid, NULL);
	}
	exit(EXIT_SUCCESS);
}
	
		
	
	
	
	
