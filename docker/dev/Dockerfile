FROM archlinux:latest

RUN pacman -Syu --noconfirm && \
	pacman -S --noconfirm base base-devel clang git llvm18 cmake ninja lld18

RUN pacman -Scc --noconfirm

RUN useradd user

ENV CC=clang
ENV CXX=clang++
ENV LLVM_DIR=/usr/lib/llvm18/lib/cmake/llvm
ENV LLD_DIR=/usr/lib/llvm18/lib/cmake/lld

WORKDIR /lily
VOLUME /lily
USER user

ENTRYPOINT ["/bin/bash"]
