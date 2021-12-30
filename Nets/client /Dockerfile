FROM gcc:latest

COPY . /usr/cpp_test_2

WORKDIR /usr/cpp_test_2

RUN g++ -std=c++11 Client.cpp -o ready

CMD [ "./ready" ]
