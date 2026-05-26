#!/bin/bash

qemu-system-aarch64 \
  -machine virt \
  -cpu cortex-a72 \
  -m 2048 \
  -smp 2 \
  -nographic \
  \
  -drive if=pflash,format=raw,readonly=on,file=/usr/share/AAVMF/AAVMF_CODE.fd \
  -drive if=pflash,format=raw,file=efi-vars.fd \
  \
  -drive if=virtio,file=debian11-arm64.img,format=qcow2 \
  \
  -boot d \
  \
  -virtfs local,path=.,mount_tag=host0,security_model=none,id=host0