#include "json.hpp"
#include <stdint.h>
#include <iostream>

class JsonParser {
public:
    virtual void toJson(nlohmann::json& j) const = 0;
    virtual void fromJson(const nlohmann::json& j) = 0;
};

void to_json(nlohmann::json& j, const JsonParser& trace) {
    trace.toJson(j);
}

void from_json(const nlohmann::json& j, JsonParser& trace) {
    trace.fromJson(j);
}

//通用解析
template <typename T>
void parseValue(const nlohmann::json &jobj, const char* key, T& var, bool printError) {
    try {
        jobj.at(key).get_to<T>(var);
    } catch (std::exception& e) {
        if (printError) {
            printf("failed with key \"%s\": %s\n", key, e.what());
        }
    }
}

//解析数值：支持将内容为数值的字符串类型转换为数值类型
template <typename I>
void parseNumber(const nlohmann::json &jobj, const char* key, I& num, bool printError) {
    try {
        auto& value = jobj.at(key);
        if (value.is_string()) {
            std::stringstream ss;
            ss << value.get<std::string>();
            ss >> num;
        } else {
            value.get_to<I>(num);
        }
    } catch (std::exception& e) {
        if (printError) {
            printf("failed with key \"%s\": %s\n", key, e.what());
        }
    }
}

//特化parseValue数值解析：都走parseNumber
#define SPECIFY_PARSE_INTEGER(T) \
template<>\
void parseValue(const nlohmann::json &jobj, const char* key, T& var, bool printError) {\
    parseNumber(jobj, key, var, printError);\
}
SPECIFY_PARSE_INTEGER(int)
SPECIFY_PARSE_INTEGER(uint32_t)
SPECIFY_PARSE_INTEGER(int64_t)
SPECIFY_PARSE_INTEGER(uint64_t)
SPECIFY_PARSE_INTEGER(long)
SPECIFY_PARSE_INTEGER(unsigned long)
SPECIFY_PARSE_INTEGER(int16_t)
SPECIFY_PARSE_INTEGER(uint16_t)
SPECIFY_PARSE_INTEGER(int8_t)
SPECIFY_PARSE_INTEGER(uint8_t)
SPECIFY_PARSE_INTEGER(char)
SPECIFY_PARSE_INTEGER(float)
SPECIFY_PARSE_INTEGER(double)

//特化parseValue bool解析：数值、字符串都可以解析为bool
template<>
void parseValue(const nlohmann::json &jobj, const char* key, bool& var, bool printError) {
    try {
        do {
            const auto& value = jobj.at(key);
            if (value.is_boolean()) {
                value.get_to<bool>(var);
                break;
            } else if (value.is_number_integer()) {
                var = value.get<int>() != 0;
                break;
            } else if (value.is_number_float()) {
                var = fabs(value.get<float>()) > __FLT_EPSILON__;
                break;
            } else if (value.is_string()) {
                const std::string& s = value.get<std::string>();
                if (s == "true" || s == "false") {
                    var = s == "true";
                    break;
                }
            }
            value.get_to<bool>(var);
        } while(0);
    } catch (std::exception& e) {
        if (printError) {
            printf("failed with key \"%s\": %s\n", key, e.what());
        }
    }
}

//特化parseValue字符串解析
template<>
void parseValue(const nlohmann::json &jobj, const char* key, std::string& var, bool printError) {
    try {
        auto& value = jobj.at(key);
        if (value.is_string()) {
            value.get_to<std::string>(var);
        } else {
            var = jobj.at(key).dump();
        }
    } catch (std::exception& e) {
        if (printError) {
            printf("failed with key \"%s\": %s\n", key, e.what());
        }
    }
}

#define TO_JSON(j, varName) j[#varName]=this->varName

#define FROM_JSON(j, varName) \
do {\
    parseValue(j, #varName, this->varName, true);\
} while(0)

struct Trace : public JsonParser {
    uint32_t event_time = 0;
    std::string event_key;
    struct Message : public JsonParser {
        int i = 0;

        void toJson(nlohmann::json& j) const override {
            TO_JSON(j, i);
        }

        void fromJson(const nlohmann::json& j) override {
            FROM_JSON(j, i);
        }
    };
    Message message;

    void toJson(nlohmann::json& j) const override {
        //先执行父类的toJson方法
        TO_JSON(j, event_time);
        TO_JSON(j, event_key);
        TO_JSON(j, message);
    }

    void fromJson(const nlohmann::json& j) override {
        //先执行父类的fromJson方法
        FROM_JSON(j, event_time);
        FROM_JSON(j, event_key);
        FROM_JSON(j, message);
    }
};

int main() {
    Trace trace;
    trace.event_time = 123;
    trace.event_key = "push_stream";
    nlohmann::json j(trace);
    printf("%s\n", j.dump().c_str());

    nlohmann::json jArray = nlohmann::json::parse("[1,2,3,4]");
    printf("jArray type: %s, value: %s\n", jArray.type_name(), jArray.dump().c_str());
    std::vector<int> array;
    jArray.get_to<std::vector<int>>(array);
    std::cout << "array size: " << array.size() << ", value: ";
    for (int i = 0; i < array.size(); i++) {
        std::cout << array[i] << (i == array.size() - 1 ? "\n" : ", ");
    }

    j = "{\"event_time\":\"10.5\",\"event_key\":[\"connect_start\"],\"message\":{\"i\":100}}"_json;
    j.get_to<Trace>(trace);
    std::cout << "trace event_time: " << trace.event_time << ", event_key: " << trace.event_key 
        << ", message: { i: " << trace.message.i << " }" << std::endl;
    return 0;
}
