task: task.c reader.c utils.c grid.c
	gcc task.c reader.c utils.c grid.c -lm -o task
read: read.c reader.c ui.c utils.c
	gcc read.c reader.c ui.c utils.c -lm -o read
