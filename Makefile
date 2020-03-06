# Where the different the lab should be copied to when handed in.
HANDINDIR = /users/groups/cs224ta/bmpFilter/handin

# Replace these file names with the names of your source files you want included in your hand in
FILES = main.c pixel.c pixel.h

NAME = bmpFilter

NOW = $(shell date +%F.%T)

CC = gcc
CFLAGS = -g -Wall

all: bmpFilter
	# Generate a handin tar file each time you compile
	-tar -cvf ${USER}-handin.tar  ${FILES}

bmpFilter:
	$(CC) $(CFLAGS) -o $(NAME) *.c

handin:
	install -m660 ${USER}-handin.tar $(HANDINDIR)/${USER}-handin-${NOW}.tar 

#
# Clean the src dirctory
#
clean:
	rm -f $(NAME)
