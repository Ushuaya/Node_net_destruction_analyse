FROM gcc:latest

COPY . /usr/cpp_test

WORKDIR /usr/cpp_test

RUN g++ -std=c++11 main.cpp -o ready

CMD [ "./ready" ]
