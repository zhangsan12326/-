#include <stdio.h>
#include <string.h>
#include <stdint.h> // 包含 uint32_t 定义

// 定义 CPU 状态结构体 (简化版)
typedef struct {
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
} CPU_state;

// 全局 CPU 实例
CPU_state cpu;

// 打印监视点函数 (模拟实现)
void print_wp() {
    printf("Watchpoint status:\n");
    printf("  WP1: 0x1234 (enabled)\n");
    printf("  WP2: 0x5678 (disabled)\n");
    printf("  WP3: 0x9ABC (enabled)\n");
}

// info 命令处理函数
static int cmd_info(char *args) {
    // 创建可修改的副本 (strtok 会修改字符串)
    char *args_copy = args ? strdup(args) : NULL;
    char *arg = args_copy ? strtok(args_copy, " ") : NULL;
    
    if (arg == NULL) {
        printf("Usage: info <r|w>\n");
        printf("  r - show register values\n");
        printf("  w - show watchpoint status\n");
        if (args_copy) free(args_copy);
        return 1;
    }
    
    // printf("Argument: %s\n", arg); // 调试信息
    
    // CPU 寄存器信息
    if (strcmp(arg, "r") == 0) {
        printf("Register values:\n");
        printf("eax is 0x%08x\n", cpu.eax);
        printf("ecx is 0x%08x\n", cpu.ecx);
        printf("edx is 0x%08x\n", cpu.edx);
        printf("ebx is 0x%08x\n", cpu.ebx);
        printf("esp is 0x%08x\n", cpu.esp);
        printf("ebp is 0x%08x\n", cpu.ebp); 
        printf("esi is 0x%08x\n", cpu.esi);
        printf("edi is 0x%08x\n", cpu.edi);
        printf("---------------------------\n");
    }
    // 监视点信息
    else if(strcmp(arg, "w") == 0) {
        print_wp();
    }
    else {
        printf("Unknown option: '%s'\n", arg);
        printf("Valid options: r, w\n");
        free(args_copy);
        return 1;
    }
    
    if (args_copy) free(args_copy);
    return 0;
}

// 测试函数
void test_info_command(const char *input) {
    printf("\nTesting command: 'info %s'\n", input ? input : "(null)");
    char *args = input ? strdup(input) : NULL;
    int ret = cmd_info(args);
    printf("Return code: %d\n", ret);
    if (args) free(args);
}

int main() {
    // 初始化 CPU 寄存器值 (示例)
    cpu.eax = 0x12345678;
    cpu.ecx = 0x00000001;
    cpu.edx = 0x89ABCDEF;
    cpu.ebx = 0xDEADBEEF;
    cpu.esp = 0x7FFFFFF0;
    cpu.ebp = 0x7FFFFFE0;
    cpu.esi = 0x00000042;
    cpu.edi = 0x00000024;
    
    printf("CPU Info Command Test\n");
    printf("=====================\n");
    
    // 测试各种输入情况
    test_info_command("r");      // 显示寄存器
    test_info_command("w");      // 显示监视点
    test_info_command("x");      // 无效选项
    test_info_command("");       // 空参数
    test_info_command(NULL);     // NULL 参数
    test_info_command("r extra"); // 带额外参数 (会被忽略)
    
    return 0;
}

