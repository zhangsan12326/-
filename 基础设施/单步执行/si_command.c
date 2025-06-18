#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 模拟的 CPU 执行函数
void cpu_exec(int num_steps) {
    printf("Executing %d CPU instruction(s)\n", num_steps);
    // 实际模拟器中这里会执行指定数量的指令
}

// 实际的 si 命令函数
static int cmd_si(char *args) {   
    char *arg = strtok(args, " ");
    if (arg == NULL) {
        printf("too few arguments.\n");
        return 1;
    }
    int num = atoi(arg);
    cpu_exec(num);
    printf("OK\n");
    return 0;
}

// 测试函数
void test_si_command(const char *input) {
    // 创建可修改的字符串副本（strtok 会修改原始字符串）
    char *args = strdup(input);
    printf("\nTesting command: '%s'\n", input);
    printf("Return code: %d\n", cmd_si(args));
    free(args); // 释放复制的字符串
}

// 主函数
int main() {
    printf("Testing si command processor\n");
    printf("============================\n");
    
    test_si_command("5");        // 有效输入
    test_si_command("10");       // 有效输入
    test_si_command("");         // 空输入
    test_si_command("   ");      // 空白输入
    test_si_command("three");    // 无效数字
    test_si_command("5 extra");  // 额外参数
    
    return 0;
}
