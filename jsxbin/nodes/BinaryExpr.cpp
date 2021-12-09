//
// Created by Angelo DeLuca on 12/1/21.
//

#include "BinaryExpr.h"

using namespace jsxbin::nodes;

string create_expr(const string &literal, AbstractNode *exprNode){
    bool parenthesis = false;
    string expression;

    if(exprNode != nullptr && strcmp(typeid(exprNode).name(), "BinaryExpr") == 0){
        BinaryExpr *binExpr = (BinaryExpr*) exprNode;
        expression = binExpr->get_op();
        parenthesis = true;

        bool associative = (strcmp(binExpr->get_op_name().c_str(), "*") == 0);
    }

    return expression;
}

void BinaryExpr::parse() {
    op_name = decoders::d_ident(scanState);
    left = decoders::d_node(scanState);
    right = decoders::d_node(scanState);
    literalLeft = decoders::d_variant(scanState);
    literalRight = decoders::d_variant(scanState);

}

string BinaryExpr::jsx() {
    return std::string();
}
