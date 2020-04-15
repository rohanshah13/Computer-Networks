#include "bridgesim.h"
#include<iostream>

bool consensus(vector<Bridge>& bridges){
	bool same_root = true;
	for(auto it=bridges.begin();it!=bridges.end();it++){
		if((it+1)!=bridges.end()){
			if(it->root_bridge!=(it+1)->root_bridge){
				same_root = false;
				break;
			}
		}
	}
	return same_root;
}

bool self_root(Bridge b){
	return b.bridge_no == b.root_bridge;
}

void send_messages(vector<Bridge>& bridges,map<char,LAN>& lans,int t,bool trace_flag){
	for(auto it=bridges.begin();it!=bridges.end();it++){
		if(self_root(*it)){
			Message m(it->bridge_no,it->bridge_no,0);
			for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
				char lan = it1->LAN_name;
				if(trace_flag)print_trace(t,'s',*it,m);
				lans[lan].messages.push_back(m);
			}
		}
		else{
			Message m(it->root_bridge,it->bridge_no,it->root_distance);
			for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
				if(it->root_port == '1' || it->root_port != it1->LAN_name){
					char lan = it1->LAN_name;
					if(trace_flag)print_trace(t,'s',*it,m);
					lans[lan].messages.push_back(m);
					//cout<<"hi";
				}
			}
		}
	}
}

void process_messages(vector<Bridge>& bridges,map<char,LAN>& lans,int t,bool trace_flag){
	for(auto it=bridges.begin();it!=bridges.end();it++){
		int this_bridge = it->bridge_no;
		for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
			char lan = it1->LAN_name;
			LAN port = lans[lan];
			for(auto message_it=port.messages.begin();message_it!=port.messages.end();message_it++){
				if(message_it->source_bridge!=this_bridge){
					if(trace_flag)print_trace(t,'r',*it,*message_it);
					if(message_it->root_bridge<it->root_bridge){
						it->root_bridge = message_it->root_bridge;
						it->root_distance = message_it->root_distance + 1;
						it->next_root = message_it->source_bridge;
						it->root_port = lan;
					}
					else if(message_it->root_bridge==it->root_bridge&&message_it->root_distance+1<it->root_distance){
						it->root_distance = message_it->root_distance + 1;
						it-> next_root = message_it->source_bridge;
						it->root_port = lan;
					}
					else if(message_it->root_bridge==it->root_bridge&&message_it->root_distance+1==it->root_distance&&message_it->source_bridge<it->next_root){
						it->next_root = message_it->source_bridge;
						it->root_port = lan;
					}
					else{}
				}
			}
		}
	}
}

void clear_messages(map<char,LAN>& lans){
	for(auto it=lans.begin();it!=lans.end();it++){
		it->second.messages.clear();
	}
}

void send_designated_port_messages(vector<Bridge>& bridges, map<char,LAN>& lans,int t,bool trace_flag){
	for(auto it=bridges.begin();it!=bridges.end();it++){
		Message m(it->root_bridge,it->bridge_no,it->root_distance);
		for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
			char lan = it1->LAN_name;
			if(trace_flag)print_trace(t,'s',*it,m);
			lans[lan].messages.push_back(m);
		}
	}
	for(auto it=bridges.begin();it!=bridges.end();it++){
			int this_bridge = it->bridge_no;
			for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
				char lan = it1->LAN_name;
				LAN port = lans[lan];
				for(auto message_it=port.messages.begin();message_it!=port.messages.end();message_it++){
					if(message_it->source_bridge!=this_bridge){
						if(trace_flag)print_trace(t+1,'r',*it,*message_it);
					}
				}
			}
		}
}

void choose_designated_ports(vector<Bridge>& bridges, map<char,LAN>& lans){
	for(auto it=lans.begin();it!=lans.end();it++){
		LAN port = it->second;
		int min_distance = 1000000000;
		int min_id = 1000000000;
		for(int i=0;i<port.messages.size();i++){
			if(port.messages[i].root_distance<min_distance){
				min_distance = port.messages[i].root_distance;
				min_id = port.messages[i].source_bridge;
			}
			else if(port.messages[i].root_distance==min_distance&&port.messages[i].source_bridge<min_id){
				min_id = port.messages[i].source_bridge;
			}
			else{}
		}
	//cout<<min_id<<endl;
	port.designated_bridge = min_id;
	//cout<<port.designated_bridge<<endl;
        vector<bridge_port> lst = bridges[min_id].ports;
        for(auto it1=lst.begin();it1!=lst.end();it1++){
        	if(it1->LAN_name==port.name){
        		it1->status = 1;
        	}
        }
        bridges[min_id].ports = lst;
        it->second = port;
	}
}

