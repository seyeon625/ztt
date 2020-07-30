import mmap
import os
import struct
import time
from inotify_simple import INotify, flags

'''
	Get FPS from a C process which is hooking glXSwapBuffers() in OpenGL

	Author: Kyungmin Bin
	E-mail: kmbin@unist.ac.kr
'''

mmap_file_name = "/tmp/ipc_fps.txt"
inotify = INotify()
wd = inotify.add_watch(mmap_file_name, flags.MODIFY)

while True:
	fd = os.open(mmap_file_name, os.O_RDONLY)
	buf = mmap.mmap(fd, os.path.getsize(mmap_file_name), 
			mmap.MAP_SHARED, mmap.PROT_READ)
	line = buf.readline()
	if inotify.read()[0].mask == flags.MODIFY:
		if len(line) == 8:
			fps = struct.unpack('d', line)
			print("Recieved FPS: " + str(fps[0]))
	
