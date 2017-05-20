#!/usr/bin/python3.4
"""
    Generate a basic makefile to compile all the cpp files
    The headers are added as dependencies, so if they change, the
    project gets updated accordingly
"""

from glob import glob
from re import match, sub


def generate_makefile():
    """ generates a makefile for the cpp files in the src/ directory """

    cpp_files = glob('src/*.cpp')
    header_files = glob('src/*.hpp')
    header_files.extend(glob('src/*.h'))
    obj_files = ''

    for filename in cpp_files:
        obj_files += ' ' + \
            sub(r'src[/\\]([^.]*)\.cpp', r'obj/\g<1>.obj', filename)

    output = """# https://bugs.launchpad.net/ubuntu/+source/gcc-defaults/+bug/1228201/comments/4
CPPFLAGS = -pthread -std=c++11 -Wall -Wl,--no-as-needed
LDLIBS = -lsfml-system -lsfml-network -lmysqlcppconn
RM = rm -rf
TOUCH = touch

all: bin/server

bin/server: """ + obj_files + """
	$(CXX) -o bin/server""" + obj_files + """ $(LDLIBS) $(CPPFLAGS)

"""
    for filename in cpp_files:
        file = open(filename)
        rows = file.read().split('\n')
        file.close()

        filename = sub(r'src[/\\](.*)\.cpp$', r'\g<1>', filename)
        output += 'obj/' + filename + '.obj: '
        output += 'src/' + filename + '.cpp'

        for row in rows:
            _ = match(r'#include "(.*\.h(pp)?)"', row)
            if _:
                output += ' src/' + _.group(1)

        output += '\n\t$(CXX) -c -o obj/' + filename + '.obj src/' \
            + filename + '.cpp $(CPPFLAGS)\n\n'

    for filename in header_files:
        file = open(filename)
        rows = file.read().split('\n')
        file.close()

        included_files = ''
        for row in rows:
            _ = match(r'#include "(.*\.h(pp)?)"', row)
            if _:
                included_files += ' src/' + _.group(1)

        if included_files != '':
            filename = sub(r'\\', '/', filename)
            output += filename + ':' + included_files + '\n\n'

    output += """clean:
	$(RM) bin/server""" + obj_files + """
"""

    file = open('makefile', 'w')

    file.write(output)
    file.close()
    print('done')

if __name__ == '__main__':
    generate_makefile()