char find_port(map<char,LAN>& lans, string host){
	for(auto it=lans.begin();it!=lans.end();it++){
		LAN port = it->second;
		for(int i=0;i<port.hosts.size();i++){
			if(port.hosts[i]==host){
				return it->first;
			}
		}
	}
}

/*void set_status(vector<Bridge>& bridges, char port){
	for(auto it=bridges.begin();it!=bridges.end();it++){

	}
}*/
void start_flow(map<char,LAN>& lans, string source, string destination){
	char source_port = find_port(lans, source);
	//cout<<source_port<<endl;
	lans[source_port].flow_messages.push_back(Flow_Message(source,destination));
}

Flow_Message find_message(Bridge& b, map<char,LAN>& lans,int t,bool trace_flag){
	char port;
	for(auto it=b.ports.begin();it!=b.ports.end();it++){
		port = it->LAN_name;
		LAN lan = lans[port];
		if(lan.flow_messages.size()>0&&(it->status==1||b.root_port==it->LAN_name)&&(it->status)!=3){
			it->status = 3;
			Flow_Message fm = lan.flow_messages[0];
			if(trace_flag)print_flow_trace(t,'r',b,find_port(lans,fm.source),find_port(lans,fm.destination));
			//cout<<port<<" "<<fm.source<<endl;
			b.forwarding_table[fm.source] = port;
			//lan.flow_messages.clear();
			lans[port] = lan;
			//cout<<port<<endl;
			return fm;
		}
		
	}
	return Flow_Message("X","X");
}


bool send_flow_messages(vector<Bridge>& bridges, map<char,LAN>& lans, int t, bool trace_flag){
	bool active = false;
	for(auto it=bridges.begin();it!=bridges.end();it++){
		Flow_Message fm = find_message(*it,lans,t,trace_flag);
		//Flow_Message fm("X","X");
		if(fm.source!="X"){
			if(it->forwarding_table.find(fm.destination)==it->forwarding_table.end()){
				for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
					if((it1->status==1||it1->LAN_name==it->root_port)&&it1->status!=3){
						char lan = it1->LAN_name;
						if(trace_flag)print_flow_trace(t,'s',*it,find_port(lans,fm.source),find_port(lans,fm.destination));
						lans[lan].flow_messages.push_back(fm);
						active = true;
						it1->status=3;
					}
				}
			}
			else{
				char port = it->forwarding_table[fm.destination];
				bool send = false;
				for(auto it1 = it->ports.begin();it1!=it->ports.end();it1++){
					if(it1->LAN_name==port){
						if(it1->status!=3){
							it1->status =3;
							send = true;
						}
					}
				}
				if(send){
					if(trace_flag)print_flow_trace(t,'s',*it,find_port(lans,fm.source),find_port(lans,fm.destination));
					lans[port].flow_messages.push_back(fm);
					active = true;
				}
			}
		}
		
	}
	return active;
}

void clear_status(vector<Bridge>& bridges){
	for(auto it=bridges.begin();it!=bridges.end();it++){
		for(auto it1=it->ports.begin();it1!=it->ports.end();it1++){
			if(it1->status==3){
				if(it1->LAN_name==it->root_port){
					it1->status = 0;
				}
				else{
					it1->status = 1;
				}
			}
		}
	}
}

void clear_flow_messages(map<char,LAN>& lans){
	for(auto it=lans.begin();it!=lans.end();it++){
		it->second.flow_messages.clear();
	}
}

void print_forwarding_tables(vector<Bridge>& bridges){
	for(int i=0;i<bridges.size();i++){
		cout<<"B"+to_string(i+1)+":"<<endl;
		cout<<"HOST ID | FORWARDING PORT"<<endl;
		for(auto it=bridges[i].forwarding_table.begin();it!=bridges[i].forwarding_table.end();it++){
			cout<<it->first<<" | "<<it->second<<endl;
		}
	}
}

void print_trace(int time, char action, Bridge& b, Message m){
	cout<<time<<" "<<action<<" "<<"B"+to_string(b.bridge_no+1)<<" ("<<"B"+to_string(m.root_bridge+1)+","+to_string(m.root_distance)+",B"+to_string(m.source_bridge+1)+")"<<endl;
}

void print_flow_trace(int time, char action, Bridge& b, char source, char destination){
	cout<<time<<" "<<action<<" B"+to_string(b.bridge_no+1)+" "<<source<<"-->"<<destination<<endl;
}