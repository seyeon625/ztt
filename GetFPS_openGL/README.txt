Compile: make

Run:
	Run the command in a terminal
	NOTE: The path of libfps.so should be full path not relative path

	LD_PRELOAD=/path/to/directory/libfps.so 'process for getting fps'
	ex: LD_PRELOAD=$HOME/Desktop/CalFPS/libfps.so google-earth-pro

	Run the python script in another terminal
	python3 get_fps.py
