/*
Karatsuba and big number calculation implementation in c++ 

Author: Jimmy Ziqiang Cheng
Date: 29, April, 2019
*/
#include <iostream>
#include<string>
#include<cstring>
using namespace std;


// implementation of school method addition
string add_school(string a, string b, int base){
	// append length difference numbers of 0s to the shorter string
	// array length equal to the longer string, also get length difference
	int length = max(a.length(), b.length());
	int length_diff = abs((int)(a.length() - b.length()));
	if (a.length() < b.length()){
		a = string(length_diff, '0').append(a);
	}
	if (a.length() > b.length()){
		b = string(length_diff, '0').append(b);
	}

	string result_add;
	int carry = 0;
	for (int i=0; i<length; i++){
		int ai = a[length -i -1] - '0';
		int bi = b[length -i -1] - '0';
		int remainder = (ai + bi + carry)%base;
		carry = (ai+bi+carry)/base;
		result_add = to_string(remainder).append(result_add);
	}
	// check if the last carry is 0, if not, append it to the left
	if (carry != 0){
		result_add = to_string(carry).append(result_add);
	}
	return result_add;
}

string minus_school(string a, string b, int base){

	int length = max(a.length(), b.length());
	int length_diff = abs((int)(a.length() - b.length()));
	if (a.length() < b.length()){
		a = string(length_diff, '0').append(a);
	}
	if (a.length() > b.length()){
		b = string(length_diff, '0').append(b);
	}

	string result;
	int carry = 0;
	for (int i=0; i<length; i++){
		int ai = a[length -i -1] - '0';
		int bi = b[length -i -1] - '0';
		int remainder = 0;
		if ((ai-carry) >= bi){
			remainder = ai - bi - carry;
			carry = 0;
		}else{
			remainder = base + ai - bi - carry;
			carry = 1;
		}
		result = to_string(remainder).append(result);
	}
	if (result[0] == '0'){
		int l = result.length()-1;
		int numZeros = result.find_first_not_of('0');
		if (numZeros == -1){
			result.erase(0, l);
		}else{
			result.erase(0,numZeros);	
		}
	}
	return result;
}

// implementation of school method multiplication
string mult_school(string a, string b, int base){
	// array length equal to the longer string, also get length difference
	int length = max(a.length(), b.length());
	// append length difference numbers of 0s to the shorter string
	int length_diff = abs((int)(a.length() - b.length()));
	if (a.length() < b.length()){
		a = string(length_diff, '0').append(a);
	}
	if (a.length() > b.length()){
		b = string(length_diff, '0').append(b);
	}

	string result;
	for (int i=0; i<length; i++){
		int bi = b[length-i-1] - '0';
		
		string result_sub;
		int carry = 0;
		int remainder = 0;
		for (int j=0; j<length; j++){
			int ai = a[length-j-1] - '0';
			remainder = (carry + (ai*bi)%base)%base;
			carry = (ai*bi+carry)/base;
			result_sub = to_string(remainder).append(result_sub);
		}
		// add the remaining carry to the beginning of the sub result
		if (carry != 0){
			result_sub = to_string(carry).append(result_sub);
		}
		// pad trailing 0 according to index of b, to realize b*base^i
		if (i < b.length()){
			result_sub = result_sub.append(string(i, '0'));
		}
		result = add_school(result, result_sub, base);
	}
	if (result[0] == '0'){
		int numZeros = result.find_first_not_of('0');
		result.erase(0,numZeros);
	}
	return result;
}

// implementation of karatsuba multiplication with gauss trick
string mult_k2(string x, string y, int base){


	int n = max(x.length(),y.length());
	if (n < 4){
		return mult_school(x, y, base);
	}

	// append length difference numbers of 0s to the shorter string
	int length_diff = abs((int)(x.length() - y.length()));
	if (x.length() < y.length()){
		x = string(length_diff, '0').append(x);
	}
	if (x.length() > y.length()){
		y = string(length_diff, '0').append(y);
	}

	int k = n/2;
	
	string a = x.substr(0, k);
	string b = x.substr(k, x.length()-k);
	string c = y.substr(0, k);
	string d = y.substr(k, y.length()-k);

	string p0 = mult_k2(a,c,base);
	string p1 = add_school(a,b,base);
	string p2 = add_school(c,d,base);
	string p3 = mult_k2(b,d,base);

	string p12 = mult_k2(p1,p2,base);
	string p03 = add_school(p0,p3,base);

	return minus_school(add_school(add_school(p0.append(string(2*n-2*k, '0')),p3,base),p12.append(string(n-k,'0')),base),p03.append(string(n-k,'0')),base);
}

// helper function to compare two strings
bool compare (string a, string b){
	if (a.length() > b.length()){
		return true;
	}else if (a.length() == b.length()){		
		if(a>=b){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

// division
string divide_school(string a, string b, int base, string count){
	string one = "1";
	if (a.length() > b.length()){
		int length = b.length();
		int length_diff = a.length() - b.length();
		if (compare(a.substr(0, length), b)){
			string aSub = minus_school(a.substr(0, length), b, base);
			if (aSub != "0"){
				a.replace(0, length, aSub);
			}else{
				a.replace(0, length, "");
			}
			count = add_school(count, one.append(string(length_diff, '0')), 10);
			return divide_school(a, b, base, count);
		}else{
			string aSub = minus_school(a.substr(0, length+1), b, base);
			if (aSub != "0"){
				a.replace(0, length+1, aSub);
			}else{
				a.replace(0, length+1, "");
			}
			if ((length_diff-1)!=0){
				count = add_school(count, one.append(string(length_diff-1,'0')), 10);
			}else{
				count = add_school(count, one, 10);
			}
			return divide_school(a, b, base, count);
		}		
	}else if (a.length() == b.length()){
		if (compare(a,b)){
			a = minus_school(a, b, base);
			count = add_school(count, one, 10);
			return divide_school(a, b, base, count);
		}else{
			return count;
		}
	}else{
		return count;
	}
}