#define _GNU_SOURCE

/* These libraries are necessary for the hook */
#include <dlfcn.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>

/* "Injected" stuff */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/ipc.h>

/*

   	A simple program for hooking glXSwapBubffers() in OpenGL for calculating FPS

	Author: Kyungmin Bin
	E-mail: kmbin@unist.ac.kr

*/

struct timeval lastTime = {};
struct timeval currentTime;
int nbframes = 0;


// Calculate FPS
void CalculateFrameRate() {

	int fd;
	double *dest = NULL;
	int flag = PROT_WRITE | PROT_READ;
	
    gettimeofday(&currentTime, NULL);
	long int current_ms = currentTime.tv_sec * 1000 + currentTime.tv_usec / 1000;
	long int last_ms = lastTime.tv_sec * 1000 + lastTime.tv_usec / 1000;
	long int delta_ms = current_ms - last_ms;
	
	nbframes++;
	double fps = 0.0;

	if(delta_ms > 1000) {

		fps = (double)nbframes/((double)delta_ms/1000);
		printf("# of frames: %d, FPS: %f\n", nbframes, fps);
		delta_ms = 0;
		nbframes = 0;
		lastTime = currentTime;	

		if ((fd = open("/tmp/ipc_fps.txt", O_RDWR|O_CREAT|O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
			perror("File Open Error");
			exit(1);
		}

		if(ftruncate(fd, sizeof(fps)) == -1) { 
			perror("ftruncate error"); 
			exit(1);
		}

		dest = mmap(0, sizeof(fps), flag, MAP_SHARED, fd, 0);
		if(dest == MAP_FAILED) {
			perror("mmap failed\n");
			exit(1);
		}

		memcpy(dest, &fps, sizeof(fps));
		msync(dest, sizeof(fps), MS_SYNC);
		munmap(dest, sizeof(fps));
		close(fd);
	}
	

}

// hook glXSwapBuffers
void glXSwapBuffers(void* dpy, void* drawable) {
	static void (*lib_glXSwapBuffers)() = NULL;
	void* handle;
	char* errorstr;

	if(!lib_glXSwapBuffers) {
		/* Load real libGL */
		handle = dlopen("/usr/lib/x86_64-linux-gnu/libGL.so.1", RTLD_LAZY);
		if(!handle) {
			fputs(dlerror(), stderr);
			exit(1);
		}
		/* Fetch pointer of real glxSwapBuffers() func */
		lib_glXSwapBuffers = dlsym(handle, "glXSwapBuffers");
		if( (errorstr = dlerror()) != NULL ) {
			fprintf(stderr, "dlsym fail: %s\n", errorstr);
			exit(1);
		}
	}

	/* Calculate FPS here */
	CalculateFrameRate();

	/* Call real glxSwapBuffers() */
	lib_glXSwapBuffers(dpy, drawable);
}
