
project 		:= libCommon
project_name 	:= $(project).so
project_out_dir	:= ../Output/
project_type	:= g++ -shared -fPIC
#project_type	:= g++
#project_type	:= ar -r

c_flags			:= debug
#c_flags		:= release

out_put_path	:= ../Output/$(project)

source			:= ./

libs			:= -ldl -lpthread
#-lLogger
#libs			:= -lLib1 -lLib2

libs_path		:= -L../Output/$(c_flags)
#libs_path		:= -Lpath1 -Lpath2

include_path	:= -I../ 
#../Thridparty
#include_path	:= -I/usr/lib/xxxx/include

sub_dir			:= $(shell find $(source) -type d)

out_put_dir		:= $(out_put_path)/$(c_flags)

debug_flag		:= -O0 -g -Wall -c
release_flag	:= -O3 -Wall -c

rm_				:= rm -rf

cpp_source		:= $(shell find	./$(source) -name '*.cpp')

objects			:= $(cpp_source:%.cpp=$(out_put_dir)%.o)

ifeq ($(c_flags), debug)
	c_compile_c_flags := $(debug_flag)
else
	c_compile_c_flags := $(release_flag)
endif

all: dir_make $(out_put_dir)$(project_name) copy

dir_make: 
	@mkdir -p $(out_put_dir);
	@for val in $(sub_dir); do 					\
		if [ $${val} != . ]; then 				\
			mkdir -p $(out_put_dir)$${val};		\
		fi 										\
	done;

$(out_put_dir)$(project_name): $(objects)
	$(project_type) $(libs_path) -o $(out_put_dir)/$(project_name) $^ $(libs) -pg

$(out_put_dir)%o:./%cpp
	g++ -fPIC $(c_compile_c_flags) $(include_path) -fpermissive -o $@ $<

clean:
	-$(rm_) $(out_put_path)/*

copy:
	@mkdir -p $(project_out_dir)$(c_flags)
	@cp $(out_put_dir)/$(project_name) $(project_out_dir)$(c_flags)/$(project_name)

install:
	@mkdir -p $(project_out_dir)$(c_flags)
	@cp $(out_put_dir)/$(project_name) $(project_out_dir)$(c_flags)/$(project_name)

.PHONY:all clean



