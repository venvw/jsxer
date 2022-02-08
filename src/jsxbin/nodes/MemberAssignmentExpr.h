#pragma once

#include "AbstractNode.h"
#include "../decoders.h"
#include "BinaryExpr.h"

using namespace jsxbin;

namespace jsxbin::nodes {
    class MemberAssignmentExpr : public AbstractNode {
    public:
        explicit MemberAssignmentExpr(ScanState &scanState) : AbstractNode(scanState) {}

        void parse() override;

        string jsx() override;

    private:
        AbstractNode *variable;
        AbstractNode *expression;
        string literal;
        bool shorthand;
    };
}