---
# USER MANUAL

## 0. Requirements
- ActiveMQ-CPP library
(http://activemq.apache.org/building-activemq-cpp)
- g++
- libboost-regex
- libcurl

Tested on Ubuntu 16.04


## 1. Consumer 코드 작성
Consumer.cpp의 main함수에서 원하는 코드 작성.
(제공되는 API는 첨부된 슬라이드 참고)


## 2. 소스코드 build
$ g++ -o Consumer -pthread ActiveMQ.cpp Consumer.cpp -lactivemq-cpp -luuid -lcurl -lboost_regex


## 3. Consumer 실행
$ ./Consumer
	
---
