// 用 AI 写的。验证 write() 函数执行后文件偏移量的变化。
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main()
{
    const char *filename = "testfile.txt";
    int fd;
    ssize_t bytes_written;
    off_t offset;

    // 打开文件（不存在则创建，存在则清空）
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("❌ 文件打开失败");
        exit(EXIT_FAILURE);
    }

    // 第一次写入：5字节
    bytes_written = write(fd, "Hello", 5);
    if (bytes_written != 5)
    {
        perror("❌ 第一次写入失败");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 验证偏移量自动更新
    offset = lseek(fd, 0, SEEK_CUR);
    printf("📝 第一次写入后偏移量: %ld (预期: 5)\n", (long)offset);

    // 第二次写入：5字节
    bytes_written = write(fd, "World", 5);
    if (bytes_written != 5)
    {
        perror("❌ 第二次写入失败");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 再次验证偏移量
    offset = lseek(fd, 0, SEEK_CUR);
    printf("📝 第二次写入后偏移量: %ld (预期: 10)\n", (long)offset);

    close(fd); // 关闭文件

    // 重新以只读模式打开验证内容
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("❌ 读取验证时打开文件失败");
        exit(EXIT_FAILURE);
    }

    char buffer[11];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1)
    {
        perror("❌ 读取文件内容失败");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_read] = '\0';
    printf("🔍 文件实际内容: \"%s\" (预期: \"HelloWorld\")\n", buffer);

    close(fd);

    // 附加测试：O_APPEND 模式下的行为
    fd = open(filename, O_WRONLY | O_APPEND);
    write(fd, "Append", 6);
    offset = lseek(fd, 0, SEEK_CUR);
    printf("\n🔧 O_APPEND 模式测试:\n");
    printf("📝 追加写入后偏移量: %ld (预期: 16)\n", (long)offset);
    close(fd);


    unlink("testfile.txt");


    return 0;
}
