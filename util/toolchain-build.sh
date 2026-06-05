#!/bin/bash

set -e

# =========================
# BlockOS Toolchain Builder
# x86_64-elf cross compiler
# =========================

TARGET=x86_64-elf
PREFIX="$HOME/opt/cross"
PATH="$PREFIX/bin:$PATH"

BINUTILS_VER=2.42
GCC_VER=14.2.0

JOBS=$(nproc)

echo "[BlockOS] Installing dependencies..."
sudo apt update
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo wget

mkdir -p $HOME/src
cd $HOME/src

# =========================
# BINUTILS
# =========================
echo "[BlockOS] Downloading binutils..."
wget -nc https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VER.tar.gz

tar -xf binutils-$BINUTILS_VER.tar.gz

mkdir -p build-binutils
cd build-binutils

../binutils-$BINUTILS_VER/configure \
  --target=$TARGET \
  --prefix=$PREFIX \
  --with-sysroot \
  --disable-nls \
  --disable-werror

make -j$JOBS
make install

cd ..

# =========================
# GCC
# =========================
echo "[BlockOS] Downloading GCC..."
wget -nc https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VER/gcc-$GCC_VER.tar.gz

tar -xf gcc-$GCC_VER.tar.gz

mkdir -p build-gcc
cd build-gcc

../gcc-$GCC_VER/configure \
  --target=$TARGET \
  --prefix=$PREFIX \
  --disable-nls \
  --enable-languages=c,c++ \
  --without-headers

make all-gcc -j$JOBS
make all-target-libgcc -j$JOBS

make install-gcc
make install-target-libgcc

cd ..

echo "[BlockOS] Cleaning..."
echo "[BlockOS] Toolchain installed to: $PREFIX"

echo "[BlockOS] Done!"
echo "[BlockOS] Use: $TARGET-gcc"
