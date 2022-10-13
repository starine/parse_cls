// parse_cls.cpp : Defines the entry point for the console application.

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <iomanip>

using namespace std;

typedef struct TheElement{
    string name;
    string type;
    string length;
}TheElement;

typedef struct TheObject{
    string name;
    vector<TheElement> elements;
}TheObject;

//去除头尾空格和tab符号
std::string& trim(std::string &s)
{
    if(s.empty()){
        return s;
    }
	int tempi;
	int tempj;
	tempi = s.find_first_not_of(" ");
	tempj = s.find_first_not_of("\t");
    while((tempi != string::npos && tempi > 0) || (tempj != string::npos && tempj > 0)){
		if(tempi != string::npos && tempi > 0) s.erase(0, s.find_first_not_of(" ")); //去除开头空格
		tempj = s.find_first_not_of("\t");
        if(tempj != string::npos && tempj > 0) s.erase(0, s.find_first_not_of("\t"));//去除空格后面的tab
		tempi = s.find_first_not_of(" ");
		tempj = s.find_first_not_of("\t");
		if(tempi == tempj) break; //开头空格和tab去除完成
	}
    //去除末尾空格和tab	
	tempi = s.find_last_not_of(" ");
	tempj = s.find_last_not_of("\t");
	while((tempi != string::npos && tempi > 0) || (tempj != string::npos && tempj > 0)){
		if(tempi != string::npos && tempi > 0) s.erase(tempi + 1);
		tempj = s.find_last_not_of("\t");
		if(tempj != string::npos && tempj > 0) s.erase(tempj + 1);
		tempi = s.find_last_not_of(" ");
		tempj = s.find_last_not_of("\t");
		if(tempi == tempj) break;
	}
    return s;
}

//对于$开头的key，检索map中是否存在该key
bool isValueKey(map<string, string>& keyValueMap, const string key){
    for (map<string, string>::iterator itr = keyValueMap.begin(); itr != keyValueMap.end(); itr++) {
        //cout << itr->first << itr->second << endl;
        if(key.compare(itr->first) == 0) return true;
    }
    return false;
}

//str：#DEFINE IDLength=10 
bool parseDefine(map<string, string>& keyValueMap, const string& str)
{
    string key;
    string value;
    int starti = -1;
    int len = 0;
    if(str.empty()){
        cout << "Format error: " << str  << endl;
        return false;
    } 
    //参数str必须包含一个=符号，和至少一个空格,且=符号不能在最末尾
    if(str.find_first_of("=") == string::npos || 
       str.find_first_of(" ") == string::npos || 
       str.find_first_of("=") != str.find_last_of("=") || 
       str.length() - 1 <= str.find_first_of("=")
       ){
        cout << "Format error: " << str  << endl;
        return false;
    }

    starti = str.find_first_of(" ") + 1;
    len = str.find_first_of("=") - str.find_first_of(" ") - 1;
    if( (starti + len) > str.length() ) {
        cout << "---------------error:out of memory!--------------- " <<  endl;
        return false;
    }
    key = str.substr(starti, len);
    key = trim(key); //可能存在多个空格

    starti = str.find_first_of("=") + 1;
    len = str.length() - str.find_first_of("=") -1;
    if( (starti + len) > str.length() ) {
        cout << "---------------error:out of memory!--------------- " <<  endl;
        return false;
    }
    value = str.substr(starti, len);
    value = trim(value); //"="两边可能存在空格

    if(key.empty() || value.empty() || key.length() < 1 || value.length() < 1){
        cout << "Format error: " << str  << endl;
        return false;
    }
    keyValueMap.insert(make_pair(key, value)); //将字符串转换为键值对

    //cout << "get a key: " << key  << endl;
    //cout << "get a value: " << value << endl;
    return true;
}

//查找map，如果存在该键值对，返回value，否则返回key
string getValue(map<string, string>& keyValueMap, const string key){
    for (map<string, string>::iterator itr = keyValueMap.begin(); itr != keyValueMap.end(); itr++) {
        //cout << itr->first << itr->second << endl;
        if(key.compare(itr->first) == 0) return itr->second;
    }
    return key;
}

//解析对象名，输入格式是：#OBJECT=objA 
string getObjectName(const string str){
    string temp = "";
	//只能有一个=符号，并且不能在最末尾
	if(str.find_first_of("=") == string::npos ||  
       str.find_first_of("=") != str.find_last_of("=") || 
       str.length() - 1 <= str.find_first_of("=")
       ){
        cout << "Format error: " << str  << endl;
        return temp;
    }
	int starti = -1;
    int len = 0;
	starti = str.find_first_of("=") + 1;
    len = str.length() - str.find_first_of("=") -1;
    if( (starti + len) > str.length() ) {
        cout << "---------------error:out of memory!--------------- " <<  endl;
        return temp;
    }
    temp = str.substr(starti, len);
    temp = trim(temp);
    return temp;
}

//检查ELEMENT是否是有效，输入格式：#ELEMENT=szName      	TYPE=C       	LENGTH=$NameLength
bool isElementValue(const string str){
	if(str.empty()){
        cout << "Format error: " << str  << endl;
        return false;
    } 
    //初步判断参数str必须包含一个=符号，和至少一个空格,且=符号不能在最末尾
    if(count(str.begin(), str.end(), '=') != 3 || 
       (count(str.begin(), str.end(), ' ') < 2  && count(str.begin(), str.end(), '\t') < 2)  //至少有2个空格或者2个tab键
       ){
        cout << "Format error: " << str  << endl;
        return false;
    }
	if(str.find_first_of("#ELEMENT") == string::npos || 
	   str.find_first_of("TYPE") == string::npos || 
	   str.find_first_of("LENGTH") == string::npos 
	   ){
		cout << "Format error: " << str  << endl;
        return false;
	}
	return true;
}

