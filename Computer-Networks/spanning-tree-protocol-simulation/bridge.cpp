#include "bridge.h"

bridge_port :: bridge_port(int b,char l,int s){
	bridge = b;
	LAN_name = l;
	status = s;
}

Bridge :: Bridge(int no, vector<bridge_port> p){
	ports = p;
	bridge_no = no;
	root_bridge = no;
	next_root = no;
	root_port = '1';
	root_distance = 0;
}

Bridge :: Bridge(){

}

Message :: Message(int r, int s, int d){
	root_bridge = r;
	source_bridge = s;
	root_distance = d;
}

void Message :: forward(int s){
	source_bridge = s;
	root_distance++;
}

LAN :: LAN(char n,vector<string> h){
	name = n;
	hosts = h;
}

LAN :: LAN(char n){
	name = n;
}

LAN :: LAN(){

}
Flow :: Flow(string h1,string h2){
	host1 = h1;
	host2 = h2;
}
Flow :: Flow(){
	
}
Flow_Message :: Flow_Message(string s, string d){
	source = s;
	destination = d;
}
