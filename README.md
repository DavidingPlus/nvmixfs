# nvmixfs

本科毕设：面向 NVM + SSD 的混合存储文件系统设计与实现。

本毕设致力于构建一个基于 Linux 内核模块的文件系统，内核版本是 5.4。

# 数据布局

本文件系统的数据布局如下：

```plaintext
+==================+==================+==================+==================+--
|   SuperBlock     |   Inode Zone     |    Data Block 0  |    Data Block 1  | ...
|    (Block 0)     |    (Block 1)     |     (Block 2)    |     (Block 3)    |
|------------------|------------------|------------------|------------------|
| [NvmixSuperBlock]| [NvmixInode[32]] | [File/Directory] | [File/Directory] | ...
|  4 KiB Metadata  |   4 KiB Inodes   |    4 KiB Data    |    4 KiB Data    |
+==================+==================+==================+==================+--
```

目前本文件系统设计的非常简单，磁盘块以 4 KIB 为单位。第一个块是超级块区，第二个块是 inode 区。数据块从第三个块，块号 2 开始。这也是文件系统经典的三段式布局。

# 具体设计

（待补）

super_block 区。

inode 区存放 NvmixInode 数组，用于管理本文件系统的所有 inode 元数据。目前限制了文件系统总 inode 的数量为 32，4 KIB 够用。

data 区以 4 KIB 为单位，每个文件或目录使用一个数据块，因此目前对文件的最大大小也限制为 4 KIB。文件的数据块存储数据的字节流。而目录的数据块存储 NvmixDentry 数组，记录该目录下所有的目录项的信息。

# 已完成

1. 不考虑 NVM，完成基于 DRAM + SSD 的传统文件系统的基本框架的搭建，留出数据结构和接口。初版只完成最简单的设计。
2. 基本完成框架中预留接口的实现，已实现功能包括文件的创建、删除和修改，目录的创建和删除等。
3. 按照 Doxygen 的风格补全注释。
4. 使用持久性内存模拟 NVM 空间，将其映射到内核虚拟地址空间，并拿到虚拟起始地址用于管理。但目前尚未修改文件系统，只做了架构性的调研和思考。

# 代办

1. 在 NVM 上部署元数据，修改文件系统的细节逻辑，完成收尾工作。

# 参考文档

1. [https://hanqi-blogs.cn/2024/linux%20kernel%20lab%20--%20Notes/](https://hanqi-blogs.cn/2024/linux%20kernel%20lab%20--%20Notes/)
2. [https://github.com/linux-kernel-labs/linux/](https://github.com/linux-kernel-labs/linux/)
3. [https://blog.csdn.net/qq_43845988/article/details/127018049](https://blog.csdn.net/qq_43845988/article/details/127018049)

