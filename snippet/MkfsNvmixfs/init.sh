set -e


CURRENT_DIR="$(dirname "$(realpath "$0")")"


$CURRENT_DIR/clear.sh

sudo mkdir /mnt/nvmixfs/

sudo insmod "$CURRENT_DIR/../../build/linux/x86_64/debug/bin/nvmixfs.ko"

sudo "$CURRENT_DIR/../../build/linux/x86_64/debug/snippet/mkfs.nvmixfs" /dev/sdb2

sudo mount -t nvmixfs /dev/sdb2 /mnt/nvmixfs
