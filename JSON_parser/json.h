#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <variant>
#include <string>
#include <sstream>
#include <fstream>

namespace BMSTU {
    using namespace std;
    using namespace std::string_literals;

    class Node;

    using Dict = std::map<std::string, Node>;

    using Array = std::vector<Node>;


    class ParsingError : public std::runtime_error {

    public:

        using std::runtime_error::runtime_error;

    };


    class Node final : private std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string> {

    public:

        using variant::variant;
        using Value = variant;

        Node(Value &value) {
            this->swap(value);
        }

        bool IsInt() const {
            return std::holds_alternative<int>(*this);
        }

        int AsInt() const {
            if (!IsInt())
                throw std::logic_error("Not as int");
            return std::get<int>(*this);
        }

        bool IsPureDouble() const {
            return std::holds_alternative<double>(*this);
        };

        bool IsDouble() const {
            return IsPureDouble() || IsInt();
        }

        double AsDouble() const {
            if (!IsDouble()){
                throw std::logic_error(" Not as Double");
            }
            return IsPureDouble() ? std::get<double>(*this) : AsInt(); // manager
        }

        bool IsBool() const{
            return std::holds_alternative<bool>(*this);
        }

        bool AsBool() const {
            if (!IsBool()){
                throw std::logic_error(" Not as Bool");
            }
            return std::get<bool>(*this);
        }

        bool IsNull() const{
            return std::holds_alternative<nullptr_t>(*this);
        }

        bool IsArray() const{
            return std::holds_alternative<Array>(*this);
        }

        const Array &AsArray() const{
            using namespace std::literals;
            if (!IsArray()){
                throw std::logic_error(" Is not Array");
            }
            return std::get<Array>(*this);
        }

        Array &GiveArray(){
            using namespace std::literals;
            if (!IsArray()){
                throw std::logic_error(" Is not Array");
            }
            return std::get<Array>(*this);
        }


        bool IsString() const {
            return std::holds_alternative<std::string>(*this);
        }

        const std::string &AsString() const {
            using namespace std::literals;

            if (!IsString())
                throw std::logic_error("Not as string");

            return std::get<std::string>(*this);
        }

        bool IsDict() const{
            return std::holds_alternative<Dict>(*this);
        }

        const Dict &AsDict(){
            using namespace std::literals;
            if (!IsDict()){
                throw std::logic_error(" Not a Dict ");
            }
            return std::get<Dict>(*this);
        }

        Dict &GiveDict(){
            using namespace std::literals;
            if (!IsDict()){
                throw std::logic_error(" Not a Dict ");
            }
            return std::get<Dict>(*this);
        }

        const Value &Get_value() const {
            return *this;
        }


    };
    
    inline bool operator==(const Node &lhs, const Node &rhs){
        return lhs.Get_value() == rhs.Get_value();
    }

    inline bool operator!=(const Node &lhs, const Node &rhs){
        return lhs.Get_value() != rhs.Get_value();
    }

    ///Load functions

    Node LoadNode(std::istream &input);

    Node LoadString(std::istream &input) {
        auto iterator = std::istreambuf_iterator<char>(input);
        auto end = std::istreambuf_iterator<char>();
        std::string my_string;
        while (iterator != end) {
            const char ch = *iterator;
            if (iterator != end){
                    switch (ch) {
                        case '\"':{
                            if (++iterator != end){
//                            my_string.push_back('"');
//                            my_string.push_back(*iterator);
                            iterator = end;
                            }
                            break;
                        }

                        case '\\':{
                            my_string.push_back('\\');
                            break;
                        }
                        case '\n':{
                            my_string.push_back('\n');
                            break;
                        }
                        case '\t':{
                            my_string.push_back('\t');
                            break;
                        }
                        case '\r':{
                            my_string.push_back('\r');
                            break;
                        }
                        default:{
                            my_string.push_back(ch);
                            break;
                        }
                    }
                } else if(iterator == end){
                throw ParsingError("string parsing error");
            } else my_string.push_back(ch);
            if (iterator == end){
                break;
            } else
            ++iterator;
            }
        return Node(std::move(my_string));
        }

    std::string LoadLiteral(std::istream &input) {
        std::string s;
        while (std::isalpha(input.peek())) {
            s.push_back(static_cast<char>(input.get()));
        }
        return s;
    }

    Node LoadArray(std::istream &input) {
        Array result;
        for (char c ; input >> c && c != ']';) {
            if (c != ',') {
                input.putback(c);
            }
            result.push_back(LoadNode(input));
        }
        if (!input) throw ParsingError("ARRAY parsing exception");
        return Node(std::move(result));
    }

    Node LoadDict(std::istream &input) {
        Dict dict;
        for (char c; input >> c && c != '}';) {
            if (c == '"') {
                std::string key = LoadString(input).AsString();
                if (input >> c && c == ':') {
                    dict.emplace(std::move(key), LoadNode(input));
                } else {
                    throw ParsingError(" : is expected but"s + c + "has been founded"s);
                }
            } else if (c == ','){
                throw ParsingError("can't add more than one VALUE");
            }
        }
        return dict;
    }

    Node LoadBool(std::istream &input) {
        const auto s = LoadLiteral(input);
        if (s == "true"s) {
            return Node(true);
        } else if (s == "false"s) {
            return Node(false);
        } else {
            throw ParsingError(" Failed to parse "s + s + "as bool"s);
        }
    }

    Node LoadNull(std::istream &input) {
        if (auto l = LoadLiteral(input); l == "null"s) {
            return Node{nullptr};
        } else {
            throw ParsingError(" Failed to parse Null "s);
        }
    }

