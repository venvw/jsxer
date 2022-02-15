#include "util.h"

#include <cstring>
#include <algorithm>

BEGIN_NS(jsxbin) BEGIN_NS(utils)

bool string_equal(const string& str1, const string& str2) {
    return strncmp(str1.c_str(), str2.c_str(), MIN(str1.length(), str2.length())) == 0;
}

void string_replace_char(string& str, char search, char replace) {
    std::replace(str.begin(), str.end(), search, replace);
}

void string_strip_char(string& str, char search) {
    str.erase(remove(str.begin(), str.end(), search), str.end());
}

void replace_str_inplace(string& subject, const string& search, const string& replace) {
    size_t pos = 0;

    while ((pos = subject.find(search, pos)) != string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

#define HEX_CHARSET_CAPITAL ("0123456789" "ABCDEF")
#define HEX_CHARSET_SMALL   ("0123456789" "abcdef")

string unicode_escape(uint16_t value, bool capital = false) {
    auto* cs = capital ? HEX_CHARSET_CAPITAL : HEX_CHARSET_SMALL;
    char result[] = { '\\', 'u', '0', '0', '0', '0', '\0' };

    for (int i = 0; i < 4; ++i) {
        auto hc = cs[(value >> (4 * i)) & 0xF];
        result[sizeof(result) - (i + 2)] = hc;
    }

    return result;
}

string hex_escape(uint8_t value, bool capital = false) {
    auto* cs = capital ? HEX_CHARSET_CAPITAL : HEX_CHARSET_SMALL;
    char result[] = { '\\', 'x', '0', '0', '\0' };

    for (int i = 0; i < 2; ++i) {
        auto hc = cs[(value >> (4 * i)) & 0xF];
        result[sizeof(result) - (i + 2)] = hc;
    }

    return result;
}

bool is_non_printable_ascii(uint32_t value) {
    // ([\x00-\x07\x0E-\x1F\x7F])
    return in_range_i(0, 7, value) ||
           in_range_i(0x0E, 0x1F, value) || (value == 0x7F);
}

bool is_non_printable_utf8(uint32_t value) {
    // ([\x00-\x07\x0E-\x1F\x7F\x80-\xFF])
    return is_non_printable_ascii(value) || in_range_i(0x80, 0xFF, value);
}

bool is_non_printable_utf16(uint32_t value) {
    return is_non_printable_ascii(value) || in_range_i(0x80, 0xFF, value) || (value > 0xFF);
}

string escape_hex_or_unicode(uint16_t value, bool capital = false) {
    if (in_range_i(0x00, 0xFF, value)) {
        return hex_escape((uint8_t) value, capital);
    }

    return unicode_escape(value, capital);
}

string string_literal_escape(uint16_t value, bool capital) {
    switch (value) {
        case '\b': return "\\b";
        case '\f': return "\\f";
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\v': return "\\v";
        case '\t': return "\\t";
        case '\"': return "\\\"";
        case '\'': return "\\\'";
        case '\\': return "\\\\";
        default:
            return is_non_printable_utf16(value)
                ? escape_hex_or_unicode(value, capital)
                : string(1, (char) value);
    }
}

string string_literal_escape(const ByteString& value, bool capital) {
    string res;

    for (const auto& c : value) {
        res += string_literal_escape(c, capital);
    }

    return res;
}

string to_string_literal(const ByteString& value, bool capital) {
    string res = "\"";

    for (auto& c : value) {
        res += string_literal_escape(c, capital);
    }

    return res + "\"";
}

string to_string_literal(const string& value, bool capital) {
    string res = "\"";

    for (auto& c : value) {
        res += string_literal_escape(c, capital);
    }

    return res + "\"";
}

string to_string(const ByteString& value) {
    string res;

    for (auto& c : value) {
        res += (char) c;
    }

    return res;
}

vector<string> string_split(const string& str, const string& delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = str.find(delimiter, pos_start)) != string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(str.substr(pos_start));
    return res;
}

// TODO: complete this
string double_to_string(double value) {
    size_t val = (*(size_t*) &value);
    string res;

    if (value == 0.0) {
        return "0";
    }
    else if (val == 0x7FEFFFFFFFFFFFFF) {
        return "1.7976931348623157e+308";
    }
    else if (val == 0xFFEFFFFFFFFFFFFF) {
        return "-1.7976931348623157e+308";
    } else if (value >= 1.0e21 || floor(value) != value) {
        if (value < 1.0e21 && value >= 0.000001) {
            int v11 = (int) log10(value);
            int v12 = 0;
            if (v11 >= 0) {
                v12 = v11;
            }

            int v13 = 15 - (value >= 1.0) - v12;

            if (v13 > 15) {
                v13 = 15;
            }

            char _buff[24] = {0};
            snprintf(_buff, sizeof(_buff), "%20.*f", v13, value);
            res.clear();
            res.append(_buff);
        } else {
            char _buff[24] = {0};
            snprintf(_buff, sizeof(_buff), "%20.*e", 15, value);
            res.clear();
            res.append(_buff);
            std::transform(
                res.begin(),
                res.end(),
                res.begin(),
                std::tolower
            );
        }

        vector<string> splits = string_split(res, "e");
        char* sp0 = (char*) splits[0].c_str();
        size_t l = splits.size();
        while (l > 1) {
            l--;
            char v17 = *sp0--;
            if ( v17 != '0' ) {
                if ( v17 == '.' ) {
                    l--;
                }
                splits[0].erase(-1, l + 1);
                break;
            }
        }

        if (res.length() > 1) {
            int i = 0;

            while (res[i] == '0') {
                if ((++i + 1) >= res.length()) {
                    goto LABEL_50;
                }
            }

            res.erase(i, 1);
        }

LABEL_50:
        return res;
    }

    return std::to_string(value);
}

int byte_length(uint64_t value) {
    int len = sizeof(uint64_t);
    auto* p = (uint8_t*) &value;

    while ((p[len - 1] == 0) && len) {
        len--;
    }

    return len;
}

string ltrim(const string& s, char target = ' ') {
    size_t start = s.find_first_not_of(target);
    return (start == string::npos) ? "" : s.substr(start);
}

string rtrim(const string& s, char target = ' ') {
    size_t end = s.find_last_not_of(target);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string trim(const string& s, char target = ' ') {
    return rtrim(ltrim(s, target), target);
}

uint64_t to_integer(double value) {
    return *(uint64_t*) &value;
}

bool is_double_type(double value) {
    return byte_length(*(uint64_t*) &value) == 8;
}

string simplify_number_literal(const string& value) {
    // TODO: impl
    // stuff like
    // - stripping off excess suffix zeroes for doubles.
    // - stripping off excess prefix zeroes for integers.
    // - formatting scientific number literal (eg: 1e5, 1.72e+5, etc).
    return value;
}

// TODO: fix formatting and rounding as in es
string number_to_string(double value) {
    int fp_precision = 15;
    const char* fmt;

    // integer -> 1-7 bytes in memory
    // double  -> 8 bytes in memory
    if (is_double_type(value)) {
        uint64_t val_u64 = to_integer(value);

        switch (val_u64) {
            case 0x7FEFFFFFFFFFFFFF: return "1.7976931348623157e+308";
            case 0xFFEFFFFFFFFFFFFF: return "-1.7976931348623157e+308";
            default: {
                if ((value >= 1.0e21) || (floor(value) != value)) {
                    if ((value < 1.0e21) && (value >= 0.000001)) {
                        int l10 = (int) log10(value);
                        int fpn = (l10 >= 0) ? l10 : 0;
                        fp_precision = 15 - (value >= 1.0) - fpn;
                        if (fp_precision > 15) {
                            fp_precision = 15;
                        }

                        fmt = "%20.*f";
                    } else {
                        fmt = "%20.*e";
                    }
                } else if (value >= 1000000000.0) {
                    fmt = "%*.0f";
                } else {
                    fmt = "%*.f";
                }
            }
        }
    }  else {
        fmt = "%*ld";
    }

    char _num_str_buff[32] = {0};
    snprintf(_num_str_buff, sizeof(_num_str_buff), fmt, fp_precision, value);

    return simplify_number_literal(trim(_num_str_buff, ' '));
}

bool bytes_eq(const uint8_t* b1, const uint8_t* b2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (b1[i] != b2[i]) {
            return false;
        }
    }

    return true;
}

void zero_mem(const void* buff, size_t size) {
    for (int i = 0; i < size; ++i) {
        ((uint8_t*) buff)[i] = '\0';
    }
}

END_NS(utils) END_NS(jsxbin)
