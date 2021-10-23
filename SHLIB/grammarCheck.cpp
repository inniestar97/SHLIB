#include "grammarCheck.h"
#include <string>
#include <regex>

//완성
bool check_id(string id){
	//앞자리 공백 삭제
	const auto st = id.find_first_not_of(" ");
	id = id.substr(st);

	//공백 포함여부
	if (id.find(" ") != string::npos) {
		return false;
	}

	//한글,특수문자 잇으면 안됨
	regex r0("[|~!@#`$=-%^&*()_+?></.:;]");
	if(regex_match(id, r0)){
		return false;
	}
	
	regex r1("[ㄱ-ㅎ|ㅏ-ㅣ|가-힣]+");
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

//완성
bool check_password(string pw){
	//앞자리 공백 삭제
	const auto st = pw.find_first_not_of(" ");
	pw = pw.substr(st);

	//공백 포함여부
	if (pw.find(" ") != string::npos) {
		return false;
	}
	
	regex r0("[ㄱ-ㅎ|ㅏ-ㅣ|가-힣]+");
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

//완성
bool check_Name(string name){
	if (name.find(" ") != string::npos) {
		return false;
	}
	
	regex r0("[ㄱ-ㅎ|ㅏ-ㅣ|가-힣]{1,}");
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

// 완성
bool check_book(string name){
	return true;
}

// 완성 
bool check_author(string name){

	regex r0("[ㄱ-ㅎ|ㅏ-ㅣ|가-힣]{1,}");
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

// 완성
bool check_translator(string name){

	if (name.length() == 0)
		return true;

	regex r0("[ㄱ-ㅎ|ㅏ-ㅣ|가-힣]{1,}");
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
	
	return true;
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

//완성
bool check_publisher(string pub){
	
	regex r("[~!@#`$=-%^&*()_+?></.,:;]{1,}");
	
	if(regex_match(pub, r)){
		return false;
	}
	
	return true;
}

//
bool check_year(string year){
	if (year.length() != 4)
		return false;
	 return !year.empty() && find_if(year.begin(), year.end(), [](unsigned char c) { return !isdigit(c); }) == year.end();
}