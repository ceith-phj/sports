NAME := ui_sports

$(NAME)_VERSION    := 0.1.0
$(NAME)_SUMMARY    := ui sports
MODULE             := ui_sports

$(NAME)_COMPONENTS := packages.apps.sports

$(NAME)_INCLUDES :=  ./include \
					 ./include/containers \
					 ./include/setting \
					 ./include/sport \
					 ./include/typeInit \
					 ./include/itemEdit

$(NAME)_SOURCES     += src/*.c \
					   src/*/*.c\
					   src/*/*/*.c\
					   src/*/*/*/*.c


GLOBAL_INCLUDES 	+= ./include