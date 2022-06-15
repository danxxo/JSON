#include "JSON_parser/json.h"
#include <gtest/gtest.h>


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
    ss.str("\"hello\\t here is the \\\"quotes\\\"\\nhere is the \\ttabulation\\\\\"");
    Document first = Load(ss);
    Print(first,std::cout);
    Document last = Document(std::string(("hello\t here is the \"quotes\"\nhere is the \ttabulation\\")));
    EXPECT_EQ(first, last);
}

TEST(LOADSTR_EXC, Unknown_escape){
    using namespace BMSTU;

    std::stringstream ss("\"Hello\\h\"");

    try {
        Document doc = BMSTU::Load(ss);
    }catch (std::exception &exception){
        EXPECT_STREQ(exception.what(), "Unknown \\h escape symb");
    }
}

TEST(LOADSTR_EXC, EOL){
    using namespace BMSTU;

    std::stringstream ss("\"Hello\n\"");

    try {
        Document doc = BMSTU::Load(ss);
    }catch (std::exception &exception){
        EXPECT_STREQ(exception.what(), "Unexpected EOL");
    }
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

    try {
        Document first = Load(ss);
    } catch(std::exception &exception){
        EXPECT_STREQ(exception.what(),  " Failed to parse flse as bool");
    }
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
    try{
        Document doc = Load(ss);
    } catch (std::exception &exception){
        ASSERT_STREQ(exception.what(), "ARRAY parsing exception");
    }
}

#pragma endregion

#pragma region "LOAD DICT"

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
    ss.str("{\"key_STRING\" : \"my\\tstring\\nbetter\\\\than\\tyours\"}");

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

    try {
        Document first = Load(ss);
    } catch(std::exception &exception){
        EXPECT_STREQ(exception.what(),  "can't add more than one VALUE");
    }
}

///NO_DOUBLE_AT
TEST(LOADDICT_EXC, NO_DOUBLE_AT){
    using namespace BMSTU;

    std::stringstream ss, ss1;
    ss.str("{\"key_STRING\"} : true");

    try {
        Document first = Load(ss);
    } catch(std::exception &exception){
        EXPECT_STREQ(exception.what(),  " : is expected but } has been founded");
    }
}

TEST(LOADDICT_EXC, NO_VALUE){
    std::stringstream ss("{\"wadwd\" :}");
    try{
        BMSTU::Document doc = BMSTU::Load(ss);
    } catch (std::exception &exception){
        ASSERT_STREQ(exception.what(), "No value");
    }
}

TEST(LOADDICT_EXC, parsing_error){
    using namespace BMSTU;

    std::stringstream ss("{{");

    try {
        Document doc = BMSTU::Load(ss);
    }catch (std::exception &exception){
        EXPECT_STREQ(exception.what(), "DICT parsing exception");
    }
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
    ss.str("-.10.1");

    try{
        Document doc = Load(ss);
    } catch (std::exception &exception){
        ASSERT_STREQ(exception.what(), "digit Error");
    }
}

TEST(NUM_EXCEPTION, NUM_ERROR){
    using namespace BMSTU;

    std::stringstream ss("1.9770e+308");
        try{
        Document doc = Load(ss);
    } catch (std::exception &exception){
        ASSERT_STREQ(exception.what(), " number error ");
    }
}

#pragma endregion

///LOAD NODE EXCEPTION NOT EOF
TEST(LOADNODE, Unexpected_EOF){
    using namespace BMSTU;
    std::ifstream input("false path");
    try {
        Document doc = BMSTU::Load(input);
    }catch (std::exception &exception){
        EXPECT_STREQ(exception.what(), " Error unexpected EOF ");
    }
}

///PRINT
TEST(JSON, Print){
using namespace BMSTU;

std::stringstream ss;
ss.str("[{\"BOOL\" : [{\"TRUE\" : true}, {\"FALSE\" : false}]},"
       "{\"NULL\" : null}, {\"NUMBERS\" : [{\"INT\" : 12}, {\"DOUBLE\" : 12.52},"
       "{\"NEGATIVE\" : -123.5}, {\"EXPONENT\" : -13e-10}]}, "
       "{\"STRING\" : \"my\\\\\\n\\tstring\"}, "
       "{\"ARRAY\" : [{\"ARRAY_STRING\" : [\"string1\", \"string2\"]}, "
       "{\"ARRAY_NUM\" : [1,-2,2.542]}]}, {\"DICT\" : \"dict\"}]");

Document doc = Load(ss);
Print(doc,std::cout);
}

///NODE METHODS
TEST(JSON, NODE_METHODS){
    using namespace BMSTU;

    std::stringstream stream_int, stream_double, stream_bool, stream_null, stream_Array, stream_Dict;

    stream_int.str("12");
    stream_double.str("14.653");
    stream_bool.str("true");
    stream_Array.str("[1,2,3]");
    stream_Dict.str("{\"key\" : \"value\"}");
    stream_null.str("null");

    int value = LoadNode(stream_int).AsInt();
    EXPECT_EQ(value, 12);
    std::stringstream int_error("12.421");
    EXPECT_ANY_THROW(int error = LoadNode(int_error).AsInt());

    double val_double = LoadNode(stream_double).AsDouble();
    EXPECT_EQ(val_double, 14.653);
    std::stringstream double_error("true");
    EXPECT_ANY_THROW(double error = LoadNode(double_error).AsDouble());

    bool val_bool = LoadNode(stream_bool).AsBool();
    EXPECT_TRUE(val_bool);
    std::stringstream bool_error("12.421");
    EXPECT_ANY_THROW(bool error = LoadNode(bool_error).AsBool());

    bool is_null = LoadNode(stream_null).IsNull();
    EXPECT_TRUE(is_null);

    Array val_Array = LoadNode(stream_Array).AsArray();

    Dict val_Dict = LoadNode(stream_Dict).AsDict();
}
