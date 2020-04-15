#include "bridge.h"

bool consensus(vector<Bridge>& bridges);

bool self_root(Bridge b);

void send_messages(vector<Bridge>& bridges, map<char,LAN>& lans, int t, bool trace_flag);

void process_messages(vector<Bridge>& bridges, map<char,LAN>& lans, int t, bool trace_flag);

void clear_messages(map<char,LAN>& lans);

void send_designated_port_messages(vector<Bridge>& bridges, map<char,LAN>& lans,int t,bool trace_flag);

void choose_designated_ports(vector<Bridge>& bridges, map<char,LAN>& lans);

char find_port(map<char,LAN>& lans, string host);

void start_flow(map<char,LAN>& lans, string source, string destination);

Flow_Message find_message(Bridge& b, map<char,LAN>& lans,int t,bool trace_flag);

bool send_flow_messages(vector<Bridge>& bridges, map<char,LAN>& lans,int t, bool trace_flag);

void clear_status(vector<Bridge>& bridges);

void clear_flow_messages(map<char,LAN>& lans);

void print_forwarding_tables(vector<Bridge>& bridges);

void print_trace(int time, char action, Bridge& b, Message m);

void print_flow_trace(int time, char action, Bridge& b, char source, char destination);