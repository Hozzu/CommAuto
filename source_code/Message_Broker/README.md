---
# USER MANUAL

## 0. Requirements
- bash
- Java Runtime Environment (JRE) 1.7 (1.6 for version <=5.10.0)

Tested on Ubuntu 16.04


## 1. ActiveMQ 설치
### a) 아래 link에서 activemq 다운로드(apache-activemq-*.*.*-bin.tar.gz).
Link: https://activemq.apache.org/components/classic/download/ 

### b) 다운받은 파일을 압축 해제하여 apache-activemq-*.*.* 디렉토리 생성 확인.
$ tar xvfz apache-activemq-*-*-*-bin.tar.gz 


## 2. Broker administrator 연결 설정
### a) 제공된 "activemq" 파일의 59번째 라인에서 broker administrator의 URI({ADMIN_URI}) 설정.
$ vi activemq

BROKER_ADMIN="{ADMIN_URI}:8080"

### b) 설정한 "activemq" 파일을 apache-activemq-*.*.*/bin 디렉토리에 붙여넣기.
$ cp activemq apache-activemq-*.*.*/bin/


## 3. Tag 설정
### Tag는 edge device와 data center server가 데이터를 주고 받을 때 쓰는 password 역할을 함.

### a) 원하는 이름({BROKER_NAME})으로 ActiveMQ broker 생성.
$ cd apache-activemq-*.*.*/bin
$ bash activemq create {BROKER_NAME}

### b) {BROKER_NAME}/conf 디렉토리 하의 activemq.xml의 96번째 라인에 다음을 추가하여 원하는 tag({TAG_NAME}) 설정.
$ vi {BROKER_NAME}/conf/activemq.xml

	<destinationInterceptors>
	    <virtualDestinationInterceptor>
	        <virtualDestinations>
	            <compositeQueue name="{TAG_NAME}">
	                <forwardTo>
	                    <topic physicalName="{TAG_NAME}_PUB"/>
	                </forwardTo>
	            </compositeQueue>
	        </virtualDestinations>
	    </virtualDestinationInterceptor>
	</destinationInterceptors>


## 4. ActiveMQ 실행
$ bash activemq console

---
