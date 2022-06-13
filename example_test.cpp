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

TEST(LOADSTRING, ESCAPE_SYMBOLS){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("\"HELLO!\t\\there,\\\n"
           "there will be the tabulation\ttabulation\nhere is\0 the end\"");
    Document first = Load(ss);
    Print(first,std::cout);
    Document last = Document(std::string(("HELLO!\t\\there,\\\n"
                                          "there will be the tabulation\ttabulation\nhere is\0 the end")));
    EXPECT_EQ(first, last);
}

///LOAD BOOL
TEST(JSON, LOADBOOL_TRUE){
    using namespace BMSTU;

    std::string s = "true";
    std::stringstream ss;
    ss.str(s);
    Document first = Load(ss);
    Document last = Document(true);
    EXPECT_EQ(first, last);
}
TEST(JSON, LOADBOOL_FALSE){
    using namespace BMSTU;

    std::string s = "false";
    std::stringstream ss;
    ss.str(s);
    Document first = Load(ss);
    Document last = Document(false);
    EXPECT_EQ(first, last);
}

///LOAD BOOL EXCEPTION
TEST(LOADBOOL, EXCEPTION){
    using namespace BMSTU;

    std::string s = "flse";
    std::stringstream ss;
    ss.str(s);
    EXPECT_THROW(Document first = Load(ss), ParsingError);
}

///LOAD ARRAY
TEST(JSON, LOADARRAY){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("[true, true, null, \"str1\", \"str2\"   ]");
    Document first = Load(ss);
    Document last = Document(Array{true, true, {}, std::string("str1"), std::string("str2")});
    EXPECT_EQ(first, last);
}

///LOAD ARRAY EXCEPTION
TEST(LOADARRAY, EXCEPTION){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("[[]");
    EXPECT_THROW(Document first = Load(ss), ParsingError);
}




///LOAD DICT







///LOAD NUMBER





///LOAD NODE EXCEPTION NOT EOF




///PRINT
