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
///TODO можно еще написать эксепшн чтобы не было лишних запятых
//[
//true,
//true,
//null,
//"str1",
//"str2"
//    ] в этом массиве после зпт ничего нет
TEST(JSON, LOADARRAY){
    using namespace BMSTU;
///TODO NUMBER
    std::stringstream ss;
    ss.str("[true, true, null, \"str1\", \"str2\"  ]");
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




///LOAD DICT_ARRAY
TEST(JSON, LOADDICT_ARRAY_VALUE){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("{\"key_ARRAY\" : [true, null, \"my_string\"]}");

    Document first = Load(ss);
    Document last = Document(Dict{{std::string("key_ARRAY"), Array{true, {}, std::string("my_string")}}});

    EXPECT_EQ(first, last);
}
///LOAD DICT_STRING
TEST(JSON, LOADDICT_STRING_VALUE){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("{\"key_STRING\" : \"my\tstring\nbetter\\than\tyours\"}");

    Document first = Load(ss);
    Document last = Document(Dict{{std::string("key_STRING"),
                                   std::string("my\tstring\nbetter\\than\tyours")}});
    EXPECT_EQ(first, last);
}
///LOAD DICT_BOOL
TEST(JSON, LOADDICT_BOOL_VALUE){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("{\"key_STRING\" : true}");

    Document first = Load(ss);
    Document last = Document(Dict{{std::string("key_STRING"), true}});
    EXPECT_EQ(first, last);
}

///LOAD DICT_NULL
TEST(JSON, LOADDICT_NULL_VALUE){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("{\"key_STRING\" : null}");

    Document first = Load(ss);
    Document last = Document(Dict{{std::string("key_STRING"), {}}});
    EXPECT_EQ(first, last);
}

///LOAD DICT_NUM
///TODO NUM

///LOAD DICT EXCEPTIONS

///can't add more than one VALUE
TEST(LOADDICT_EXC, MORE_VALUE){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("{\"key_STRING\" : null, true}");

    EXPECT_THROW(Document first = Load(ss), ParsingError);
}

///NO_DOUBLE_AT
TEST(LOADDICT_EXC, NO_DOUBLE_AT){
    using namespace BMSTU;

    std::stringstream ss, ss1;
    ss.str("{\"key_STRING\"} : true");

    EXPECT_THROW(Document first = Load(ss), ParsingError);
}



///LOAD NUMBER





///LOAD NODE EXCEPTION NOT EOF




///PRINT
