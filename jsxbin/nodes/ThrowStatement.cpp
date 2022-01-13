//
// Created by Angelo DeLuca on 12/26/21.
//

#include "ThrowStatement.h"

void ThrowStatement::parse() {
    lineInfo = decoders::d_linfo(scanState);
    expression = decoders::d_node(scanState);
}

string ThrowStatement::jsx() {
    return "throw " + expression->jsx() + ';';
}
