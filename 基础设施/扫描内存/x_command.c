#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

// 定义虚拟地址类型
typedef uint32_t vaddr_t;

// 模拟内存读取函数
uint32_t vaddr_read(vaddr_t addr, int size) {
    // 在实际模拟器中，这里会从物理内存读取
    // 这里使用一个简单的模式填充内存内容
    return (addr << 8) | (0xFF >> (8 - size)); // 返回模式化的值
}

// x 命令处理函数
static int cmd_x(char *args) {
    // 检查参数是否存在
    if (args == NULL) {
        printf("too few parameters!\n");
        return 1;
    }
    
    // 复制参数（strtok 会修改原始字符串）
    char *args_copy = strdup(args);
    if (args_copy == NULL) {
        printf("memory allocation failed\n");
        return 1;
    }
    
    // 解析第一个参数（数量）
    char *arg = strtok(args_copy, " ");
    if (arg == NULL) {
        printf("too few parameters!\n");
        free(args_copy);
        return 1;
    }
    
    // 转换为整数
    int n = atoi(arg);
    if (n <= 0) {
        printf("invalid count: %s\n", arg);
        free(args_copy);
        return 1;
    }
    
    // 解析第二个参数（地址表达式）
    char *EXPR = strtok(NULL, " ");
    if (EXPR == NULL) {
        printf("too few parameters!\n");
        free(args_copy);
        return 1;
    }
    
    // 检查是否有额外参数
    if (strtok(NULL, " ") != NULL) {
        printf("too many parameters!\n");
        free(args_copy);
        return 1;
    }
    
    // 模拟表达式求值（实际项目中会有 expr 函数）
    bool success = true;
    // vaddr_t addr = expr(EXPR, &success); // 实际项目中取消注释
    // 这里使用简化版本：直接解析十六进制地址
    char *endptr;
    vaddr_t addr = strtol(EXPR, &endptr, 16);
    if (*endptr != '\0') {
        printf("invalid address expression: %s\n", EXPR);
        free(args_copy);
        return 1;
    }
    
    // 打印表头
    printf("Address      Byte0   Byte1   Byte2   Byte3   Value\n");
    printf("--------------------------------------------------\n");
    
    // 扫描内存区域
    for (int i = 0; i < n; i++) {
        // 读取32位值
        uint32_t data = vaddr_read(addr + i * 4, 4);
        
        // 打印地址
        printf("0x%08x  ", addr + i * 4);
        
        // 打印每个字节（小端序）
        uint32_t temp = data;
        for (int j = 0; j < 4; j++) {
            printf("0x%02x   ", temp & 0xFF);
            temp >>= 8;
        }
        
        // 打印完整32位值
        printf("0x%08x\n", data);
    }
    
    free(args_copy);
    return 0;
}

// 测试函数
void test_x_command(const char *input) {
    printf("\nTesting command: 'x %s'\n", input);
    char *args = strdup(input);
    int ret = cmd_x(args);
    printf("Return code: %d\n", ret);
    free(args);
}

int main() {
    printf("Memory Examine Command Test\n");
    printf("===========================\n");
    
    // 测试各种情况
    test_x_command("4 0x1000");     // 正常输入
    test_x_command("2 0x2000");     // 不同地址
    test_x_command("0 0x3000");     // 无效数量
    test_x_command("-1 0x4000");    // 负数量
    test_x_command("3");            // 缺少地址
    test_x_command("0x5000");       // 缺少数量
    test_x_command("");             // 空输入
    test_x_command("2 0x6000 extra"); // 额外参数
    test_x_command("2 not_hex");    // 无效地址
    
    return 0;
}
