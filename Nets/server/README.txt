To run: 
Open terminal in folder with files then 
	1. Server
		1) write "sudo docker build . -t cpp_test:1"
		2) using "sudo docker images" you can find our image
		3) then "sudo docker run --rm -it cpp_test:1"
	2. Client 
		1) write "sudo docker build . -t cpp_test_2:1"
		2) using "sudo docker images" you can find our image
		3) then "sudo docker run --rm -it cpp_test_2:1"
	3. When server works there 3 test in client which you can run writing 1 or 2 or 3 	   in the client's terminal window 
	4. To stop both server and client write ctrl+c in their terminal window

P.S.    in Dockerfile server and client work on 3434 port and you can change it, by 		opening Dockerfile in any text editor. Now only local connection included. You can 	include distance connection by using getaddrinfo function

!!!     if something went wrong with DOCKER or you receive error of connection or error of 	getaddrinfo function (I don't know how to fix them, because they occur only with 	using of docker)  you can use makefile included also:
	1) Put Client.cpp and main.cpp in one folder with makefile
	2) compile them using "make" and then run them "./Server" and "./Client"