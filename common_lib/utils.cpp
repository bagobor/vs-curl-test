#include "stdafx.h"

#include "utils.h"

#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Ws2_32.lib")

size_t get_file_size(std::ifstream& f)
{
	size_t curr_pos = f.tellg();
	f.seekg( 0, std::ios_base::end );
	size_t file_size = f.tellg();

	f.seekg( curr_pos, std::ios_base::beg );

	return file_size;
}

bool write_string_to_file(const wchar_t* filename, const std::string& str)
{
	std::ofstream out(filename, std::ios::binary);

	if (!out.is_open())
		return false;

	size_t string_size = str.size();
	out.write((const char*)&str[0], string_size);
	out.close();

	return true;
}

bool read_file_to_string(const wchar_t* filename, std::string& out)
{
	std::ifstream in(filename, std::ios::binary);

	if (!in.is_open())
		return false;

	size_t file_size = get_file_size(in);

	out.resize(file_size);
	in.read(&out[0], file_size);
	in.close();

	size_t string_size = out.size();

	return true;
}

bool read_file_to_string(const char* filename, std::string& out)
{
	std::ifstream in(filename, std::ios::binary);

	if (!in.is_open())
		return false;

	size_t file_size = get_file_size(in);

	out.resize(file_size);
	in.read(&out[0], file_size);
	in.close();

	size_t string_size = out.size();

	return true;
}

bool write_string_to_file(const char* filename, const std::string& str)
{
	std::ofstream out(filename, std::ios::binary);

	if (!out.is_open())
		return false;

	size_t string_size = str.size();
	out.write((const char*)&str[0], string_size);
	out.close();

	return true;
}

int wildcmp(const char *wild, const char *src) 
{
	while ((*src) && (*wild != '*')) {
		if ((*wild != *src) /*&& (*wild != '?')*/)
			return 0;

		wild++;
		src++;
	}

	const char *cp = NULL;
	const char *mp = NULL;

	while (*src) {	  
		if (*wild == '*') {
			if (!*++wild) 
				return 1;

			mp = wild;
			cp = src+1;
		} 
		else if ((*wild == *src) /*|| (*wild == '?')*/) {
			wild++;
			src++;
		} 
		else {
			wild = mp;
			src = cp++;
		}
	}

	while (*wild == '*') {
		++wild;
	}

	return !*wild;
}

#define CURL_STATICLIB
#include <curl/curl.h>

namespace
{
	static size_t curl_writer_(char *data, size_t size, size_t nmemb,  std::string *buffer)
	{
		if(!buffer || 0 == size)
			return 0;

		(*buffer) += std::string(data, size*nmemb);

		size_t result = size * nmemb; 
		return result;	
	}
	

	void get_file_from_url(const std::string& url, std::string& out)
	{
		if (CURL *curl = curl_easy_init() )
		{
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer_);		
			curl_easy_setopt(curl, CURLOPT_HEADER , 0 );  // 1- отобразить заголовок ; 0 – не отображать

			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, 	&out); 	

			curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L); // trying to fix "page not found" message in output string
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

			CURLcode code = curl_easy_perform(curl); 

			curl_easy_cleanup(curl); 

			if (CURLE_OK == code)
				return;
		}
		out.clear();
	}
}


bool read_file_to_string_from_url(const char* url, std::string& out)
{
	get_file_from_url(url, out);
	return !out.empty();
}


bool read_file(std::string file_name, std::string& data)
{
	static const std::string template_file_name = "http://localhost:21689/conf/";
	file_name =  template_file_name + file_name;
	return read_file_to_string_from_url(file_name.c_str(), data);
}