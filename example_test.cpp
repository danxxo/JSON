#include "JSON_parser/json.h"
#include <gtest/gtest.h>

///LOAD

///LOAD NULL
TEST(JSON, LOAD_NULL){
    using namespace BMSTU;
    std::string s = "null";
    std::stringstream ss;
    ss.str(s);
    Document first = Load(ss);
    Document last = Document({});
    EXPECT_EQ(first, last);
}
///LOAD NULL HAS NO EXCEPTIONS

///LOAD STRING
TEST(JSON, LOADSTRING){
    using namespace BMSTU;
    std::stringstream ss;
    ss.str("\"some_string\"");
    Document first = Load(ss);
    Document last = Document(std::string("some_string"));
    Print(first,std::cout);
    Print(last, std::cout);
    EXPECT_EQ(first, last);
}

///LOAD STRING EXCEPTIONS
TEST(LOADSTRING_EXC, string_parsing_error){
    using namespace BMSTU;
    std::stringstream ss;
    ss.str("\"\0\"");
    EXPECT_THROW(Document first = Load(ss),ParsingError);
}

TEST(LOADSTRING_EXC, Unexpexted_end_of_line){
    using namespace BMSTU;
    std::stringstream ss;
    ss.str("\"p\nh\"some_string\"");
    EXPECT_THROW(Document first = Load(ss), ParsingError);
}


TEST(sefew, wefwe){
    std::string s;
    s.push_back('a');
    s.push_back('\n');
    s.push_back('b');
    std::cout << s;

}

















///PRINT
