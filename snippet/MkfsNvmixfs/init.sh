set -e


CURRENT_DIR="$(dirname "$(realpath "$0")")"


$CURRENT_DIR/clear.sh

cd $CURRENT_DIR/../../

xmake f -m debug --linux-headers=/usr/src/linux-headers-5.4.0-214-generic/ --with-gtest=true

xmake build

sudo mkdir /mnt/nvmixfs/

# 我本地预留了一段内存，起始物理地址是 0x100000000，大小是 0x10000000，即 256 M。
sudo insmod "$CURRENT_DIR/../../build/linux/x86_64/debug/bin/nvmixfs.ko" nvmixNvmPhyAddr=0x100000000 nvmixNvmPhySize=0x10000000

sudo "$CURRENT_DIR/../../build/linux/x86_64/debug/snippet/mkfs.nvmixfs" /dev/sdb2

sudo mount -t nvmixfs /dev/sdb2 /mnt/nvmixfs
