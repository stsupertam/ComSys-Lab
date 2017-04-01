FROM gcc:4.9
ADD ./midterm_2.c
RUN gcc -shared -Wl,-soname,midterm_2 -o midterm_2.so -fPIC midterm2.c

FROM python:2.7
ADD ./testlibwrapper.py .
ADD ./data .
ADD ./midterm_2.so .
CMD [ "python", "./testlibwrapper.py" ]
