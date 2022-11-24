BIN = libcnct.so
VERSION = 0.0.1

CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wfloat-equal -g -std=c99 -fPIC -shared

LIBS = -lm -lpthread `pkg-config --cflags --libs glib-2.0`
