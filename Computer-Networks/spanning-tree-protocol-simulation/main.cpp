//#include "bridge.h"
#include "bridgesim.h"
#include<bits/stdc++.h>

using namespace std;

bool compare(bridge_port bp1, bridge_port bp2){
	return(bp1.LAN_name<bp2.LAN_name);
}

int main(){
	bool trace_flag;
	cin>>trace_flag;
	int n;
	cin>>n;
	string line;
	getline(cin,line);
	//cout<<endl;
	vector<Bridge> bridges(n);
	map<char,LAN> lans;
	for(int i=0;i<n;i++){
		vector<bridge_port> ports;
		getline(cin,line);
		//cout<<line<<i<<endl;
		istringstream ss(line);		
		bool first = true;
		while(ss){
			if(first){
				first = false;
				string word;
				ss>>word;
			}
			else{
				string word;
				ss>>word;
				//cout<<word.size()<<endl;
				if(word.size()>0){
					bridge_port bp(i,word[0],0);
					ports.push_back(bp);
					if(lans.find(word[0])==lans.end()&&word[0]>=65&&word[0]<=90&&word!="\n"&&(int)word[0]!=0&&word.size()>0){
						//cout<<(int)word[0]<<endl;cout<<lans.size()<<endl;

						LAN l(word[0]);
						lans[word[0]] = l;
					}
				}
			}
			Bridge b(i,ports);
			bridges[i] = b;
		}
	}
	for(int i=0;i<lans.size();i++){
		vector<string> hosts;
		getline(cin,line);
		//cout<<line<<i<<endl;
		istringstream ss(line);		
		bool first = true;
		string lan;
		while(ss){
			if(first){
				first = false;
				//string lan;
				ss>>lan;
			}
			else{
				string word;
				ss>>word;
				if(word.size()>0){
					hosts.push_back(word);
					;
				}
			}
			if(lan.size()>0){
			lans[lan[0]].hosts = hosts;
			//cout<<lans[lan[0]].hosts.size();
			}
		}	
	}
	getline(cin,line);
	int m = stoi(line);
	vector<Flow> flows(m);
	for(int i=0;i<m;i++){
		getline(cin,line);
		istringstream ss(line);
		string h1,h2;
		ss>>h1>>h2;
		Flow f(h1,h2);
		flows[i] = f;
	}
	int t=0;
	while(!consensus(bridges)){
		send_messages(bridges,lans,t,trace_flag);
		process_messages(bridges,lans,t+1,trace_flag);
		clear_messages(lans);
		t++;
	}
	send_designated_port_messages(bridges,lans,t,trace_flag);
	choose_designated_ports(bridges,lans);
	clear_messages(lans);
	/*for(auto it=lans.begin();it!=lans.end();it++){
		LAN port = it->second;
		cout<<port.name<<" "<<port.designated_bridge<<endl;
	}*/
	for(int i=0;i<bridges.size();i++){
		string output = "";
		int no = i+1;
		string b = "B"+to_string(no)+":";
		string rem = "";
		vector<bridge_port> ports = bridges[i].ports;
		sort(ports.begin(),ports.end(),compare);
		for(int j=0;j<ports.size();j++){
			if(bridges[i].root_port==ports[j].LAN_name){
				rem = rem + " " + ports[j].LAN_name + "-RP";
			}
			else{
				if(ports[j].status==1){
					rem = rem + " " + ports[j].LAN_name + "-DP";
				}
				else{
					rem = rem + " " + ports[j].LAN_name + "-NP";
				}
			}
		}
		output = b + rem;
		cout<<output<<endl;
	}
	
	for(auto it=flows.begin();it!=flows.end();it++){
		t=0;
		//cout<<it->host1<<" "<<it->host2<<endl;
		start_flow(lans,it->host1,it->host2);
		bool active = true;
		while(active){
			active = send_flow_messages(bridges,lans,t,trace_flag);
			t++;
			//cout<<active<<endl;
		}
		clear_status(bridges);
		clear_flow_messages(lans);
		print_forwarding_tables(bridges);
		if(it+1!=flows.end())cout<<endl;
	}
}