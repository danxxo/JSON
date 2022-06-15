#include "JSON_parser/json.h"
#include <gtest/gtest.h>
#include <fstream>


#pragma region "LOAD NULL"

TEST(JSON, LOAD_NULL){
    using namespace BMSTU;

    std::string s = "null";
    std::stringstream ss;
    ss.str(s);
    Document first = Load(ss);
    Document last = Document({});
    EXPECT_EQ(first, last);
}

#pragma endregion

#pragma region "LOAD STRING"

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
           "there will be the\r tabulation\ttabulation\nhere is\0 the end\"");
    Document first = Load(ss);
    Print(first,std::cout);
    Document last = Document(std::string(("HELLO!\t\\there,\\\n"
                                          "there will be the\r tabulation\ttabulation\nhere is\0 the end")));
    EXPECT_EQ(first, last);
}

#pragma endregion

#pragma region "LOAD BOOL"

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

    std::stringstream ss;
    ss.str("flse");
    EXPECT_THROW(Document first = Load(ss), ParsingError);
}

#pragma endregion

#pragma region "LOAD ARRAY"

TEST(JSON, LOADARRAY){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("[true, true, null, \"str1\", \"str2\", 233, -678, 123.532]");

    Document first = Load(ss);
    Document last = Document(Array{true, true, {}, std::string("str1"), std::string("str2"), 233, -678, 123.532});
    EXPECT_EQ(first, last);
}

///LOAD ARRAY EXCEPTION
TEST(LOADARRAY, EXCEPTION){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("[[]");
    EXPECT_THROW(Document first = Load(ss), ParsingError);
}

#pragma endregion

#pragma region "LOAD DICT_ARRAY"

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
TEST(JSON, LOADDICT_NUM_VALUE){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("{\"key_STRING\" : -123.7645}");

    Document first = Load(ss);
    Document last = Document(Dict{{std::string("key_STRING"), -123.7645}});
    EXPECT_EQ(first, last);
}

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

#pragma endregion

#pragma region "LOAD NUMBER"

TEST(JSON, LOADNUMBER){
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("[123e-1, +12, +123.532, -165, -0.532456]");

    Document first = Load(ss);
    Document last = Document(Array{123e-1, 12, 123.532, -165, -0.532456});
    EXPECT_EQ(first, last);
}

///NO_DIGIT
TEST(NUM_EXCEPTION, NO_DIGIT){
    ///так как switch в Loadnode по дефолту уходит в LoadNum, тогда любой неправильно
    ///введенный символ уходит в ошибку no_digit
    using namespace BMSTU;

    std::stringstream ss;
    ss.str("[one, two, three]");

    EXPECT_THROW(Document first = Load(ss), ParsingError);
}

#pragma endregion

///LOAD NODE EXCEPTION NOT EOF
TEST(LOADNODE, Unexpected_EOF){
    using namespace BMSTU;

    std::ifstream input("false path");
    EXPECT_THROW(Document doc = BMSTU::Load(input), ParsingError);
}

//TEST(JSON, EOOOOOOOOOOOOOF){
//    std::ifstream input("/home/nikita/dev/cpp/yap_kurs_Busarov/JSON/popo.txt");
//    BMSTU::Document doc = BMSTU::Load(input);
//    BMSTU::Print(doc, std::cout);
//}

///PRINT
TEST(JSON, Print){
using namespace BMSTU;

std::stringstream ss;
ss.str("[{\"BOOL\" : [{\"TRUE\" : true}, {\"FALSE\" : false}]},"
       "{\"NULL\" : null}, {\"NUMBERS\" : [{\"INT\" : 12}, {\"DOUBLE\" : 12.52},"
       "{\"NEGATIVE\" : -123.5}, {\"EXPONENT\" : -13e-10}]}, "
       "{\"STRING\" : \"my\\\n\tstring\"}, "
       "{\"ARRAY\" : [{\"ARRAY_STRING\" : [\"string1\", \"string2\"]}, "
       "{\"ARRAY_NUM\" : [1,-2,2.542]}]}, {\"DICT\" : \"dict\"}]");

Document doc = Load(ss);
Print(doc,std::cout);
}
