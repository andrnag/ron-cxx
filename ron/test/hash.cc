#include <iostream>
#include <cassert>
#include <unistd.h>
#include "ron/ron.hpp"
#include "ron/hash.hpp"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

struct IOSink {
    int out;
    IOSink(int fd) : out{fd} {}
    inline void update(uint8_t* buf, size_t size) {
        write(out, buf, size);
    }
};
typedef Stream<IOSink> IOStream;

void test_serialization () {
    Builder builder;
    builder.AppendNewOp(HEADER, Uuid{"1+src"}, Uuid{"lww"});
    builder.AppendNewOp(REDUCED, Uuid{"2+orig"}, Uuid{"1+src"}, "key", "value");
    TextFrame frame = builder.frame();
    const string &data = frame.data();
    Cursor cur = frame.cursor();
    SHA2 SRC_HASH;
    SHA2 LWW_HASH;
//    IOStream out{IOSink{STDOUT_FILENO}};
    hash_uuid(Uuid{"0+src"}, SRC_HASH);
    hash_uuid(Uuid{"lww"}, LWW_HASH);
//    WriteOpHashable<Frame, IOStream>(cur, out, SRC_HASH, LWW_HASH);
    SHA2Stream ophash;
    WriteOpHashable<Frame, SHA2Stream>(cur, ophash, SRC_HASH, LWW_HASH);
    SHA2 OP_HASH;
    ophash.close(OP_HASH.bits_);
    string okhex =  "97fa0525e009867adffe5e2c71f93057dfb8293c25c27292cd4caf230a0e39ec";
    string okbase = "a~d59U09XcgV~athSV_lLyztAJlalcAIoKnk8ldEEUl";
    assert(OP_HASH.hex()==okhex);
    assert(OP_HASH.base64()==okbase);
    assert(SHA2{okbase}==OP_HASH);
    assert(SHA2::valid(okbase));
    string not_a_hash = okbase;
    not_a_hash[SHA2::BASE64_SIZE-1] = '1';
    assert(!SHA2::valid(not_a_hash));
}

void test_partial_match () {
    SHA2 a = SHA2::hex("97fa0525e009867adffe5e2c71f93057dfb8293c25c27292cd4caf230a0e39ec");
    SHA2 a2 = SHA2::hex("97fa");
    assert(a.matches(a2));
    assert(a!=a2);
    assert(SHA2::hex("97fA0525E").matches(a));
    assert(SHA2::hex("97fa0").matches(a));
    assert(!SHA2::hex("97fa1").matches(a));
}

int main (int argn, char** args) {
    test_serialization();
    test_partial_match();
    return 0;
}
