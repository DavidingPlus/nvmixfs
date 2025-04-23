#include <iostream>

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>


#define PMEM_DEV "/dev/pmem0"
#define MAP_SIZE (1024 * 1024 * 256) // 映射 256 M


int main()
{
    // 打开设备文件
    int fd = open(PMEM_DEV, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    // 映射内存，获得虚拟地址
    void *pmem = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (pmem == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    // 写入数据（示例：写入一个整数）
    int *data = (int *)pmem;
    *data = 305419896;

    std::cout << "写入值: " << *data << std::endl;
    std::cout << "写入成功！" << std::endl;

    // 确保数据持久化（重要！）
    msync(pmem, sizeof(int), MS_SYNC);

    // 解除映射
    munmap(pmem, MAP_SIZE);

    // 关闭文件
    close(fd);


    return 0;
}
