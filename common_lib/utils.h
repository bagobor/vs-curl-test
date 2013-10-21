#pragma once


size_t get_file_size(std::ifstream& f);

bool read_file_to_string_from_url(const char* url, std::string& out);

bool write_string_to_file(const wchar_t* filename, const std::string& str);
bool read_file_to_string(const wchar_t* filename, std::string& out);


bool read_file_to_string(const char* filename, std::string& out);
bool write_string_to_file(const char* filename, const std::string& str);

int wildcmp(const char *wild, const char *src);

bool read_file(std::string file_name, std::string& data);

template
<
	size_t string_size = 256,
	typename char_type = char
>
struct cstr_t
{
	enum {size = string_size};
	char_type str[size];

	const char_type* c_str() const {return &str[0];}
	char_type* c_str() {return &str[0];}

	size_t length() const {return strlen(&str[0]);}

	char_type& operator[](size_t index) {return str[index];}
	const char_type& operator[](size_t index) const {return str[index];}
};

typedef cstr_t<256> cstr;
typedef cstr_t<256, wchar_t> wcstr;