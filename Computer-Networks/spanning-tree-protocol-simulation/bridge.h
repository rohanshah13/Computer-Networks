#include<vector>
#include<map>

using namespace std;

class bridge_port{
public:
	int bridge;
	char LAN_name;
	int status;
	bridge_port(int b, char l, int s);
};
class Bridge{
public:
	int bridge_no;
	vector<bridge_port> ports;
	int root_bridge;
	int next_root;
	char root_port;
	int root_distance;
	map<string,char> forwarding_table;
	Bridge(int no, vector<bridge_port> p);
	Bridge();
};
class Message{
public:
	int root_bridge;
	int source_bridge;
	int root_distance;
	Message(int r, int s, int d);
	void forward(int s);
};
class Flow_Message{
public:
	string source;
	string destination;
	Flow_Message(string s,string d);
};
class LAN{
public:
	char name;
	int designated_bridge;
	vector<Message> messages;
	vector<Flow_Message> flow_messages;
	vector<string> hosts;
	LAN(char n,vector<string> h);
	LAN(char n);
	LAN();
};
class Flow{
public:
	string host1;
	string host2;
	Flow(string h1, string h2);
	Flow();
};