    Node LoadNumber(std::istream &input) {
        std::string parsed_num;

        // std::function<void(std::string, std::istream&)>;
        auto read_char = [&parsed_num, &input] {
            parsed_num += static_cast<char>(input.get());
        };
        ///читаем чарик-цифру
        auto read_digit = [&input, read_char] {
            if (!std::isdigit(input.peek())) {
                throw ParsingError("no digit");
            }
            while(std::isdigit(input.peek())){
                read_char();
            }
        };
        ///парсим знак числа
        if (input.peek() == '-' || input.peek() == '+') {
            read_char();
        }
        ///ищем точку если число дробное
        if (input.peek() == '0') {
            read_char();
        } else {
            read_digit();
        }
        bool is_int = true; ///стринг числа инт
        ///читаем дробную часть
        if (input.peek() == '.') {
            read_char();
            read_digit();
            is_int = false;
        }

        ///exponent
        if (int ch = input.peek(); ch == 'e' || ch == 'E') {
            read_char();
            if (int ch = input.peek(); ch == '+' || ch == '-') {
                read_char();
            }
            read_digit();
            is_int = false; ///число с плавающей точкой чтобы парсить экспоненту
        }

        if(is_int){
            return std::stoi(parsed_num);
        }else {
            return std::stod(parsed_num);
        }

//        try {
//            if(is_int){
//                return std::stoi(parsed_num);
//            }else {
//                return std::stod(parsed_num);
//            }
//        } catch (...) {
//            throw ParsingError(" number error ");
//        }
    }

    Node LoadNode(std::istream &input) {
        char c;
        if (!input >> c) {
            throw ParsingError(" Error unexpected EOF ");
        }
        input >> c;
        switch (c){
            case '[':
                return LoadArray(input);
            case '{':
                return LoadDict(input);
            case '"':
                return LoadString(input);
            case 't':
                [[fallthrough]]; //так как 't' и 'f' относятся к LOADBOOL
            case 'f':
                input.putback(c);
                return LoadBool(input);
            case 'n':
                input.putback(c);
                return LoadNull(input);
            default:
                input.putback(c);
                return LoadNumber(input);
        }
    }


    class Document {
    public:
        explicit Document(Node root) : root_(std::move(root)) {

        }

        const Node &Get_root() const {
            return root_;
        }

    private:
        Node root_;

    };

    inline bool operator==(const Document &l, const Document &r){
        return l.Get_root() == r.Get_root();
    }

    inline bool operator!=(const Document &l, const Document &r){
        return l.Get_root() != r.Get_root();

    }


///структура Print_context для табуляций JSON

    struct PrintContext {

        std::ostream &out;
        int indent = 0;
        int indent_step = 3;

        void Print_indent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }

        PrintContext Indented() const {
            return {out, indent+indent_step, indent_step};
        }
    };
    
    ///Print

    void PrintNode(const Node &node, const PrintContext &ctx);

    void PrintString(const std::string &value, std::ostream &out) {
        out.put('"');
        using namespace std;
        for (const char c: value) {
            switch (c) {
                case '\r': {
                    out << "\r";
                    break;
                }
                case '\n': {
                    out << "\n";
                    break;
                }
                case '"': {
                    out << '\"';
                    break;
                }
                case '\\': {
                    out << "\\"s;
                    break;
                }
                default: {
                    out.put(c);
                    break;
                }
            }
        }
        out.put('"');
    }

    template<typename Value>
    void PrintValue(const Value &value, const PrintContext &ctx) {
        ctx.out << value;
    }

    template<>
    void PrintValue<std::string>(const std::string &value, const PrintContext &ctx) {
        PrintString(value, ctx.out);
    }

    template<>
    void PrintValue<std::nullptr_t>(const std::nullptr_t &value, const PrintContext &ctx) {
        ctx.out << "null"s;
    }

    template<>
    void PrintValue<bool>(const bool &value, const PrintContext &ctx) {
        ctx.out << (value ? "true"s : "false"s);
    }

    template<>
    void PrintValue<Array>(const Array &nodes, const PrintContext &ctx) {
        std::ostream &out = ctx.out;
        out << "[\n"s;
        bool first = true;
        auto inner_ctx = ctx.Indented();
        for (const Node &node: nodes) {
            if (first) {
                first = false;
            } else {
                out << ",\n"s;
            }
            inner_ctx.Indented();
            inner_ctx.Print_indent();
            PrintNode(node, inner_ctx);
        }
        out.put('\n');
        ctx.Print_indent();
        out.put(']');
        //out.put('\n');
    }

    template<>
    void PrintValue<Dict>(const Dict &nodes, const PrintContext &ctx) {
        std::ostream &out = ctx.out;
        out << "{\n"s;
        auto inner_ctx = ctx.Indented();
        for (const auto &[key, node]: nodes) {
            inner_ctx.Print_indent();
            PrintString(key, ctx.out);
            out << ": "s;
            PrintNode(node, inner_ctx);

        }
        out.put('\n');
        ctx.Print_indent();
        out << "}"s;
    }

    void PrintNode(const Node &node, const PrintContext &ctx) {
        std::visit([&ctx](const auto &value) {
            PrintValue(value, ctx);
        }, node.Get_value());
    }

    void Print(const Document &doc, std::ostream &output) {
        PrintNode(doc.Get_root(), PrintContext{output});
    }
    
    Document Load(std::istream &input){
        return Document(LoadNode(input));
    }

}
