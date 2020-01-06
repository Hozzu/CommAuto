#include <iostream>
#include <vector>
#include "ActiveMQ.h"

vector<string> receive_vector;

/* Description: 
 * This is a main function for Consumer. The user can connect, and receive data to brokers. 
 * User has a right to receive data by using corresponding tag. This can be changed by the user.
 * The user can store received message(data) at receive_vector.
 * The user can choose between receive(pull) and receive_async(push).
 * Inputs: None 
 * Return value: None
 */ 
int main(int argc AMQCPP_UNUSED, char* argv[] AMQCPP_UNUSED){

	ActiveMQ_Solution active_consumer;
	active_consumer.c_connect("147.47.208.44:8080");
	
	string tag = "tag1";

	//active_consumer.receive_async(tag, receive_vector);
	while(1) active_consumer.receive(tag, &receive_vector);
	
	cout << "receive end" << endl;	
}
                                                                                                                                                                                                   
                                                                                                                                                                                                    
                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
 