//解析出element插入到vector，输入格式：#ELEMENT=szName      	TYPE=C       	LENGTH=$NameLength
bool parseElement(vector<TheElement>& elements, map<string, string>& keyValueMap, const string str){
	if(isElementValue(str) == false) return false; //初步检查
    string temp;
    TheElement e;
    int starti;
    int len;
    
	starti = str.find_first_of("=") + 1;
    len = str.find("TYPE") - str.find_first_of("=") - 1;  
    if( (starti + len) > str.length() ) {
        cout << "---------------error:out of memory!--------------- " <<  endl;
        return false;
    }
    temp = str.substr(starti, len);
    e.name = trim(temp); //“=”两边可能存在空格

	starti = str.find("TYPE") + 4; //"TYPE"长度是4
    len = str.find("LENGTH") - str.find("TYPE") - 4; //"TYPE"长度是4
    if( (starti + len) > str.length() ) {
        cout << "---------------error:out of memory!--------------- " <<  endl;
        return false;
    }
    temp = str.substr(starti, len);
    temp = trim(temp); //=C ,接着要删掉=号
	starti = temp.find_first_of("=") + 1;
    len = temp.length() - 1;
	if( (starti + len) > str.length() ) {
        cout << "---------------error:out of memory!--------------- " <<  endl;
        return false;
    }
    temp = temp.substr(starti, len);
    temp = trim(temp);
    // starti = temp.find(","); //如果需要解析tag
    // if (starti != -1) {
    //     e.type = temp.substr(0,starti);
    //     e.tag = temp.substr(starti+1);
    // }
	e.type = temp;

    starti = str.find_last_of("=") + 1;
    len = str.length() - str.find_last_of("=") - 1;
    temp = str.substr(starti, len); 
    temp = trim(temp);
    e.length = temp;

	starti = temp.find_first_of("$");
	if(starti >= 0){
		temp.erase(0, temp.find_first_not_of("$"));
		if(isValueKey(keyValueMap, temp) == false){
			cout << "error: " << temp << " has not define!!!" << endl;
			return false;
		}
        e.length = getValue(keyValueMap, temp);
	}

    
    //cout << e.name << "  e.type is: " << e.type << "  e.length is:  " << e.length;
    elements.push_back(e);
    return true;
}

/**
格式化输出常量:
DEFINE:
NAME		    VALUE
IDLength	    10
NameLength 	    20
...
*/
void printDefine(map<string, string>& keyValueMap){
    if(keyValueMap.size() <= 0) return;
    cout << "DEFINE: " << endl;
    cout << std::left << setw(16) << "NAME" << std::left << setw(16) << "VALUE" << endl;
	for (map<string, string>::iterator itr = keyValueMap.begin(); itr != keyValueMap.end(); itr++) {
        cout << std::left << setw(16) << itr->first.c_str() << std::left << setw(16) << itr->second.c_str() << endl;
    }
}

/**
格式化输出对象:
OBJECT:
objA:
ELEMENT		    TYPE		    LENGTH
szName		    C			    20
szRdfID		    C,Key		    10
...
*/
void printObjects(vector<TheObject>& object_v){
     int size = object_v.size();
     TheObject objtemp;
	 vector<TheElement> elemtemp;
	 TheElement e;
	 int i = 0;
	 int j = 0;
     cout << "OBJECT: " << endl;
	 for(i = 0; i < size; i++){
		objtemp = object_v.at(i);
		cout << objtemp.name << ":" << endl;
		j = 0;
        elemtemp = objtemp.elements;
        cout << std::left << setw(16) << "ELEMENT"  << std::left << setw(16) << "TYPE" << std::left << setw(16) << "LENGTH" << endl;
		for(j = 0; j < objtemp.elements.size(); j++){
			e = objtemp.elements.at(j);
            cout << std::left << setw(16) << e.name.c_str() << std::left << setw(16) << e.type.c_str() << std::left << setw(16) << e.length.c_str() << endl;
		}
        cout << endl;
	 }
}

int main()
{
    string test = "./test.cls";
    ifstream inFile = ifstream(test.c_str(), ios_base::in);
    if (!inFile)
    {
        cout << "can not open file" << test << endl;
    }
    string str;
    size_t nLoc = -1;
    int line = 0;
    int tempi = -1;
	int tempj = -1;
    map<string, string> keyValueMap; //存储读取到的键值对
    vector<TheObject> object_v; //储存读取到的object
    int object_layer = 0; //对象关闭的状态变量

    while(getline(inFile,str))
    {
        str = trim(str);
		tempi = str.find_first_of("#");
        if(tempi == 0){
            //cout << str <<endl;
			tempi = str.find("DEFINE");
            if(tempi > 0){
                if(parseDefine(keyValueMap, str) == false) cout << "----Find error at: " << str <<endl;
            }
			tempi = str.find("#OBJECT");
            if(tempi >= 0){
                object_layer++;
                TheObject objtemp;
                objtemp.name = getObjectName(str);
                while(getline(inFile,str)){
                    str = trim(str);
                    tempi = str.find("#ELEMENT");
                    if(tempi == 0) parseElement(objtemp.elements, keyValueMap, str);
					tempi = str.find("#END");
                    tempj = str.find("OBJECT");
                    if(tempi >= 0 && tempj >= 0) {
                        object_layer--;
                        break;
                    }
					//cout << str <<endl;
                }
                if(object_layer != 0) cout << "----Find error at (there is not #END OBJECT): " << objtemp.name <<endl;
                object_v.push_back(objtemp);
            }
        }
    }

	printDefine(keyValueMap);
    cout << endl;
	printObjects(object_v);
	return 0;
}
