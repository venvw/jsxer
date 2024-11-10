#include <jsxer.h>

const char compiled[] = "@JSXBIN@ES@2.0@MyBbyBnABMAbyBn0AFJBnASzDjTjFjQBAneBhPftJCnASzDjMjFjOCBXzGjMjFjOj\n"
                        "HjUjIDfVzJjBjSjHjVjNjFjOjUjTEf4kBkWkYAnftJDnASzDjSjFjTFCdCzChchdGXDfVEf4kBkWkYA\n"
                        "nndAjzJjVjOjEjFjGjJjOjFjEHfXzBhQIfVEf4kBkWkYAnftaEbyFn0ABJFnASFCCzBhLJCJVFfCVBf\n"
                        "AnndUzCjcjcKCzBhcLVzBjJMfDnndACGXDfVEf4kBkWkYAVMfDnnnnjHfQzANfVEf4kBkWkYAVMfDnn\n"
                        "nffAVMfDBVCfBByBLZHnAVFfCAEB40BiAC4B0AiAF4C0AiAM4D0AiAAEAzEjKjPjJjOOAI0ENByB";

int main() {
    string decompiled;
    int err = jsxer::decompile(compiled, decompiled);

    printf("--------------------------------------------------\n");
    printf("%s\n", decompiled.c_str());
    printf("--------------------------------------------------\n");

    return err;
}