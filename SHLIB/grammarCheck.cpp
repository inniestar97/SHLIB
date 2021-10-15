#include "grammarCheck.h"

//肯己
bool check_id(string id){
	//菊磊府 傍归 昏力
	const auto st = id.find_first_not_of(" ");
	id = id.substr(st);

	//傍归 器窃咯何
	if (id.find(" ") != string::npos) {
		return false;
	}

	//茄臂,漂荐巩磊 勒栏搁 救凳
	regex r0 = ("[|~!@#`$=-%^&*()_+?></.:;]");
	if(regex_match(id, r0)){
		return false;
	}
	
	regex r1("[ぁ-ぞ|た-び|啊-芌]+");
	if(regex_match(id, r1)){
		return false;
	}

	regex r2("[a-zA-Z1-9_]{6,15}");
	if(regex_match(id, r2)){
		return true;
	}
	else{
		return false;
	}	

}

//肯己
bool check_password(string pw){
	//菊磊府 傍归 昏力
	const auto st = pw.find_first_not_of(" ");
	pw = pw.substr(st);

	//傍归 器窃咯何
	if (id.find(" ") != string::npos) {
		return false;
	}
	
	regex r0("[ぁ-ぞ|た-び|啊-芌]+");
	if(regex_match(pw, r0)){
		return false;
	}

	regex r1("[a-zA-Z1-9]{8,20}");
	if(!regex_match(pw, r1)){
		return false;
	}

	regex r2("[~!@#`$=-%^&*()_+?></.,:;]{1,}");
	if(!regex_match(pw, r2)){
		return false;
	}

	return true;
}

//肯己
bool check_Name(string name){
	if (name.find(" ") != string::npos) {
		return false;
	}
	
	regex r0("[ぁ-ぞ|た-び|啊-芌]{1,}");
	if(!regex_match(name, r0)){
		return false;
	}

	regex r1("[a-zA-Z1-9]{1,}");
	if(regex_match(name, r1)){
		return false;
	}

	regex r2("[~!@#`$=-%^&*()_+?></.,:;]{1,}");
	if(regex_match(name, r2)){
		return false;
	}

	if (name.length() < 2 || name.length() > 6)
		return false;

	return true;

}

// 肯己
bool check_book(string name){
	return true;
}

// 肯己 
bool check_author(string name){

	regex r0("[ぁ-ぞ|た-び|啊-芌]{1,}");
	if(!regex_match(name, r0)){
		return false;
	}

	regex r1("[a-zA-Z1-9]{1,}");
	if(regex_match(name, r1)){
		return false;
	}

	regex r2("[~!@#`$=-%^&*()_+?></,:;]{1,}");
	if(regex_match(name, r2)){
		return false;
	}
	
	return true;
}

// 肯己
bool check_translator(string name){
	regex r0("[ぁ-ぞ|た-び|啊-芌]{1,}");

	if(!regex_match(name, r0)){
		return false;
	}

	regex r1("[a-zA-Z1-9]{1,}");
	if(regex_match(name, r1)){
		return false;
	}

	regex r2("[~!@#`$=-%^&*()_+?></.,:;]{1,}");
	if(regex_match(name, r2)){
		return false;
	}
	
	return true
}


bool check_studentID(string s_id)
{
	if (s_id.length()!=9 )
		return false;
	
	int id = stoi(s_id);
	
	if (id<193100000)
		return false;
	
	return true;
}

//肯己
bool check_publisher(string pub){
	
	regex r("[~!@#`$=-%^&*()_+?></.,:;]{1,}");
	
	if(regex_match(pub, r)){
		return false;
	}
	
	return true
}

//
bool check_year(string year){
	if (year.length()!=4  )
		return false;
	 return !year.empty() && find_if(year.begin(), year.end(), [](unsigned char c) { return !isdigit(c); }) == year.end();
}